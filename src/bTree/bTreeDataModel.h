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

typedef struct BTreeMetadata BTreeMetadata_t; // Forward declaration

typedef struct BHeader {
    char status; // '0' or '1'. '1' when everything is OK. '0' otherwise.
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

    ByteOffset_t offsets[BTREE_ORDER -1];
    RegKey_t keys[BTREE_ORDER -1];
    RRN_t childrenRRNs[BTREE_ORDER];
} BNode_t;


/**
 * @brief 
 * 
 * @param status The file status
 * @param rootRRN The rrn of the root
 * @param nextNodeRRN The rrn of the next node to be created
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
 * @param metadata the metadata to get the RRN from
 * @param isLeaf indicates if the node is a leaf
 * @return an empty Node
 */
BNode_t* BNode_CreateWithRRN(BTreeMetadata_t* meta, char isLeaf);

/**
 * @brief Creates a B-Tree Node with its fields assigned to -1.
 * @return an empty Node
 */
BNode_t* BNode_CreateNull();

/**
 * @brief Frees the given B-Tree node.
 * @param node The node to free.
 */
void BNode_Free(BNode_t* node);

/**
 * @brief Returns the key index of the given key at the given node. If not found, returns -1.
 * 
 * @param node Node to use during search
 * @param key Key to find
 */
int BNode_GetKeyIndex(BNode_t* node, RegKey_t key);

/**
 * @brief Helper function that converts an RRN to a file Offset.
 * 
 * @param rrn to be used
 * @return the offset to that rrn
 */
ByteOffset_t RRNToOffset(RRN_t rrn);

#endif