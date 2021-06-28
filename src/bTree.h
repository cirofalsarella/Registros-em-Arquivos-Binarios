#ifndef _B_TREE_H_
#define _B_TREE_H_

#include <stdint.h>

// Constant types for better code control
typedef int32_t RRN;
typedef int32_t REGKEY;
typedef int64_t OFFSET;


// Constant values for better code control

#define BTREE_ORDER 5 // Maximum number of children
#define BTREE_REGKEY_COUNT (BTREE_ORDER-1)
#define BTREE_MAX_CHILD_COUNT BTREE_ORDER
#define BTREE_PAGE_SIZE 77
#define BTREE_RECORD_SIZE BTREE_PAGE_SIZE

typedef struct BHeader {
    char status; // '0' or '1'
    RRN rootRRN; // RRN of the root node
    RRN rrnNextNode; // RRN of the next node to be inserted
    char unused[68];
} BHeader_t;

/**
 * @brief A B-Tree node.
 * 
 * @param isLeaf Flag indicating whether this node is a leaf 
 * @param rrn This node's RRN
 * @param indexedKeysCount Number of indexed keys in node
 * 
 * @param childrenRRNs RRN of the children
 * @param regOffsets Byte offsets of the registers stored in this node
 * @param regKeys Keys of the registers stored in this node
 */
typedef struct BNode {
    char isLeaf;
    RRN rrn;
    int32_t indexedKeysCount;

    OFFSET regOffsets[BTREE_ORDER -1];
    REGKEY regKeys[BTREE_ORDER -1];
    RRN childrenRRNs[BTREE_ORDER];
} BNode_t;


/**
 * @brief Creates a new BTreeIndex Header and returns it.
 * 
 * @param status 
 * @param noRaiz 
 * @param RRNproxNo 
 * @return BHeader_t* 
 */
BHeader_t* BHeader_Create(char status, RRN rootRRN, RRN nextNodeRRN);

/**
 * @brief Frees the given B-Tree header.
 * 
 * @param header The header to free.
 */
void BHeader_Free(BHeader_t* header);

/**
 * @brief Creates a new BTreeIndex Node and returns it.
 * the parameters are the members of the struct BNode
 * @return the node created
 */
BNode_t* BNode_Create(char isLeaf, int32_t indexedKeysCount, RRN rrn, OFFSET* regOffsets, REGKEY* regKeys, RRN* childrenRRNs);

/**
 * @brief Creates a B-Tree Node with no children (correctly initializes pointer and keys to -1).
 * @param isLeaf indicates if the node is an leaf
 * @param rrn indicates the rrn of the node
 * @return an empty Node
 */
BNode_t* BNode_CreateNoChildren(char isLeaf, RRN rrn);

/**
 * @brief Frees the given B-Tree node.
 * @param node The node to free.
 */
void BNode_Free(BNode_t* node);

/**
 * @brief Helper function that converts an RRN to a file Offset.
 * 
 * @param rrn to be used
 * @return the offset to that rrn
 */
OFFSET RRNToOffset(RRN rrn);

#endif