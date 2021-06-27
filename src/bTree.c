#include "bTree.h"
#include "bTreeCache.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/*  TODO:
 *      (inserção)
 *      Escrita em disco (nós e header)
 *      Inserir o valor no arquivo 
 *      Promoção do nó 
 */

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
FILEPTR_t SearchNextNode(BNode_t* node, REGKEY_t key) {
    if (node->folha)    return -1;  // Não existe próximo nó

    for (int i=0; i<node->nroChavesIndexadas; i++) {
        if (node->C[key] < key) return i;
    }
    return node->nroChavesIndexadas; 
}

void InsertOnNode(BNode_t* node, REGKEY_t chave, FILEPTR_t PR, RRN_t ant, RRN_t prox){
    assert(node->nroChavesIndexadas < BTREE_ORDER-1);

    // Encontro a posição que vou inserir
    int pos = 0;
    while (chave > node->C[pos] && node->C[pos] != -1) {
        pos++;
    }

    // Movo os registros 1 pra frente
    for (int i=node->nroChavesIndexadas; i>pos; i++) {
        node->C[i] = node->C[i-1];
        node->PR[i] = node->PR[i-1];
        node->P[i] = node->P[i];
    }

    // Salvo o conteúdo da nova inserção
    node->C[pos] = chave;
    node->PR[pos] = PR;

    node->P[pos] = ant;
    node->P[pos+1] = prox;
    
    node->nroChavesIndexadas++;
}

void RegTradeNode(BNode_t* origem, BNode_t* dest, int pos) {

    // Salvando os valores do nó de origem
    RRN_t ant = origem->P[pos];
    origem->P[pos] = -1;
    RRN_t prox = origem->P[pos+1];
    origem->P[pos+1] = -1;

    REGKEY_t chave = origem->C[pos];
    origem->C[pos] = -1;

    FILEPTR_t PR = origem->PR[pos];
    origem->PR[pos] = -1;


    // Atualizando as posições
    origem->nroChavesIndexadas--;
    for (int i=pos; i<origem->nroChavesIndexadas; i++) {
        origem->P[i] = origem->P[i+1];
        origem->C[i] = origem->C[i+1];
        origem->PR[i] = origem->PR[i+1];
    }
    origem->P[origem->nroChavesIndexadas] = origem->P[origem->nroChavesIndexadas+1];


    // Inserindo no nó de destino
    InsertOnNode(dest, chave, PR, ant, prox);
}

void* Insert_Rec(BHeader_t* header, BTreeCache_t* cache, BNode_t* node, REGKEY_t chave, FILEPTR_t PR) {
    if (node->folha) {     // Insere no próprio nó
        if (node->nroChavesIndexadas == (BTREE_ORDER - 1)) {    // Nó Cheio
            // Criando o nó de partição
            BNode_t* particionado = BNode_CreateNoChildren(1, header->RRNproxNo);
            header->RRNproxNo += BTREE_RECORD_SIZE;

            RegTradeNode(node, particionado, BTREE_ORDER-1);    // Coloca o último no novo nó
            InsertOnNode(node, chave, PR, -1, -1);              // Coloca o novo registro no nó antigo
            RegTradeNode(node, particionado, BTREE_ORDER-1);

            // TODO:    Inserir o registro promovido no nó pai
            // TODO:    Escrever nó particionado no disco
        } 
        else    // Tem espaço
            InsertOnNode(node, chave, PR, -1, -1);
    }
    else    // Insere em um nó filho    
        Insert_Rec(header, cache, BTreeCache_GetNode(cache, SearchNextNode(node, chave)), chave, PR);

    // TODO:    escrever nó no disco

    return NULL;
}

void BTree_Insert(BTreeCache_t* cache, BHeader_t* header, REGKEY_t chave, FILEPTR_t PR) {
    Insert_Rec(header, cache, BTreeCache_GetNode(cache, header->noRaiz), chave, PR);
    // TODO:    Escrever header no disco
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
