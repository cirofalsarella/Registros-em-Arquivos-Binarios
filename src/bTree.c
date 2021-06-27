#include "bTree.h"
#include "bTreeCache.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// TODO:    Salvar o registro 
//          Att o valor de 


BHeader_t* BHeader_Create(char status, RRN_t noRaiz, RRN_t RRNproxNo) {
    BHeader_t* header = (BHeader_t*) malloc(sizeof(BHeader_t));
    header->status = status;
    header->noRaiz = noRaiz;
    header->RRNproxNo = RRNproxNo;
    for (int i = 0; i < 68; ++i) {
        header->lixo[i] = '@';
    }
    assert(sizeof(BHeader_t) == BTREE_PAGE_SIZE); // Simple check that ensures that the struct has the right size
    return header;
}


void BHeader_Free(BHeader_t* header) {
    free(header);
}


// retorna o pt pro próximo nó a ser inserido
// -1 indica que vai inserir no nó apontado por atual
FILEPTR_t BuscaProxRRN(BNode_t* node, REGKEY_t key) {
    if (node == NULL || node->nroChavesIndexadas < BTREE_ORDER)   return -1;

    if (key < node->C1) return node->P1;
    if (key < node->C2) return node->P2;
    if (key < node->C3) return node->P3;
    if (key < node->C4) return node->P4;
    return node->P5;
}


void BTree_Insert(BTreeCache_t* cache, BHeader_t* header, REGKEY_t chave, FILEPTR_t PR) {
    // Encontra o lugar para inserção
    BNode_t* ant = NULL;
    BNode_t* atual = BTreeCache_GetNode(cache, header->noRaiz);
    FILEPTR_t prox = BuscaProxRRN(atual, chave);
    while (prox != -1) {
        ant = atual;
        atual = BTreeCache_GetNode(cache, prox);
        prox = BuscaProxRRN(atual, chave);
    }

    int pos = 0;
    if (atual == NULL) {
        atual = BNode_CreateNoChildren(1, header->RRNproxNo);
        header->RRNproxNo += BTREE_RECORD_SIZE;
        // Atualiza RRNproxNo no arquivo
    } else {
        atual = ant;
        while (chave > atual.C[pos] && atual.C[pos] != -1) {
            pos++;
        }

        for (int i=atual->nroChavesIndexadas; i>pos; i++) {
            atual.C[i] = atual.c[i-1];
        }
    }

    atual->C[pos] = chave;
    atual->PR[pos] = PR;
    atual->nroChavesIndexadas++;

    // Escreve nó atual no arquivo
}



BNode_t* BNode_Create(char folha, int32_t nroChavesIndexadas, RRN_t RRNdoNo,
                                  RRN_t P1, REGKEY_t C1, FILEPTR_t PR1,
                                  RRN_t P2, REGKEY_t C2, FILEPTR_t PR2,
                                  RRN_t P3, REGKEY_t C3, FILEPTR_t PR3,
                                  RRN_t P4, REGKEY_t C4, FILEPTR_t PR4,
                                  RRN_t P5) {
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
    assert(sizeof(BNode_t) == BTREE_PAGE_SIZE); // Simple check that ensures that the struct has the right size
}

BNode_t* BNode_CreateNoChildren(char folha, RRN_t RRNdoNo) {
    return BNode_Create(folha, 0, RRNdoNo, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
}

void BNode_Free(BNode_t* node) {
    free(node);
}

FILEPTR_t RRNToFilePtr(RRN_t rrn) {
    return ((FILEPTR_t)rrn) * (FILEPTR_t)BTREE_RECORD_SIZE;
}
