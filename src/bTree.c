#include "bTree.h"
#include "bTreeCache.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/*  TODO:
 *      escrita no arquivo indice
 */

BHeader_t* BHeader_Create(char status, RRN rootNode, RRN rrnNextNode) {
    BHeader_t* header = (BHeader_t*) malloc(sizeof(BHeader_t));
    header->status = status;
    header->rootRRN = rootNode;
    header->rrnNextNode = rrnNextNode;
    memset(header->unused, 68, sizeof(header->unused));
    
    return header;
}

void BHeader_Free(BHeader_t* header) {
    free(header);
}

BNode_t* BNode_Create(char isLeaf, int32_t indexedKeysCount, RRN* rrn, OFFSET* regOffsets, REGKEY* regKeys, RRN* childrenRRNs) {
    BNode_t* node = (BNode_t*) malloc(sizeof(BNode_t));
    node->isLeaf = isLeaf;
    node->indexedKeysCount = indexedKeysCount;
    node->rrn = *rrn;
    (*rrn)++;

    for (int i=0; i<BTREE_ORDER-1; i++) {
        node->childrenRRNs[i] = childrenRRNs[i];
        node->regKeys[i] = regKeys[i];
        node->regOffsets[i] = regOffsets[i];
    }
    node->childrenRRNs[BTREE_ORDER-1] = childrenRRNs[BTREE_ORDER-1];

    return node;
}

BNode_t* BNode_CreateNoChildren(char isLeaf, RRN* rrn) {
    OFFSET regOffsets[BTREE_ORDER-1] = { -1 };
    REGKEY regKeys[BTREE_ORDER-1] = { -1 };
    RRN childrenRRNs[BTREE_ORDER] = { -1 };
    
    return BNode_Create(isLeaf, 0, rrn, &regOffsets[0], &regKeys[0], &childrenRRNs[0]);
}

void BNode_Free(BNode_t* node) {
    free(node);
}

OFFSET RRNToOffset(RRN rrn) {
    return ((OFFSET) rrn) * ((OFFSET) BTREE_RECORD_SIZE);
}
