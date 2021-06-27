#ifndef _B_TREE_H_
#define _B_TREE_H_

#include <stdint.h>

// Relative record number (RRN) type
typedef int32_t RRN_t;

// Register key type
typedef int32_t REGKEY_t;

// Pointer to a register in the binary file type (offset in bytes)
typedef int64_t FILEPTR_t;

#define BTREE_ORDER 5 // Maximum number of children
#define BTREE_PAGE_SIZE 77
#define BTREE_RECORD_SIZE BTREE_PAGE_SIZE

/**
 * @brief The header of our B-Tree.
 */
struct BHeader {
    char status; // '0' or '1'
    RRN_t noRaiz; // RRN of the root node
    RRN_t RRNproxNo; // RRN of the next node to be inserted
    char lixo[68];
};

/**
 * @brief A B-Tree node.
 * 
 * P = pointers to subtrees
 * PR = pointer to with our registers
 */
struct BNode {
    char      folha; // FALSE or TRUE
    int32_t   nroChavesIndexadas; // Number of register keys stored in this node
    RRN_t     RRNdoNo;
    RRN_t     P1;  // RRN of a child node
    REGKEY_t  C1;  // Register key
    FILEPTR_t PR1; // Pointer to the registers file
    RRN_t     P2;  // RRN of a child node
    REGKEY_t  C2;  // Register key
    FILEPTR_t PR2; // Pointer to the registers file
    RRN_t     P3;  // RRN of a child node
    REGKEY_t  C3;  // Register key
    FILEPTR_t PR3; // Pointer to the file
    RRN_t     P4;  // RRN of a child node
    REGKEY_t  C4;  // Register key
    FILEPTR_t PR4; // Pointer to the registers file
    REGKEY_t  P5;  // RRN of a child node
};

typedef struct BHeader BHeader_t;
typedef struct BNode BNode_t;

/**
 * @brief Creates a new BTreeIndex Header and returns it.
 * 
 * @param status 
 * @param noRaiz 
 * @param RRNproxNo 
 * @return BHeader_t* 
 */
BHeader_t* BHeader_Create(char status, RRN_t noRaiz, RRN_t RRNproxNo);

/**
 * @brief Frees the given B-Tree header.
 * 
 * @param header The header to free.
 */
void BHeader_Free(BHeader_t* header);

/**
 * @brief Creates a new BTreeIndex Node and returns it.
 * 
 * @param folha 
 * @param nroChavesIndexadas 
 * @param RRNdoNo 
 * @param P1 
 * @param C1 
 * @param PR1 
 * @param P2 
 * @param C2 
 * @param PR2 
 * @param P3 
 * @param C3 
 * @param PR3 
 * @param P4 
 * @param C4 
 * @param PR4 
 * @param P5 
 * @return BNode_t* 
 */
BNode_t* BNode_Create(char folha, int32_t nroChavesIndexadas, RRN_t RRNdoNo,
                                  RRN_t P1, REGKEY_t C1, FILEPTR_t PR1,
                                  RRN_t P2, REGKEY_t C2, FILEPTR_t PR2,
                                  RRN_t P3, REGKEY_t C3, FILEPTR_t PR3,
                                  RRN_t P4, REGKEY_t C4, FILEPTR_t PR4,
                                  RRN_t P5);


/**
 * @brief Creates a B-Tree Node with no children (correctly initializes pointer and keys to -1).
 * 
 * @param folha 
 * @param nroChavesIndexadas 
 * @param RRNdoNo 
 * @return BNode_t* 
 */
BNode_t* BNode_CreateNoChildren(char folha, RRN_t RRNdoNo);

/**
 * @brief Frees the given B-Tree node.
 * 
 * @param node The node to free.
 */
void BNode_Free(BNode_t* node);

/**
 * @brief Helper function that converts an RRN to a file pointer.
 * 
 * @param rrn 
 * @return FILEPTR_t 
 */
FILEPTR_t RRNToFilePtr(RRN_t rrn);

#endif