#include "bTree.h"
#include "binaryReader.h"
#include "binaryWriter.h"
#include <assert.h>

#define INITIAL_NODE_COUNT 10000

BTreeCache_t* BTreeCache_Create(char* bTreeIndexFileName, char* registersFileName) {
    // Inits a B-Tree Cache
    BTreeCache_t* cache = calloc(1, sizeof(BTreeCache_t));
    cache->bTreeIndexFileName = bTreeIndexFileName;
    cache->registersFileName = registersFileName;
    cache->header = BHeader_Create('1', -1, 1);
    cache->nodes = calloc(INITIAL_NODE_COUNT, sizeof(BNode_t*));
    cache->root = NULL;

    return cache;
}

BTreeCache_t* BTreeCache_CreateFromFile(char* bTreeIndexFileName, char* registersFileName) {
    // Inits a B-Tree Cache
    BTreeCache_t* cache = calloc(1, sizeof(BTreeCache_t));
    cache->bTreeIndexFileName = bTreeIndexFileName;
    cache->registersFileName = registersFileName;
    cache->header = BinaryReader_BTreeHeaderAndRoot(cache, bTreeIndexFileName);
    cache->nodes = calloc(INITIAL_NODE_COUNT, sizeof(BNode_t*));

    return cache;
}


/**
 * @brief Helper function used during queries by key.
 * 
 * @param cache
 * @param nodeRRN 
 * @param key 
 * @return BNode_t* 
 */
BNode_t* GetNodeByKeyRecur(BTreeCache_t* cache, RRN_t nodeRRN, RegKey_t key) {
    BNode_t* current = BinaryReader_BTreeNode(cache, nodeRRN);

    if (current == NULL) {
        return NULL;
    }

    if (current->indexedKeysCount < (BTREE_ORDER+1)/2-1 || current->indexedKeysCount > BTREE_ORDER-1) {
        printf("Arvore-B corrompida. Um ou mais nó possuem indexedKeysCount invalido: %d.", current->indexedKeysCount);
        exit(1);
    }

    for (int i = 0; i < current->indexedKeysCount; i++) {
        if (current->regKeys[i] == key) return current;
        else if (current->regKeys[i] < key) return GetNodeByKeyRecur(cache, current->childrenRRNs[i], key);
    }

    return GetNodeByKeyRecur(cache, current->childrenRRNs[current->indexedKeysCount], key); 
}

BNode_t* GetBTreeNodeByKey(BTreeCache_t* cache, RegKey_t key) {
    return GetNodeByKeyRecur(cache, cache->root->rrn, key);
}

void BTreeCache_Free(BTreeCache_t* bTreeCache) {
    if (bTreeCache == NULL) return;

    // Frees everything
    free(bTreeCache->bTreeIndexFileName);
    free(bTreeCache->registersFileName);
    BHeader_Free(bTreeCache->header);
    
    // NOTE: This for loop also frees root
    for (int i = 0; i < bTreeCache->header->rrnNextNode; ++i) {
        BNode_Free(bTreeCache->nodes[i]);
    }

    free(bTreeCache->nodes);
    free(bTreeCache);
}




// Insertion algorithm


/**
 * @brief Struct to help manipulate registers
 * @param key the key of the register
 * @param fileOffset the offset of the record in the main file (in bytes)
 * @param p_ant the RRN of the node before
 * @param p_prox the RRN of the node after
 */
typedef struct bRegister {
    RegKey_t key;
    ByteOffset_t fileOffset;
    RRN_t previousRRN;
    RRN_t nextRRN;
} BRegister_t;

BRegister_t* InsertRegisterInNode(BTreeCache_t* cache, BNode_t* node, BRegister_t* reg);

/**
 * @brief Create and return an Register object
 */
BRegister_t* CreateRegister(RegKey_t key, ByteOffset_t regOffset, RRN_t previousRRN, RRN_t nextRRN) {
    BRegister_t* reg = (BRegister_t*) malloc(sizeof(BRegister_t));
    reg->key = key;
    reg->fileOffset = regOffset;
    reg->previousRRN = previousRRN;
    reg->nextRRN = nextRRN;
    return reg;
}

/**
 * @brief Finds the RRN of the next node on the search
 * 
 * @param node node with the pointers
 * @param key  key to be searched
 * @return the next node rrn
 */
RRN_t SearchNextNodeRRN(BNode_t* node, RegKey_t key) {
    if (node->isLeaf)    return -1;  // Não existe próximo nó

    for (int i=0; i<node->indexedKeysCount; i++) {
        if (node->regKeys[key] < key) return node->regKeys[i];
    }
    return node->regKeys[node->indexedKeysCount]; 
}

/**
 * @brief Moves a register from an node to the other with space
 * 
 * @param origem The node that will provide the register
 * @param dest The node that will receive the register
 * @param pos The position of the register in the origin node
 */
BRegister_t* RegTradeNode(BTreeCache_t* cache, BNode_t* source, BNode_t* dest, int pos) {
    // Salvando os valores do nó de origem
    BRegister_t* newReg = CreateRegister(source->regKeys[pos], source->regOffsets[pos], source->childrenRRNs[pos], source->childrenRRNs[pos+1]);

    // Atualizando o nó de origem
    source->indexedKeysCount--;
    for (int i=pos; i<source->indexedKeysCount; i++) {
        source->childrenRRNs[i] = source->childrenRRNs[i+1];
        source->regKeys[i] = source->regKeys[i+1];
        source->regOffsets[i] = source->regOffsets[i+1];
    }
    source->childrenRRNs[source->indexedKeysCount] = source->childrenRRNs[source->indexedKeysCount+1];

    source->childrenRRNs[source->indexedKeysCount] = -1;
    source->regKeys[source->indexedKeysCount] = -1;
    source->regOffsets[source->indexedKeysCount] = -1;
    source->childrenRRNs[source->indexedKeysCount+1] = -1;


    // Inserindo no nó de destino
    return InsertRegisterInNode(cache, dest, newReg);
}

