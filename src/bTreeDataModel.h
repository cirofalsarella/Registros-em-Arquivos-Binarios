#ifndef _B_TREE_H_
#define _B_TREE_H_

#include <stdint.h>

// Constant types/values for cleaner & more future-proof code
typedef int32_t RRN_t;
typedef int32_t RegKey_t;
typedef int64_t ByteOffset_t;

#define BTREE_ORDER 5 // Maximum number of children
#define BTREE_REGKEY_COUNT (BTREE_ORDER-1)
#define BTREE_MAX_CHILD_COUNT BTREE_ORDER
#define BTREE_PAGE_SIZE 77
#define BTREE_RECORD_SIZE BTREE_PAGE_SIZE

typedef struct BHeader {
    char status; // '0' or '1'
    RRN_t rootRRN; // RRN of the root node
    RRN_t rrnNextNode; // RRN of the next node to be inserted
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
    RRN_t rrn;
    int32_t indexedKeysCount;

    ByteOffset_t regOffsets[BTREE_ORDER -1];
    RegKey_t regKeys[BTREE_ORDER -1];
    RRN_t childrenRRNs[BTREE_ORDER];
} BNode_t;


/**
 * @brief Creates a new BTreeIndex Header and returns it.
 * 
 * @param status 
 * @param noRaiz 
 * @param RRNproxNo 
 * @return BHeader_t* 
 */
BHeader_t* BHeader_Create(char status, RRN_t rootRRN, RRN_t nextNodeRRN);

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
BNode_t* BNode_Create(char isLeaf, int32_t indexedKeysCount, RRN_t rrn, ByteOffset_t* regOffsets, RegKey_t* regKeys, RRN_t* childrenRRNs);

/**
 * @brief Creates a B-Tree Node with no children (correctly initializes pointer and keys to -1).
 * @param isLeaf indicates if the node is an leaf
 * @param rrn indicates the rrn of the node
 * @return an empty Node
 */
BNode_t* BNode_CreateNoChildren(char isLeaf, RRN_t rrn);

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
ByteOffset_t RRNToOffset(RRN_t rrn);

#endif