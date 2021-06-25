#ifndef _BTREE_CACHE_H_
#define _BTREE_CACHE_H_

#include <stdio.h>
#include <stdlib.h>
#include "bTree.h"

// TODO: fopens & fcloses
// TODO: Ciro - para acessar uma node por RRN, faz cache->nodes[rrn-1]. Menos 1 pois o primeiro reg do arquivo é o header.

typedef struct BTreeCache BTreeCache_t;

/**
 * @brief Cached B-Tree nodes currently in RAM.
 */
struct BTreeCache {
    char* bTreeIndexFileName; // File with the nodes from the B-Tree.
    char* registersFileName; // File with the registers from the B-Tree.
    FILE* bTreeFile;
    FILE* registersFile;
    BHeader_t* header;
    BNode_t* root;
    BNode_t** nodes;
};

/**
 * @brief Creates a B-Tree Cache from the given files and returns it.
 * 
 * @param bTreeIndexFileName File containing the B-Tree index. 
 * @param registersFileName File containing the registers.
 * @return BTreeCache_t* 
 */
BTreeCache_t* BTreeCache_CreateFromFile(char* bTreeIndexFileName, char* registersFileName);

/**
 * @brief Gets a node from the B-Tree cache by RRN.
 *        If the node is not cached, loads from disk.
 *        If RRN is null (< 0), returns NULL.
 * 
 * @param cache
 * @param nodeRRN Must be -1 or a valid RRN.
 * 
 * @return BNode_t* 
 */
BNode_t* BTreeCache_GetNode(BTreeCache_t* cache, RRN_t nodeRRN);

/**
 * @brief Gets a B-Tree Node by key.
 * 
 * @param cache
 * @param key Key to use during the query.
 * 
 * @return BNode_t* 
 */
BNode_t* BTreeCache_GetNodeByKey(BTreeCache_t* cache, REGKEY_t key);

/**
 * @brief Marks status of the B-Tree index file as '0'. Status change for the registers file is done elsewhere. Call this as soon as the file is opened. NOTE: Changes the file pointer.
 * 
 * @param bTreeCache The cache.
 */
void BTreeCache_BeginWrite(BTreeCache_t* bTreeCache);

/**
 * @brief Marks status of the B-Tree index file as '1'. Status change for the registers file is done elsewhere. Call this immediately before fclosing the file. NOTE: Changes the file pointer.
 * 
 * @param bTreeCache 
 */
void BTreeCache_EndWrite(BTreeCache_t* bTreeCache);

/**
 * @brief Frees the heap memory allocated for the given B-Tree Cache.
 * 
 * @param bTreeCache 
 */
void BTreeCache_Free(BTreeCache_t* bTreeCache);

#endif