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
    fread(&header->noRaiz, sizeof(RRN), 1, file);
    fread(&header->RRNproxNo, sizeof(RRN), 1, file);
    fread(&header->lixo[0], sizeof(char), 68, file);

    cache->root = BTreeCache_GetNode(cache, header->noRaiz);
    cache->nodes[header->noRaiz-1] = cache->root;

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

    if (cache->header->RRNproxNo > 0) {
        cache->nodes = calloc(cache->header->RRNproxNo, sizeof(BNode_t*));
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
    fread(&node->folha, sizeof(char), 1, cache->bTreeFile);
    fread(&node->nroChavesIndexadas, sizeof(int32_t), 1, cache->bTreeFile);
    fread(&node->RRNdoNo, sizeof(RRN), 1, cache->bTreeFile);

    for (int i=0; i<BTREE_ORDER-1; i++) {
        fread(&node->P[i], sizeof(RRN), 1, cache->bTreeFile);
        fread(&node->C[i], sizeof(REGKEY), 1, cache->bTreeFile);
        fread(&node->PR[i], sizeof(OFFSET), 1, cache->bTreeFile);
    }
    fread(&node->P[BTREE_ORDER-1], sizeof(RRN), 1, cache->bTreeFile);

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

    if (current->nroChavesIndexadas < (BTREE_ORDER+1)/2-1 || current->nroChavesIndexadas > BTREE_ORDER-1) {
        printf("Arvore-B corrompida. Um ou mais nó possuem nroChavesIndexadas invalido: %d.", current->nroChavesIndexadas);
        exit(1);
    }

    for (int i=0; i<current->nroChavesIndexadas; i++) {
        if (current->C[key] < key) return current->C[i];
    }
    return current->C[current->nroChavesIndexadas]; 
}

BNode_t* BTreeCache_GetNodeByKey(BTreeCache_t* cache, REGKEY key) {
    return GetNodeByKeyRecur(cache, cache->root->RRNdoNo, key);
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
    for (int i = 0; i < bTreeCache->header->RRNproxNo; ++i) {
        BNode_Free(bTreeCache->nodes[i]);
    }

    free(bTreeCache->nodes);
    free(bTreeCache);
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