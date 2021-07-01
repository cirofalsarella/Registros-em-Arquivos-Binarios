#include "bTree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

BHeader_t* BHeader_Create(char status, RRN_t rootNode, RRN_t rrnNextNode) {
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

BNode_t* BNode_Create(char isLeaf, int32_t indexedKeysCount, RRN_t* rrn, ByteOffset_t* regOffsets, RegKey_t* regKeys, RRN_t* childrenRRNs) {
    BNode_t* node = (BNode_t*) calloc(1, sizeof(BNode_t));
    node->isLeaf = isLeaf;
    node->indexedKeysCount = indexedKeysCount;
    if (rrn != NULL){
        node->rrn = *rrn;
        (*rrn)++;
    } else {
        node->rrn = -1;
    }

    for (int i = 0; i < BTREE_ORDER-1; i++) {
        node->childrenRRNs[i] = childrenRRNs[i];
        node->regKeys[i] = regKeys[i];
        node->regOffsets[i] = regOffsets[i];
    }
    node->childrenRRNs[BTREE_ORDER-1] = childrenRRNs[BTREE_ORDER-1];

    return node;
}

BNode_t* BNode_CreateNoChildren(char isLeaf, RRN_t* rrn) {
    ByteOffset_t regOffsets[BTREE_ORDER-1] = { -1 };
    RegKey_t regKeys[BTREE_ORDER-1] = { -1 };
    RRN_t childrenRRNs[BTREE_ORDER] = { -1 };
    
    return BNode_Create(isLeaf, 0, rrn, &regOffsets[0], &regKeys[0], &childrenRRNs[0]);
}

void BNode_Free(BNode_t* node) {
    free(node);
}

ByteOffset_t RRNToOffset(RRN_t rrn) {
    return ((ByteOffset_t) rrn) * ((ByteOffset_t) BTREE_RECORD_SIZE);
}
