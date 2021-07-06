#include "bTree.h"
#include "binaryReader.h"
#include "binaryWriter.h"
#include "printer.h"
#include <assert.h>

// TODO: Realloc dinamico
#define INITIAL_NODE_COUNT 10000


BTreeCache_t* BTreeCache_Create(char* bTreeIndexFileName, char* indexOpenType, char* registersFileName, char* registerOpenType) {
    // Inits a B-Tree Cache
    BTreeCache_t* cache = calloc(1, sizeof(BTreeCache_t));

    if (bTreeIndexFileName != NULL) cache->index = fopen(bTreeIndexFileName, indexOpenType);
    if (registersFileName != NULL)  cache->registers = fopen(registersFileName, registerOpenType);

    cache->header = BHeader_Create('0', -1, 0);
    cache->nodes = calloc(INITIAL_NODE_COUNT, sizeof(BNode_t*));
    cache->root = NULL;

    return cache;
}

BTreeCache_t* BTreeCache_CreateFromFile(char* bTreeIndexFileName, char* indexOpenType, char* registersFileName, char* registerOpenType) {
    // Inits a B-Tree Cache
    BTreeCache_t* cache = calloc(1, sizeof(BTreeCache_t));
    
    cache->index = fopen(bTreeIndexFileName, indexOpenType);
    cache->registers = fopen(registersFileName, registerOpenType);
    
    // If FILE status == 1, error
    if (BinaryReader_BTreeHeaderAndRoot(cache, bTreeIndexFileName)) {
        return NULL;
    }
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
    if (current == NULL)    return NULL;

    for (int i = 0; i < current->indexedKeysCount; i++) {
        if (current->regKeys[i] == key)     return current;
        else if (current->regKeys[i] < key) return GetNodeByKeyRecur(cache, current->childrenRRNs[i], key);
    }

    return GetNodeByKeyRecur(cache, current->childrenRRNs[current->indexedKeysCount], key); 
}

BNode_t* GetBTreeNodeByKey(BTreeCache_t* cache, RegKey_t key) {
    return GetNodeByKeyRecur(cache, cache->root->rrn, key);
}

