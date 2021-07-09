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
    BNode_Free(meta->root);
    free(meta);
}






// Insertion algorithm

/**
 * @brief Helper function to set the root node of the metadata.
 * 
 * @param meta 
 * @param newRoot 
 */
void MetadataSetRoot(BTreeMetadata_t* meta, BNode_t* newRoot) {
    meta->header->rootRRN = newRoot->rrn;
    BNode_Free(meta->root);
    
    meta->root = newRoot;
}

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
RRN_t GetNextNode(BTreeMetadata_t* meta, BNode_t* node, RegKey_t key) {
    int i=0;
    while (key > node->keys[i] && i < node->indexedKeysCount) {
        i++;
    }

    return node->childrenRRNs[i];
}

void printAux(RRN_t* filhos, ByteOffset_t* offsets, RegKey_t* chaves){
    for (int i=0; i<5; i++) {
        printf("(%d) %d ", filhos[i], chaves[i]);
    }
    printf("(%d)", filhos[5]);

    printf("\n");
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

    filhos[0] = node->childrenRRNs[0];
    for (int i=0; i<BTREE_ORDER-1; i++) {
        filhos[i+1] = node->childrenRRNs[i+1];
        offsets[i] = node->offsets[i];
        chaves[i] = node->keys[i];
    }

    // Insere newReg ordenado nos vetores auxiliares
    int pos = 0;
    while (pos < (BTREE_ORDER-1) && newReg->key > chaves[pos]) pos++;

    // if (pos == BTREE_ORDER-1) {
    //     printf("\nnewReg: %d\n", newReg->key);
    //     printf("\tAntes da partição:\n");
    //     Printer_Node(node);
    //     printAux(filhos, offsets, chaves);
    // }

    for (int i=BTREE_ORDER-1; i>=pos; i--) {
        filhos[i+1] = filhos[i];
        offsets[i] = offsets[i-1];
        chaves[i] = chaves[i-1];
    }
    filhos[pos+1] = newReg->filhoRRN;
    chaves[pos] = newReg->key;
    offsets[pos] = newReg->fileOffset;

    free(newReg);


    // Copia os primeiros no antigo e apaga registros removidos dele
    node->childrenRRNs[BTREE_ORDER-1] = -1;
    for (int i=0; i<BTREE_ORDER/2; i++) {
        node->childrenRRNs[i+1] = filhos[i+1];
        node->offsets[i] = offsets[i];
        node->keys[i] = chaves[i];
    }
    for (int i=BTREE_ORDER/2; i<BTREE_ORDER-1; i++) {
        node->childrenRRNs[i+1] = -1;
        node->offsets[i] = -1;
        node->keys[i] = -1;
    }
    node->indexedKeysCount = BTREE_ORDER/2;
    BinaryWriter_SeekAndWriteNode(node, meta);
    
    // Cria novo nó e copia os ultimos registros
    BNode_t* partitioned = BNode_CreateWithRRN(meta, node->isLeaf);

    partitioned->childrenRRNs[BTREE_ORDER/2] = filhos[BTREE_ORDER];
    for (int i=0; i<BTREE_ORDER/2; i++) {
        partitioned->childrenRRNs[i] = filhos[i + BTREE_ORDER/2 + 1];
        partitioned->keys[i] = chaves[i + BTREE_ORDER/2 + 1];
        partitioned->offsets[i] = chaves[i + BTREE_ORDER/2 + 1];
    }

    partitioned->indexedKeysCount = BTREE_ORDER/2;
    BinaryWriter_SeekAndWriteNode(partitioned, meta);

    // Cria o novo registro
    newReg = CreateRegister(chaves[BTREE_ORDER/2], offsets[BTREE_ORDER/2], partitioned->rrn);
    
    BNode_Free(partitioned);
    return newReg;
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
    while (reg->key > node->keys[pos] && pos < node->indexedKeysCount) {
        pos++;
    }

    // Movo os registros 1 pra frente
    for (int i=node->indexedKeysCount-1; i>=pos; i--) {
        node->childrenRRNs[i+2] = node->childrenRRNs[i+1];
        node->keys[i+1] = node->keys[i];
        node->offsets[i+1] = node->offsets[i];
    }
    node->childrenRRNs[pos+1] = node->childrenRRNs[pos];

    // Salvo o conteúdo da nova inserção
    node->keys[pos] = reg->key;
    node->offsets[pos] = reg->fileOffset;
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
BRegister_t* InsertNodeRecur(BTreeMetadata_t* meta, RRN_t nodeRRN, BRegister_t* newReg) {
    BNode_t* node = BinaryReader_BTreeNode(meta, nodeRRN);

    if (node == NULL) {
        fprintf(stderr, "Passed a NULL node to InsertNodeRecur. This should never happen. Returning...\n");
        return NULL;
    }

    BRegister_t* promoted = NULL;
    if (node->isLeaf == 1) { // Insere no próprio nó
        promoted = InsertRegisterInNode(meta, node, newReg);
    }
    else { // Insere em um nó child
        promoted = InsertNodeRecur(meta, GetNextNode(meta, node, newReg->key), newReg);

        if (promoted != NULL) {
            promoted = InsertRegisterInNode(meta, node, promoted);
        }
    }

    BinaryWriter_SeekAndWriteNode(node, meta);
    BNode_Free(node);
    
    // Caso exista um nó promovido tenta inserir no próprio nó
    return promoted;
}


void BTreeMetadata_Insert(BTreeMetadata_t* meta, RegKey_t key, ByteOffset_t fileOffset) {
    BRegister_t* newReg = CreateRegister(key, fileOffset, -1);

    // Se raiz existir ela vai estar guardada no meta
    if (meta->root == NULL) {
        // Cria nó raiz
        MetadataSetRoot(meta, BNode_CreateWithRRN(meta, TRUE));

        // Adiciona novo registro
        InsertRegisterInNode(meta, meta->root, newReg);

        // Escreve ele em memória
        BinaryWriter_SeekAndWriteNode(meta->root, meta);
    } else {
        // Cria e insere novo registro
        BRegister_t* promoted = InsertNodeRecur(meta, meta->root->rrn, newReg);

        // Confere se existe promoção
        if (promoted != NULL) {
            BNode_t* newRoot = BNode_CreateWithRRN(meta, FALSE);

            newRoot->childrenRRNs[0] = meta->root->rrn;
            MetadataSetRoot(meta, newRoot);

            InsertRegisterInNode(meta, meta->root, promoted);
            BinaryWriter_SeekAndWriteNode(meta->root, meta);
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
        if (current->keys[i] == key) return current;
        else if (current->keys[i] < key) return GetNodeByKeyRecur(meta, current->childrenRRNs[i], key);
    }

    return GetNodeByKeyRecur(meta, current->childrenRRNs[current->indexedKeysCount], key); 
}

BNode_t* BTreeMetadata_GetNodeByKey(BTreeMetadata_t* meta, RegKey_t key) {
    return GetNodeByKeyRecur(meta, meta->root->rrn, key);
}