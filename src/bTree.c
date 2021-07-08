#include "bTree.h"
#include "binaryReader.h"
#include "binaryWriter.h"
#include "printer.h"
#include <assert.h>

BTreeMetadata_t* BTreeMetadata_Create(char* bTreeIndexFileName, char* indexOpenType, char* registersFileName, char* registerOpenType) {
    // Inits a B-Tree Metadata
    BTreeMetadata_t* meta = calloc(1, sizeof(BTreeMetadata_t));

    if (bTreeIndexFileName != NULL) meta->bTreeIndexFile = fopen(bTreeIndexFileName, indexOpenType);
    if (registersFileName != NULL) meta->registersFile = fopen(registersFileName, registerOpenType);

    meta->header = BHeader_Create('0', -1, 0);
    meta->root = NULL;

    return meta;
}

BTreeMetadata_t* BTreeMetadata_CreateFromFile(char* bTreeIndexFileName, char* indexOpenType, char* registersFileName, char* registerOpenType) {
    // Inits a B-Tree Metadata
    BTreeMetadata_t* meta = calloc(1, sizeof(BTreeMetadata_t));
    
    meta->bTreeIndexFile = fopen(bTreeIndexFileName, indexOpenType);
    meta->registersFile = fopen(registersFileName, registerOpenType);
    
    // If FILE status == 1, error
    if (BinaryReader_BTreeHeaderAndRoot(meta)) {
        return NULL;
    }

    return meta;
}

void BTreeMetadata_Free(BTreeMetadata_t* meta) {
    if (meta == NULL) return;

    if (meta->registersFile != NULL) fclose(meta->registersFile);
    
    meta->header->status = '1';
    if (meta->bTreeIndexFile != NULL) {
        BinaryWriter_BTreeHeader(meta);
        fclose(meta->bTreeIndexFile);
    }
    
    BHeader_Free(meta->header);
    free(meta);
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
    RRN_t filhoRRN;
} BRegister_t;

BRegister_t* InsertRegisterInNode(BTreeMetadata_t* meta, BNode_t* node, BRegister_t* reg);

/**
 * @brief Create and return an Register object
 */
BRegister_t* CreateRegister(RegKey_t key, ByteOffset_t regOffset, RRN_t filho) {
    BRegister_t* reg = (BRegister_t*) malloc(sizeof(BRegister_t));
    reg->key = key;
    reg->fileOffset = regOffset;
    reg->filhoRRN = filho;

    return reg;
}

/**
 * @brief Get the Next Node object
 * 
 * @param meta 
 * @param node 
 * @param key 
 * @return BNode_t* 
 */
BNode_t* GetNextNode(BTreeMetadata_t* meta, BNode_t* node, RegKey_t key) {
    int i=0;
    while (key > node->regKeys[i] && i < node->indexedKeysCount) {
        i++;
    }

    return BinaryReader_BTreeNode(meta, node->childrenRRNs[i]);
}

/**
 * @brief Partitions a node to two.
 * 
 * @param meta the metadata of the tree
 * @param node the node that is fullfilled
 * @param newReg the register to be inserted in the node
 * @return the new node created
 */
BRegister_t* PartitionNode(BTreeMetadata_t* meta, BNode_t* node, BRegister_t* newReg) {
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
    while (pos < (BTREE_ORDER-1) && newReg->key > chaves[pos]) pos++;

    for (int i=BTREE_ORDER-1; i>=pos; i--) {
        filhos[i+1] = filhos[i];
        offsets[i] = offsets[i-1];
        chaves[i] = chaves[i-1];
    }
    filhos[pos] = newReg->filhoRRN;
    chaves[pos] = newReg->key;
    offsets[pos] = newReg->fileOffset;

    free(newReg);


    // Copia os primeiros no antigo e apaga registros removidos dele
    node->childrenRRNs[0] = filhos[0];  // [FLAG] remover e ver se funciona
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
    BinaryWriter_IncrementBTree(node, meta);


    // Cria novo nó e copia os ultimos registros
    BNode_t* partitioned = BNode_CreateNoChildren(node->isLeaf, meta->header->rrnNextNode);
    meta->header->rrnNextNode++;

    partitioned->childrenRRNs[BTREE_ORDER/2] = filhos[BTREE_ORDER];
    for (int i=0; i<BTREE_ORDER/2; i++) {
        partitioned->childrenRRNs[i] = filhos[i + BTREE_ORDER/2 + 1];
        partitioned->regKeys[i] = chaves[i + BTREE_ORDER/2 + 1];
        partitioned->regOffsets[i] = chaves[i + BTREE_ORDER/2 + 1];
    }

    partitioned->indexedKeysCount = BTREE_ORDER/2;
    BinaryWriter_IncrementBTree(partitioned, meta);

    // Cria o novo registro
    return CreateRegister(chaves[BTREE_ORDER/2], offsets[BTREE_ORDER/2], partitioned->rrn);
}