void BTreeCache_Free(BTreeCache_t* cache) {
    if (cache == NULL) return;

    if (cache->registers != NULL) fclose(cache->registers);
    if (cache->index != NULL) fclose(cache->index);
    
    BHeader_Free(cache->header);

    // This loop also frees root
    int i = 0;
    while (cache->nodes[i] != NULL) {
        BNode_Free(cache->nodes[i]);
        i++;
    }
    
    free(cache->nodes);
    free(cache);
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
 * @brief Get the Next Node object
 * 
 * @param cache 
 * @param node 
 * @param key 
 * @return BNode_t* 
 */
BNode_t* GetNextNode(BTreeCache_t* cache, BNode_t* node, RegKey_t key) {
    int i=0;
    while (key > node->regKeys[i] && i < node->indexedKeysCount) {
        i++;
    }

    if (key == 8820949) {
        printf("pos = %d, (%d)\n", i, node->rrn);
    }

    return BinaryReader_BTreeNode(cache, node->childrenRRNs[i]);
}

/**
 * @brief Moves a register from an node to the other with space
 * 
 * @param origem The node that will provide the register
 * @param dest The node that will receive the register
 * @param pos The position of the register in the origin node
 */
BRegister_t* LastRegTradeNode(BTreeCache_t* cache, BNode_t* source, BNode_t* dest) {
    // Salvando os valores do nó de origem
    BRegister_t* newReg = CreateRegister(source->regKeys[BTREE_ORDER-2], source->regOffsets[BTREE_ORDER-2], source->childrenRRNs[BTREE_ORDER-2], source->childrenRRNs[BTREE_ORDER-1]);

    // Atualizando o nó de origem
    source->indexedKeysCount--;

    source->childrenRRNs[BTREE_ORDER-2] = -1;
    source->regKeys[BTREE_ORDER-2] = -1;
    source->regOffsets[BTREE_ORDER-2] = -1;
    source->childrenRRNs[BTREE_ORDER-1] = -1;


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
    BNode_t* partitioned = BNode_CreateNoChildren(node->isLeaf, cache->header->rrnNextNode);
    cache->header->rrnNextNode++;

    // Pega os registros e muda os 2 maiores para o novo nó partição
    // Desconsidero o retorno pois sei que será NULL
    LastRegTradeNode(cache, node, partitioned); // node > (ant) Key (next); part < (ant) Key (next)
    InsertRegisterInNode(cache, node, newReg);  // < newReg
    LastRegTradeNode(cache, node, partitioned);
    
    // Remover o 1o registro do nó particionado
    BRegister_t* promovido = CreateRegister(partitioned->regKeys[0], partitioned->regOffsets[0], node->rrn, partitioned->rrn);
    
    partitioned->regKeys[0] = partitioned->regKeys[1];
    partitioned->regKeys[1] = -1;

    partitioned->regOffsets[0] = partitioned->regOffsets[1];
    partitioned->regOffsets[1] = -1;
    
    partitioned->childrenRRNs[0] = partitioned->childrenRRNs[1];
    partitioned->childrenRRNs[1] = partitioned->childrenRRNs[2];
    partitioned->childrenRRNs[2] = -1;

    partitioned->indexedKeysCount--;
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
    if (reg == NULL || node == NULL)   return NULL;

    // Se não tiver espaço vou ter de particionar
    if (node->indexedKeysCount == BTREE_ORDER-1) {
        return PartitionNode(cache, node, reg);
    }   // Caso contrário somente realizo a inserção simples


    // Encontro a posição que vou inserir
    int pos = 0;
    while (reg->key > node->regKeys[pos] && pos < node->indexedKeysCount) {
        pos++;
    }

    // Movo os registros 1 pra frente
    for (int i=node->indexedKeysCount-1; i>=pos; i--) {
        node->childrenRRNs[i+2] = node->childrenRRNs[i+1];
        node->regKeys[i+1] = node->regKeys[i];
        node->regOffsets[i+1] = node->regOffsets[i];
    }
    node->childrenRRNs[pos+1] = node->childrenRRNs[pos];

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
    BRegister_t* promoted = NULL;

    if (node->isLeaf) {     // Insere no próprio nó
        promoted = InsertRegisterInNode(cache, node, newReg);
    }
    else {                  // Insere em um nó filho
        BNode_t* filho = GetNextNode(cache, node, newReg->key);
        promoted = InsertNodeRecur(cache, filho, newReg);

        if (promoted != NULL)   promoted = InsertRegisterInNode(cache, node, promoted);
    }
    BinaryWriter_IncrementBTree(node, cache);
    
    // Caso exista um nó promovido tenta inserir no próprio nó
    return promoted;
}



void  BTreeCache_Insert(BTreeCache_t* cache, RegKey_t key, ByteOffset_t fileOffset) {
    printf("key: %d\n", key);

    // Confere se raiz existe
    if (cache->root == NULL) {
        // Cria nó raiz
        cache->root = BNode_CreateNoChildren(1, cache->header->rrnNextNode);
        cache->header->rrnNextNode++;
        cache->header->rootRRN = 0;
        cache->nodes[0] = cache->root;

        // Adiciona novo registro
        BRegister_t* newReg = CreateRegister(key, fileOffset, -1, -1);
        InsertRegisterInNode(cache, cache->root, newReg);   // não haverá promoção

        // Escreve ele em memória
        BinaryWriter_IncrementBTree(cache->root, cache);
    }
    else {
        // Cria e insere novo registro 
        BRegister_t* newReg = CreateRegister(key, fileOffset, -1, -1);
        BRegister_t* promoted = InsertNodeRecur(cache, cache->root, newReg);

        // Confere se existe promoção
        if (promoted != NULL) {
            cache->root = BNode_CreateNoChildren(0, cache->header->rrnNextNode);
            cache->header->rrnNextNode++;
            cache->header->rootRRN = cache->root->rrn;

            InsertRegisterInNode(cache, cache->root, promoted);
            BinaryWriter_IncrementBTree(cache->root, cache);
        }
    }

    int i=0;
    while (cache->nodes[i] != 0) {
        Printer_Node(cache, NULL, i);
        i++;
    }
    printf("\n");
}
