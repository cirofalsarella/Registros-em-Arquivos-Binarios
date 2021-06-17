#include "bTree.h"
#include <stdio.h>
#include <assert.h>

BHeader_t* BHeader_Create(char status, RRN_t noRaiz, RRN_t RRNproxNo) {
    BHeader_t* header = (BHeader_t*) malloc(sizeof(BHeader_t));
    header->status = status;
    header->noRaiz = noRaiz;
    header->RRNproxNo = RRNproxNo;
    for (int i = 0; i < 68; ++i) {
        header->lixo[i] = '@';
    }
    assert(sizeof(BHeader_t) == PAGE_SIZE); // Simple check that ensures that the struct has the right size
    return header;
}

void BHeader_Free(BHeader_t* header) {
    free(header);
}

BNode_t* BNode_Create(char folha, int32_t nroChavesIndexadas, RRN_t RRNdoNo,
                                  RRN_t P1, REGKEY_t C1, FILEPTR_t PR1,
                                  RRN_t P2, REGKEY_t C2, FILEPTR_t PR2,
                                  RRN_t P3, REGKEY_t C3, FILEPTR_t PR3,
                                  RRN_t P4, REGKEY_t C4, FILEPTR_t PR4,
                                  RRN_t P5);
    BNode_t* node = (BNode_t*) malloc(sizeof(BNode_t));
    node->folha = folha;
    node->nroChavesIndexadas = nroChavesIndexadas;
    node->RRNdoNo = RRNdoNo;
    node->P1 = P1;
    node->C1 = C1;
    node->PR1 = PR1;
    node->P2 = P2;
    node->C2 = C2;
    node->PR2 = PR2;
    node->P3 = P3;
    node->C3 = C3;
    node->PR3 = PR3;
    node->P4 = P4;
    node->C4 = C4;
    node->PR4 = PR4;
    node->P5 = P5;
    assert(sizeof(BNode_t) == PAGE_SIZE); // Simple check that ensures that the struct has the right size
}

BNode_t* BNode_CreateNoChildren(char folha, int32_t nroChavesIndexadas, RRN_t RRNdoNo) {
    return BNode_Create(folha, nroChavesIndexadas, RRNdoNo, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
}

void BNode_Free(BNode_t* node) {
    free(node);
}