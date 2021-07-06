#ifndef _BTREE_H_
#define _BTREE_H_

#include <stdio.h>
#include <stdlib.h>
#include "bTreeDataModel.h"

typedef struct BTreeCache BTreeCache_t;

/**
 * @brief Cached B-Tree nodes currently in RAM.
 */
struct BTreeCache {
    FILE* index; // File with the nodes from the B-Tree.
    FILE* registers; // File with the registers from the B-Tree.
    BHeader_t* header;
    BNode_t* root;
    BNode_t** nodes;
};

/**
 * @brief 
 * 
 * @param bTreeIndexFileName 
 * @param indexOpenType 
 * @param registersFileName 
 * @param registerOpenType 
 * @return BTreeCache_t* 
 */
BTreeCache_t* BTreeCache_Create(char* bTreeIndexFileName, char* indexOpenType, char* registersFileName, char* registerOpenType);

/**
 * @brief Creates a B-Tree cache from the given files and returns it.
 * 
 * @param bTreeIndexFileName File containing the B-Tree index. 
 * @param registersFileName File containing the registers.
 * @return BTreeCache_t* 
 */
BTreeCache_t* BTreeCache_CreateFromFile(char* bTreeIndexFileName, char* indexOpenType, char* registersFileName, char* registerOpenType);

/**
 * @brief Inserts a new register in the cache.
 * 
 * @param header header to create new nodes
 * @param cache cache to acess the nodes in ram
 * @param key the key of the new register
 * @param regOffset the rrn of the new register
 */
void BTreeCache_Insert(BTreeCache_t* cache, RegKey_t key, ByteOffset_t regOffset);

/**
 * @brief Frees the heap memory allocated for the given B-Tree Cache.
 * 
 * @param bTreeCache 
 */
void BTreeCache_Free(BTreeCache_t* bTreeCache);

#endif