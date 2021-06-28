#include "bTreeCache.h"
#include <assert.h>

int BTreeCache_OpenIndexFile(BTreeCache_t* cache, const char* openMode) {
    if (cache->bTreeFile != NULL) return 1;
    cache->bTreeFile = fopen(cache->bTreeIndexFileName, openMode);
    return cache->bTreeFile != NULL;
}

 int BTreeCache_OpenRegistersFile(BTreeCache_t* cache, const char* openMode) {
     if (cache->registersFile != NULL) return 1;
    cache->registersFile = fopen(cache->registersFileName, openMode);
    return cache->registersFile != NULL;
 }

void BTreeCache_CloseIndexFile(BTreeCache_t* cache) {
    if (cache == NULL || cache->bTreeFile == NULL) return;
    fclose(cache->bTreeFile);
    cache->bTreeFile = NULL;
}

void BTreeCache_CloseRegistersFile(BTreeCache_t* cache) {
    if (cache == NULL || cache->registersFile == NULL) return;
    fclose(cache->registersFile);
    cache->registersFile = NULL;
}

/**
 * @brief Helper function that reads the B-Header and the root node from the given file. Used during BTreeCache_Create.
 * 
 * @param fileName 
 * @return BHeader_t* 
 */
BHeader_t* ReadBHeaderAndRootFromFile(BTreeCache_t* cache, char* fileName) {
    FILE* file = fopen(fileName, "rb");

    if (file == NULL) {
        printf("Arquivo índice de Arvore-B inexistente\n");
        exit(1);
        return NULL;
    }

    BHeader_t* header = BHeader_Create(0, -1, -1);
    fread(&header->status, sizeof(char), 1, file);

    // If the status is not '1', then the file is corrupted.
    if (header->status != '1') {
        printf("Arquivo índice de Arvore-B corrompido\n");
        exit(1);
        return NULL;
    }

    // Reads the other fields
    fread(&header->rootRRN, sizeof(RRN), 1, file);
    fread(&header->rrnNextNode, sizeof(RRN), 1, file);
    fread(&header->unused[0], sizeof(char), 68, file);

    cache->root = BTreeCache_GetNode(cache, header->rootRRN);
    cache->nodes[header->rootRRN-1] = cache->root;

    fclose(file);

    return header;
}

BTreeCache_t* BTreeCache_CreateFromFile(char* bTreeIndexFileName, char* registersFileName) {
    // Inits a B-Tree Cache
    BTreeCache_t* cache = calloc(1, sizeof(BTreeCache_t));
    cache->bTreeIndexFileName = bTreeIndexFileName;
    cache->registersFileName = registersFileName;
    cache->header = ReadBHeaderAndRootFromFile(cache, bTreeIndexFileName);
    cache->registersFile = NULL;
    cache->bTreeFile = NULL;

    if (cache->header->rrnNextNode > 0) {
        cache->nodes = calloc(cache->header->rrnNextNode, sizeof(BNode_t*));
    } else {
        cache->nodes = NULL;
    }
    return cache;
}

BNode_t* BTreeCache_GetNode(BTreeCache_t* cache, RRN nodeRRN) {
    if (nodeRRN < 0) { // Checks for NULL RRNs.
        return NULL;
    }

    assert(nodeRRN != 0); // "The RRN of NODES must be greater than zero."

    // Avoids unnecessary freads
    if (cache->nodes[nodeRRN-1] != NULL) {
        return cache->nodes[nodeRRN-1];
    }

    OFFSET byteOffsetOfNode = RRNToOffset(nodeRRN);
    fseek(cache->bTreeFile, byteOffsetOfNode, SEEK_SET);
    
    // Creates a NULL node
    BNode_t* node = BNode_CreateNoChildren(-1, -1);

    // Reads fields
    fread(&node->isLeaf, sizeof(char), 1, cache->bTreeFile);
    fread(&node->indexedKeysCount, sizeof(int32_t), 1, cache->bTreeFile);
    fread(&node->rrn, sizeof(RRN), 1, cache->bTreeFile);

    for (int i=0; i<BTREE_ORDER-1; i++) {
        fread(&node->childrenRRNs[i], sizeof(RRN), 1, cache->bTreeFile);
        fread(&node->regKeys[i], sizeof(REGKEY), 1, cache->bTreeFile);
        fread(&node->regOffsets[i], sizeof(OFFSET), 1, cache->bTreeFile);
    }
    fread(&node->childrenRRNs[BTREE_ORDER-1], sizeof(RRN), 1, cache->bTreeFile);

    return node;
}

/**
 * @brief Helper function used during queries by key.
 * 
 * @param cache
 * @param nodeRRN 
 * @param key 
 * @return BNode_t* 
 */
