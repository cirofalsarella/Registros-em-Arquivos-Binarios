#include "bTreeCache.h"
#include <assert.h>

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
    fread(&header->noRaiz, sizeof(RRN_t), 1, file);
    fread(&header->RRNproxNo, sizeof(RRN_t), 1, file);
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

    if (cache->header->RRNproxNo > 0) {
        cache->nodes = calloc(cache->header->RRNproxNo, sizeof(BNode_t*));
    } else {
        cache->nodes = NULL;
    }
    return cache;
}

BNode_t* BTreeCache_GetNode(BTreeCache_t* cache, RRN_t nodeRRN) {
    if (nodeRRN < 0) { // Checks for NULL RRNs.
        return NULL;
    }

    assert(nodeRRN != 0); // "The RRN of NODES must be greater than zero."

    // Avoids unnecessary freads
    if (cache->nodes[nodeRRN-1] != NULL) {
        return cache->nodes[nodeRRN-1];
    }

    FILEPTR_t byteOffsetOfNode = RRNToFilePtr(nodeRRN);
    fseek(cache->bTreeFile, byteOffsetOfNode, SEEK_SET);
    
    // Creates a NULL node
    BNode_t* node = BNode_CreateNoChildren(-1, -1, -1);

    // Reads fields
    fread(&node->folha, sizeof(char), 1, cache->bTreeFile);
    fread(&node->nroChavesIndexadas, sizeof(int32_t), 1, cache->bTreeFile);
    fread(&node->RRNdoNo, sizeof(RRN_t), 1, cache->bTreeFile);
    fread(&node->P1, sizeof(RRN_t), 1, cache->bTreeFile);
    fread(&node->C1, sizeof(REGKEY_t), 1, cache->bTreeFile);
    fread(&node->PR1, sizeof(FILEPTR_t), 1, cache->bTreeFile);
    fread(&node->P2, sizeof(RRN_t), 1, cache->bTreeFile);
    fread(&node->C2, sizeof(REGKEY_t), 1, cache->bTreeFile);
    fread(&node->PR2, sizeof(FILEPTR_t), 1, cache->bTreeFile);
    fread(&node->P3, sizeof(RRN_t), 1, cache->bTreeFile);
    fread(&node->C3, sizeof(REGKEY_t), 1, cache->bTreeFile);
    fread(&node->PR3, sizeof(FILEPTR_t), 1, cache->bTreeFile);
    fread(&node->P4, sizeof(RRN_t), 1, cache->bTreeFile);
    fread(&node->C4, sizeof(REGKEY_t), 1, cache->bTreeFile);
    fread(&node->PR4, sizeof(FILEPTR_t), 1, cache->bTreeFile);
    fread(&node->P5, sizeof(RRN_t), 1, cache->bTreeFile);

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
BNode_t* GetNodeByKeyRecur(BTreeCache_t* cache, RRN_t nodeRRN, REGKEY_t key) {
    BNode_t* current = BTreeCache_GetNode(cache, nodeRRN);

    if (current == NULL) {
        return NULL;
    }

    if (current->nroChavesIndexadas < (BTREE_ORDER+1)/2-1 || current->nroChavesIndexadas > BTREE_ORDER-1) {
        printf("Arvore-B corrompida. Um ou mais nó possuem nroChavesIndexadas invalido: %d.", current->nroChavesIndexadas);
        exit(1);
    }

    // Performs a simple binary search.
    // Since the project specifications requests for the fields to not be in array format,
    // the only way to do the binary search is to go through each possible case.
    if (key == current->C1 || key == current->C2 || key == current->C3 || key == current->C4) return current;
    else if (key < current->C1) return GetNodeByKeyRecur(cache, current->P1, key);
    else if (key < current->C2) return GetNodeByKeyRecur(cache, current->P2, key);
    else if (key < current->C3) return GetNodeByKeyRecur(cache, current->P3, key);
    else if (key < current->C4) return GetNodeByKeyRecur(cache, current->P4, key);
    else return GetNodeByKeyRecur(cache, current->P5, key);
}

BNode_t* BTreeCache_GetNodeByKey(BTreeCache_t* cache, REGKEY_t key) {
    return GetNodeByKeyRecur(cache, cache->root->RRNdoNo, key);
}

void BTreeCache_Free(BTreeCache_t* bTreeCache) {
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