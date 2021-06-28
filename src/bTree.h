#ifndef _B_TREE_H_
#define _B_TREE_H_

#include <stdint.h>

// Relative record number (RRN) type
typedef int32_t RRN;
typedef int32_t REGKEY;
typedef int64_t OFFSET;

#define BTREE_ORDER 5
#define BTREE_PAGE_SIZE 77
#define BTREE_RECORD_SIZE BTREE_PAGE_SIZE


typedef struct BHeader {
    char lixo[68];
    char status;        // '0' or '1'
    RRN noRaiz;       // RRN of the root node
    RRN RRNproxNo;    // RRN of the next node to be inserted
} BHeader_t;

/**
 * @brief A B-Tree node.
 * 
 * folha = indica se o nó é uma folha (T/F)
 * RRNdoNo = RRN do nó no arquivo de índice
 * nroChavesIndexadas = número de chaves presentes no nó
 * 
 * P = RRN de outros nós
 * PR = Offset do registro no arquivo principal
 * C = Chave do registro
 */
typedef struct BNode {
    char folha;
    RRN RRNdoNo;
    int32_t nroChavesIndexadas;

    OFFSET PR[BTREE_ORDER -1];
    REGKEY C[BTREE_ORDER -1];
    RRN P[BTREE_ORDER];
} BNode_t;


/**
 * @brief Creates a new BTreeIndex Header and returns it.
 * 
 * @param status 
 * @param noRaiz 
 * @param RRNproxNo 
 * @return BHeader_t* 
 */
BHeader_t* BHeader_Create(char status, RRN noRaiz, RRN RRNproxNo);

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
BNode_t* BNode_Create(char folha, int32_t nroChavesIndexadas, RRN RRNdoNo, OFFSET* PR, REGKEY* C, RRN* P);


/**
 * @brief Creates a B-Tree Node with no children (correctly initializes pointer and keys to -1).
 * @param folha indicates if the node is an leaf
 * @param RRNdoNo indicates the rrn of the node
 * @return an empty Node
 */
BNode_t* BNode_CreateNoChildren(char folha, RRN RRNdoNo);

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