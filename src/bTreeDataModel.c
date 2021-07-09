#include "bTreeDataModel.h"
#include "bTree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

BHeader_t* BHeader_Create(char status, RRN_t rootNode, RRN_t rrnNextNode) {
    // Creates a new header and returns it
    BHeader_t* header = (BHeader_t*) malloc(sizeof(BHeader_t));

    header->status = status;
    header->rootRRN = rootNode;
    header->rrnNextNode = rrnNextNode;
    
    memset(header->unused, '@', sizeof(header->unused));
    
    return header;
}

void BHeader_Free(BHeader_t* header) {
    free(header);
}

BNode_t* BNode_Create(char isLeaf, int32_t indexedKeysCount, RRN_t rrn, ByteOffset_t* regOffsets, RegKey_t* regKeys, RRN_t* childrenRRNs) {
    // Creates a new node and returns it
    BNode_t* node = (BNode_t*) calloc(1, sizeof(BNode_t));
    node->isLeaf = isLeaf;
    node->indexedKeysCount = indexedKeysCount;
    node->rrn = rrn;

    for (int i = 0; i < BTREE_ORDER-1; i++) {
        node->childrenRRNs[i] = childrenRRNs[i];
        node->keys[i] = regKeys[i];
        node->offsets[i] = regOffsets[i];
    }
    node->childrenRRNs[BTREE_ORDER-1] = childrenRRNs[BTREE_ORDER-1];

    return node;
}

BNode_t* BNode_CreateWithRRN(BTreeMetadata_t* meta, char isLeaf) {
    BNode_t* node = BNode_CreateNull();
    node->isLeaf = isLeaf;
    node->rrn = meta->header->rrnNextNode;
    meta->header->rrnNextNode += 1;
    return node;
}

BNode_t* BNode_CreateNull() {
    ByteOffset_t regOffsets[BTREE_ORDER-1] = { -1, -1, -1, -1 };
    RegKey_t regKeys[BTREE_ORDER-1] = { -1, -1, -1, -1 };
    RRN_t childrenRRNs[BTREE_ORDER] = { -1, -1, -1, -1, -1 };
    return BNode_Create(-1, 0, -1, &regOffsets[0], &regKeys[0], &childrenRRNs[0]);
}

int BNode_GetKeyIndex(BNode_t* node, RegKey_t key) {
    if (node == NULL) return -1;

    for (int i = 0; i < node->indexedKeysCount; ++i) {
        if (node->keys[i] == key) {
            return i;
        }
    }
    return -1;
}

void BNode_Free(BNode_t* node) {
    if (node != NULL)   free(node);
}

ByteOffset_t RRNToOffset(RRN_t rrn) {
    return (1 + (ByteOffset_t) rrn) * ((ByteOffset_t) BTREE_RECORD_SIZE);
}