/**
 * @brief Inserts a register into the given node. The node passed must have enough space for the new register.
 * 
 * @param node node that will be updated
 * @param reg register to be inserted; its memory will be freed
 */
BRegister_t* InsertRegisterInNode(BTreeMetadata_t* meta, BNode_t* node, BRegister_t* reg) {
    if (reg == NULL || node == NULL)   return NULL;

    // Se não tiver espaço vou ter de particionar
    if (node->indexedKeysCount == BTREE_ORDER-1) {
        return PartitionNode(meta, node, reg);
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
    node->childrenRRNs[pos+1] = reg->filhoRRN;

    free(reg);
    
    node->indexedKeysCount++;
    return NULL;
}

/**
 * @brief Insert the register in the tree, takes care of partitions and promotions
 * 
 * @param meta the meta to acess the nodes in ram
 * @param node the current node of the insertion
 * @param newReg the new register to be included
 * @return in case of promotion returns the promoted reg, otherwise returns NULL
 */
BRegister_t* InsertNodeRecur(BTreeMetadata_t* meta, BNode_t* node, BRegister_t* newReg) {
    if (node == NULL) {
        fprintf(stderr, "Passed a NULL node to InsertNodeRecur. This should never happen. Returning...\n");
        return NULL;
    }

    BRegister_t* promoted = NULL;

    if (node->isLeaf) { // Insere no próprio nó
        promoted = InsertRegisterInNode(meta, node, newReg);
    }
    else { // Insere em um nó child
        BNode_t* child = GetNextNode(meta, node, newReg->key);
        assert(child != NULL);
        promoted = InsertNodeRecur(meta, child, newReg);

        if (promoted != NULL) {
            promoted = InsertRegisterInNode(meta, node, promoted);
        }
    }

    BinaryWriter_IncrementBTree(node, meta);
    
    // Caso exista um nó promovido tenta inserir no próprio nó
    return promoted;
}

void BTreeMetadata_Insert(BTreeMetadata_t* meta, RegKey_t key, ByteOffset_t fileOffset) {
    // [DEBUG]
    // printf("%d\n", key);
    
    // Confere se raiz existe
    if (meta->root == NULL) {
        // Cria nó raiz
        meta->root = BNode_CreateNoChildren(1, meta->header->rrnNextNode);
        meta->header->rrnNextNode++;
        meta->header->rootRRN = 0;

        // Adiciona novo registro
        BRegister_t* newReg = CreateRegister(key, fileOffset, -1);
        InsertRegisterInNode(meta, meta->root, newReg);   // não haverá promoção

        // Escreve ele em memória
        BinaryWriter_IncrementBTree(meta->root, meta);
    }
    else {
        // Cria e insere novo registro 
        BRegister_t* newReg = CreateRegister(key, fileOffset, -1);
        assert(meta->root != NULL);
        BRegister_t* promoted = InsertNodeRecur(meta, meta->root, newReg);

        // Confere se existe promoção
        if (promoted != NULL) {
            BNode_t* newRoot = BNode_CreateNoChildren(0, meta->header->rrnNextNode);
            meta->header->rrnNextNode++;

            newRoot->childrenRRNs[0] = meta->root->rrn;
            meta->root = newRoot;
            meta->header->rootRRN = meta->root->rrn;

            InsertRegisterInNode(meta, meta->root, promoted);
            BinaryWriter_IncrementBTree(meta->root, meta);
        }
    }
}


/**
 * @brief Helper function used during queries by key.
 * 
 * @param meta
 * @param nodeRRN 
 * @param key 
 * @return BNode_t* 
 */
BNode_t* GetNodeByKeyRecur(BTreeMetadata_t* meta, RRN_t nodeRRN, RegKey_t key) {
    BNode_t* current = BinaryReader_BTreeNode(meta, nodeRRN);
    if (current == NULL)    return NULL;

    for (int i = 0; i < current->indexedKeysCount; i++) {
        if (current->regKeys[i] == key) return current;
        else if (current->regKeys[i] < key) return GetNodeByKeyRecur(meta, current->childrenRRNs[i], key);
    }

    return GetNodeByKeyRecur(meta, current->childrenRRNs[current->indexedKeysCount], key); 
}

BNode_t* BTreeMetadata_GetNodeByKey(BTreeMetadata_t* meta, RegKey_t key) {
    return GetNodeByKeyRecur(meta, meta->root->rrn, key);
}