BNode_t* GetNodeByKeyRecur(BTreeCache_t* cache, RRN nodeRRN, REGKEY key) {
    BNode_t* current = BTreeCache_GetNode(cache, nodeRRN);

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

BNode_t* BTreeCache_GetNodeByKey(BTreeCache_t* cache, REGKEY key) {
    return GetNodeByKeyRecur(cache, cache->root->rrn, key);
}

void BTreeCache_BeginWrite(BTreeCache_t* bTreeCache) {
    bTreeCache->header->status = '0';
    fseek(bTreeCache->bTreeFile, 0, SEEK_SET);
    fwrite(&bTreeCache->header->status, sizeof(char), 1, bTreeCache->bTreeFile);
}

void BTreeCache_EndWrite(BTreeCache_t* bTreeCache) {
    bTreeCache->header->status = '1';
    fseek(bTreeCache->bTreeFile, 0, SEEK_SET);
    fwrite(&bTreeCache->header->status, sizeof(char), 1, bTreeCache->bTreeFile);
}


void BTreeCache_Free(BTreeCache_t* bTreeCache) {
    if (bTreeCache == NULL) return;

    if (bTreeCache->bTreeFile != NULL) {
        fclose(bTreeCache->bTreeFile);
        bTreeCache->bTreeFile = NULL;
    }

    if (bTreeCache->registersFile != NULL) {
        fclose(bTreeCache->registersFile);
        bTreeCache->registersFile = NULL;
    }

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
    REGKEY key;
    OFFSET fileOffset;
    RRN previousRRN;
    RRN nextRRN;
} BRegister_t;

/**
 * @brief Create and return an Register object
 */
BRegister_t* CreateRegister(REGKEY key, OFFSET regOffset, RRN previousRRN, RRN nextRRN) {
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
RRN SearchNextNodeRRN(BNode_t* node, REGKEY key) {
    if (node->isLeaf)    return -1;  // Não existe próximo nó

    for (int i=0; i<node->indexedKeysCount; i++) {
        if (node->regKeys[key] < key) return node->regKeys[i];
    }
    return node->regKeys[node->indexedKeysCount]; 
}

/**
 * @brief Inserts a register into the given node. The node passed must have enough space for the new register.
 * 
 * @param node
 * @param reg register to be inserted; its memory will be freed
 */
void InsertRegisterInNode(BNode_t* node, BRegister_t* reg) {
    assert(node->indexedKeysCount < BTREE_ORDER-1);

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
}

/**
 * @brief Moves a register from an node to the other with space
 * 
 * @param origem The node that will provide the register
 * @param dest The node that will receive the register
 * @param pos The position of the register in the origin node
 */
void RegTradeNode(BNode_t* source, BNode_t* dest, int pos) {
    assert(dest->indexedKeysCount != BTREE_ORDER-1);

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
    InsertRegisterInNode(dest, newReg);
}

/**
 * @brief Partitions a node to two.
 * 
 * @param header the header of the btree (used to create the new node)
 * @param node the node that is fullfilled
 * @param newReg the register to be inserted in the node
 * @return the new node created
 */
BRegister_t* PartitionsNode(BHeader_t* header, BNode_t* node, BRegister_t* newReg) {
    // Criando o nó de partição
    BNode_t* partitioned = BNode_CreateNoChildren(1, header->rrnNextNode);
    header->rrnNextNode++;

    // Pega os registros e muda os 2 maiores para o novo nó partição
    RegTradeNode(node, partitioned, BTREE_ORDER-1);
    InsertRegisterInNode(node, newReg);
    RegTradeNode(node, partitioned, BTREE_ORDER-1);
    
    // Remover o 1o registro do nó particionado
    BRegister_t* promovido = CreateRegister(partitioned->regKeys[0], partitioned->regOffsets[0], node->rrn, partitioned->rrn);
    
    partitioned->regKeys[0] = partitioned->regKeys[1];
    partitioned->regKeys[1] = -1;

    partitioned->regOffsets[0] = partitioned->regOffsets[1];
    partitioned->regOffsets[1] = -1;
    
    partitioned->childrenRRNs[0] = partitioned->childrenRRNs[1];
    partitioned->childrenRRNs[1] = partitioned->childrenRRNs[2];
    partitioned->childrenRRNs[2] = -1;


    // TODO:    Escrever nó particionado no disco

    return promovido;
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
        if (node->indexedKeysCount == (BTREE_ORDER - 1)) {
            return PartitionsNode(cache->header, node, newReg);
        // Se o nó estiver cheio vai particionar e promover um nó
        } else {
            InsertRegisterInNode(node, newReg);
        }
        // Caso tenha espaço no nó será realizada uma inserção simplefs
    }
    else {                  // Insere em um nó filho   
        BRegister_t* promoted = InsertNodeRecur(cache, BTreeCache_GetNode(cache, SearchNextNodeRRN(node, newReg->key)), newReg);

        // confere se um dos nós do filho foi promovido
        if (promoted != NULL) {
            if (node->indexedKeysCount == (BTREE_ORDER - 1)) {
                return PartitionsNode(node, promoted, newReg);
            // Se o nó estiver cheio vai particionar e promover um nó
            } else {
                InsertRegisterInNode(node, newReg);
            }
            // Caso tenha espaço no nó será realizada uma inserção simples
        }
    }

    // TODO:    escrever nó no disco

    return NULL;
}


void BTreeCache_Insert(BTreeCache_t* cache, REGKEY key, OFFSET fileOffset) {
    BRegister_t* newReg = CreateRegister(key, fileOffset, -1, -1);
    BRegister_t* promoted = InsertNodeRecur(cache, BTreeCache_GetNode(cache, cache->header->rootRRN), newReg);
    
    if (promoted != NULL) {
        BNode_t* root = BNode_CreateNoChildren(0, cache->header->rrnNextNode);
        cache->header->rrnNextNode++;
        cache->header->rootRRN = root->rrn;
        InsertRegisterInNode(root, promoted);

        // TODO:    Escrever nó promovido no arquivo de indice
    }
    // TODO:    Escrever header no arquivo de índice
}
