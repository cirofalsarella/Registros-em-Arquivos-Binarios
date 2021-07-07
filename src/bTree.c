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
    
    cache->header->status = '1';
    if (cache->index != NULL) {
        BinaryWriter_BTreeHeader(cache);
        fclose(cache->index);
    }
    
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
    RRN_t antRRN;
    RRN_t nextRRN;
} BRegister_t;

BRegister_t* InsertRegisterInNode(BTreeCache_t* cache, BNode_t* node, BRegister_t* reg);

/**
 * @brief Create and return an Register object
 */
BRegister_t* CreateRegister(RegKey_t key, ByteOffset_t regOffset, RRN_t antRRN, RRN_t nextRRN) {
    BRegister_t* reg = (BRegister_t*) malloc(sizeof(BRegister_t));
    reg->key = key;
    reg->fileOffset = regOffset;
    reg->antRRN = antRRN;
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

    return BinaryReader_BTreeNode(cache, node->childrenRRNs[i]);
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
    // Criar vetores auxiliares
    RRN_t filhos[BTREE_ORDER+1];
    ByteOffset_t offsets[BTREE_ORDER];
    RegKey_t chaves[BTREE_ORDER];

    filhos[BTREE_ORDER-1] = node->childrenRRNs[BTREE_ORDER-1];
    for (int i=0; i<BTREE_ORDER-1; i++) {
        filhos[i] = node->childrenRRNs[i];
        offsets[i] = node->regOffsets[i];
        chaves[i] = node->regKeys[i];
    }


    // Insere newReg ordenado nos vetores auxiliares
    int pos = 0;
    while (newReg->key > chaves[pos]) pos++;

    for (int i=BTREE_ORDER-1; i>pos; i--) {
        filhos[i+1] = filhos[i];
        offsets[i] = offsets[i-1];
        chaves[i] = chaves[i-1];
    }
    filhos[pos] = newReg->nextRRN;
    chaves[pos] = newReg->key;
    offsets[pos] = newReg->fileOffset;


    // Copia os primeiros no antigo e apaga registros removidos dele
    for (int i=0; i<BTREE_ORDER/2; i++) {
        node->childrenRRNs[i+1] = filhos[i+1];
        node->regOffsets[i] = offsets[i];
        node->regKeys[i] = chaves[i];
    }
    for (int i=BTREE_ORDER/2; i<BTREE_ORDER-1; i++) {
        node->childrenRRNs[i+1] = -1;
        node->regOffsets[i] = -1;
        node->regKeys[i] = -1;
    }
    node->indexedKeysCount = BTREE_ORDER/2;
    BinaryWriter_IncrementBTree(node, cache);


    // Cria novo nó e copia os ultimos registros
    BNode_t* partitioned = BNode_CreateNoChildren(node->isLeaf, cache->header->rrnNextNode);
    cache->header->rrnNextNode++;

    for (int i=0; i<(BTREE_ORDER-1)/2; i++) {
        partitioned->childrenRRNs[i] = filhos[i + BTREE_ORDER/2];
        partitioned->regOffsets[i] = offsets[i + BTREE_ORDER/2];
        partitioned->regKeys[i] = chaves[i+1 + BTREE_ORDER/2];
    }
    partitioned->indexedKeysCount = (BTREE_ORDER-1)/2;
    BinaryWriter_IncrementBTree(partitioned, cache);


    // Promove o nó do meio
    return CreateRegister(chaves[BTREE_ORDER/2], offsets[BTREE_ORDER/2], node->rrn, partitioned->rrn);
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
    node->childrenRRNs[pos] = reg->antRRN;
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

        printf("%d, %d\n", cache->root->rrn, node->rrn);
        if (promoted != NULL)   promoted = InsertRegisterInNode(cache, node, promoted);
    }
    BinaryWriter_IncrementBTree(node, cache);
    
    // Caso exista um nó promovido tenta inserir no próprio nó
    return promoted;
}



void  BTreeCache_Insert(BTreeCache_t* cache, RegKey_t key, ByteOffset_t fileOffset) {
    // <DEBUG>
    printf("%d\n", key);
    
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

    // <DEBUG>
    int i=0;
    while (cache->nodes[i] != NULL) {
        Printer_Node(cache, cache->nodes[i], 0);
        i++;
    }
    printf("\n");
}
