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
 * @param bTreeIndexFileName The btree FileName
 * @param indexOpenType The open mode of the btree file
 * @param registersFileName The registers filename
 * @param registerOpenType The open mode of the btree file
 * @return BTreeMetadata_t* the created meta
 */
BTreeMetadata_t* BTreeMetadata_Create(const char* bTreeIndexFileName, const char* indexOpenType, const char* registersFileName, const char* registerOpenType);

/**
 * @brief Inserts a new register in the cache.
 * 
 * @param meta metadata of the btree
 * @param key the key of the new register
 * @param regOffset the rrn of the new register
 */
void BTreeMetadata_Insert(BTreeMetadata_t* meta, RegKey_t key, ByteOffset_t regOffset);

/**
 * @brief Gets a B-Tree node by key. Returns NULL if the node is not found.
 * 
 * @param meta The metadata containing the file pointer to the B-Tree.
 * @param key The key to find.
 * @return BNode_t* with the key or NULL if not found
 */
BNode_t* BTreeMetadata_GetNodeByKey(BTreeMetadata_t* meta, RegKey_t key);

/**
 * @brief Frees the heap memory allocated for the given B-Tree Cache.
 * 
 * @param meta The metadata containing the file pointer to the B-Tree.
 */
void BTreeMetadata_Free(BTreeMetadata_t* meta);

#endif