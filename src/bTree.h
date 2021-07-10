#ifndef _BTREE_H_
#define _BTREE_H_

#include <stdio.h>
#include <stdlib.h>
#include "bTreeDataModel.h"

/**
 * @brief Data useful when dealing with a B-Tree in the disk.
 */
typedef struct BTreeMetadata {
    FILE* bTreeIndexFile; // File with the nodes from the B-Tree.
    FILE* registersFile; // File with the registers from the B-Tree.
    BHeader_t* header;
} BTreeMetadata_t;

/**
 * @brief Creates a new B-Tree metadata.
 * 
 * @param bTreeIndexFileName 
 * @param indexOpenType 
 * @param registersFileName 
 * @param registerOpenType 
 * @return BTreeCache_t* 
 */
BTreeMetadata_t* BTreeMetadata_Create(char* bTreeIndexFileName, char* indexOpenType, char* registersFileName, char* registerOpenType);

/**
 * @brief Inserts a new register in the cache.
 * 
 * @param header header to create new nodes
 * @param meta cache to acess the nodes in ram
 * @param key the key of the new register
 * @param regOffset the rrn of the new register
 */
void BTreeMetadata_Insert(BTreeMetadata_t* meta, RegKey_t key, ByteOffset_t regOffset);

/**
 * @brief Gets a B-Tree node by key. Returns NULL if the node is not found.
 * 
 * @param meta The metadata containing the file pointer to the B-Tree.
 * @param key The key to find.
 * @return BNode_t* 
 */
BNode_t* BTreeMetadata_GetNodeByKey(BTreeMetadata_t* meta, RegKey_t key);

/**
 * @brief Frees the heap memory allocated for the given B-Tree Cache.
 * 
 * @param meta 
 */
void BTreeMetadata_Free(BTreeMetadata_t* meta);

#endif