#include "bTree.h"
#include "bTreeCache.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/*  TODO:
 *      (inserção)
 *      Escrita em disco (nós e header)
 *      Inserir o valor no arquivo 
 *      Promoção do nó 
 */

BHeader_t* BHeader_Create(char status, RRN noRaiz, RRN RRNproxNo) {
    BHeader_t* header = (BHeader_t*) malloc(sizeof(BHeader_t));
    header->status = status;
    header->noRaiz = noRaiz;
    header->RRNproxNo = RRNproxNo;
    memset(header, header->lixo, 68);
    
    return header;
}


void BHeader_Free(BHeader_t* header) {
    free(header);
}


// retorna o pt pro próximo nó a ser inserido
OFFSET SearchNextNode(BNode_t* node, REGKEY key) {
    if (node->folha)    return -1;  // Não existe próximo nó

    for (int i=0; i<node->nroChavesIndexadas; i++) {
        if (node->C[key] < key) return i;
    }
    return node->nroChavesIndexadas; 
}

void InsertOnNode(BNode_t* node, REGKEY chave, OFFSET PR, RRN ant, RRN prox){
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
    RRN ant = origem->P[pos];
    origem->P[pos] = -1;
    RRN prox = origem->P[pos+1];
    origem->P[pos+1] = -1;

    REGKEY chave = origem->C[pos];
    origem->C[pos] = -1;

    OFFSET PR = origem->PR[pos];
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

void* Insert_Rec(BHeader_t* header, BTreeCache_t* cache, BNode_t* node, REGKEY chave, OFFSET PR) {
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

void BTree_Insert(BTreeCache_t* cache, BHeader_t* header, REGKEY chave, OFFSET PR) {
    Insert_Rec(header, cache, BTreeCache_GetNode(cache, header->noRaiz), chave, PR);
    // TODO:    Escrever header no disco
}



BNode_t* BNode_Create(char folha, int32_t nroChavesIndexadas, RRN RRNdoNo, OFFSET* PR, REGKEY* C, RRN* P) {
    BNode_t* node = (BNode_t*) malloc(sizeof(BNode_t));
    node->folha = folha;
    node->nroChavesIndexadas = nroChavesIndexadas;
    node->RRNdoNo = RRNdoNo;

    for (int i=0; i<BTREE_ORDER-1; i++) {
        node->P[i] = P[i];
        node->C[i] = C[i];
        node->PR[i] = PR[i];
    }
    node->P[BTREE_ORDER-1] = P[BTREE_ORDER-1];

    return node;
}

BNode_t* BNode_CreateNoChildren(char folha, RRN RRNdoNo) {
    OFFSET PR[BTREE_ORDER-1];
    memset(PR, -1, sizeof(OFFSET)*(BTREE_ORDER-1));

    REGKEY C[BTREE_ORDER-1];
    memset(PR, -1, sizeof(REGKEY)*(BTREE_ORDER-1));
    
    RRN P[BTREE_ORDER];
    memset(PR, -1, sizeof(RRN)*(BTREE_ORDER));
    
    return BNode_Create(folha, 0, RRNdoNo, PR, C, P);
}

void BNode_Free(BNode_t* node) {
    free(node);
}

OFFSET RRNToOffset(RRN rrn) {
    return ((OFFSET) rrn) * ((OFFSET) BTREE_RECORD_SIZE);
}