/**
 * @brief Partitions a node to two.
 * 
 * @param header the header of the btree (used to create the new node)
 * @param node the node that is fullfilled
 * @param newReg the register to be inserted in the node
 * @return the new node created
 */
BRegister_t* PartitionNode(BTreeCache_t* cache, BNode_t* node, BRegister_t* newReg) {
    // Criando o nó de partição
    BNode_t* partitioned = BNode_CreateNoChildren(1, &(cache->header->rrnNextNode));

    // Pega os registros e muda os 2 maiores para o novo nó partição
    // Desconsidero o retorno pois sei que será NULL
    RegTradeNode(cache, node, partitioned, BTREE_ORDER-1);
    InsertRegisterInNode(cache, node, newReg);
    RegTradeNode(cache, node, partitioned, BTREE_ORDER-1);
    
    // Remover o 1o registro do nó particionado
    BRegister_t* promovido = CreateRegister(partitioned->regKeys[0], partitioned->regOffsets[0], node->rrn, partitioned->rrn);
    
    partitioned->regKeys[0] = partitioned->regKeys[1];
    partitioned->regKeys[1] = -1;

    partitioned->regOffsets[0] = partitioned->regOffsets[1];
    partitioned->regOffsets[1] = -1;
    
    partitioned->childrenRRNs[0] = partitioned->childrenRRNs[1];
    partitioned->childrenRRNs[1] = partitioned->childrenRRNs[2];
    partitioned->childrenRRNs[2] = -1;


    BinaryWriter_IncrementBTree(partitioned, cache);

    return promovido;
}

/**
 * @brief Inserts a register into the given node. The node passed must have enough space for the new register.
 * 
 * @param node node that will be updated
 * @param reg register to be inserted; its memory will be freed
 */
BRegister_t* InsertRegisterInNode(BTreeCache_t* cache, BNode_t* node, BRegister_t* reg) {
    if (reg == NULL || node == NULL)    return NULL;

    // Se não tiver espaço vou ter de particionar
    if (node->indexedKeysCount == BTREE_ORDER-1) {
        return PartitionNode(cache, node, reg);
    }   // Caso contrário somente realizo a inserção simples

    // Encontro a posição que vou inserir
    int pos = 0;
    while (reg->key > node->regKeys[pos] && node->regKeys[pos] != -1) {
        pos++;
    }

    // Movo os registros 1 pra frente
    for (int i=node->indexedKeysCount; i>pos; i++) {
        node->regKeys[i] = node->regKeys[i-1];
        node->regOffsets[i] = node->regOffsets[i-1];
        node->childrenRRNs[i] = node->childrenRRNs[i];
    }

    // Salvo o conteúdo da nova inserção
    node->regKeys[pos] = reg->key;
    node->regOffsets[pos] = reg->fileOffset;
    node->childrenRRNs[pos] = reg->previousRRN;
    node->childrenRRNs[pos+1] = reg->nextRRN;
    free(reg);

    node->indexedKeysCount++;
    return NULL;
}

/**
 * @brief Insert the register in the tree, takes care of partitions and promotions
 * 
 * @param cache the cache to acess the nodes in ram
 * @param node the current node of the insertion
 * @param newReg the new register to be included
 * @return in case of promotion returns the promoted reg, otherwise returns NULL
 */
BRegister_t* InsertNodeRecur(BTreeCache_t* cache, BNode_t* node, BRegister_t* newReg) {
    if (node->isLeaf) {      // Insere no próprio nó
        InsertRegisterInNode(cache, node, newReg);
    }
    else {                  // Insere em um nó filho   
        BRegister_t* promoted = InsertNodeRecur(cache, BinaryReader_BTreeNode(cache, SearchNextNodeRRN(node, newReg->key)), newReg);
        InsertRegisterInNode(cache, node, promoted);
    }

    BinaryWriter_IncrementBTree(node, cache);

    return NULL;
}


void  BTreeCache_Insert(BTreeCache_t* cache, RegKey_t key, ByteOffset_t fileOffset) {
    // Checks if we need to add root
    if (cache->root == NULL) {
        cache->root = BNode_CreateNoChildren(1, &(cache->header->rrnNextNode));
        cache->header->rootRRN = cache->root->rrn;
        cache->nodes[cache->header->rrnNextNode++] = cache->root;
        BRegister_t* newReg = CreateRegister(key, fileOffset, -1, -1);
        InsertRegisterInNode(cache, cache->root, newReg); // Also frees memory
        BinaryWriter_IncrementBTree(cache->root, cache); // Writes root to disk
        return;
    }

    // Else, inserts assuming root is defined
    BRegister_t* newReg = CreateRegister(key, fileOffset, -1, -1);
    BRegister_t* promoted = InsertNodeRecur(cache, BinaryReader_BTreeNode(cache, cache->header->rootRRN), newReg);
    
    if (promoted != NULL) {
        BNode_t* root = BNode_CreateNoChildren(0, &(cache->header->rrnNextNode));
        cache->header->rootRRN = root->rrn;
        InsertRegisterInNode(cache, root, promoted);
        BinaryWriter_IncrementBTree(root, cache);
    }

    BinaryWriter_BTreeHeader(cache);
}
