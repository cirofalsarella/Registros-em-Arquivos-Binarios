#include "bTree.h"
#include "bTreeCache.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/*  TODO:
 *      escrita no arquivo indice
 */

BHeader_t* BHeader_Create(char status, RRN noRaiz, RRN RRNproxNo) {
    BHeader_t* header = (BHeader_t*) malloc(sizeof(BHeader_t));
    header->status = status;
    header->rootRRN = noRaiz;
    header->rrnNextNode = RRNproxNo;
    memset(header, header->unused, 68);
    
    return header;
}

void BHeader_Free(BHeader_t* header) {
    free(header);
}



/**
 * @brief struct to help manipulate registers
 * @param chave the key of the register
 * @param PR the offset of the record in the main file
 * @param p_ant the RRN of the node before
 * @param p_prox the RRN of the node after
 */
typedef struct bRegister {
    REGKEY chave;
    OFFSET PR;
    RRN p_ant;
    RRN p_prox;
} BRegister_t;

/**
 * @brief Create and return an Register object
 */
BRegister_t* CreateRegister(REGKEY chave, OFFSET PR, RRN p_ant, RRN p_prox) {
    BRegister_t* reg = (BRegister_t*) malloc(sizeof(BRegister_t));
    reg->chave = chave;
    reg->PR = PR;
    reg->p_ant = p_ant;
    reg->p_prox = p_prox;
    return reg;
}



/**
 * @brief find the RRN of the next node on the search
 * 
 * @param node node with the pointers
 * @param key  key to be searched
 * @return the next node rrn
 */
RRN SearchNextNodeRRN(BNode_t* node, REGKEY key) {
    if (node->isLeaf)    return -1;  // Não existe próximo nó

    for (int i=0; i<node->indexedKeysCount; i++) {
        if (node->regKeys[key] < key) return node->regKeys[i];
    }
    return node->regKeys[node->indexedKeysCount]; 
}

/**
 * @brief Insert an register into the node with space
 * 
 * @param node
 * @param reg register to be inserted; its memory will be freed
 */
void InsertOnNode(BNode_t* node, BRegister_t* reg){
    assert(node->indexedKeysCount < BTREE_ORDER-1);

    // Encontro a posição que vou inserir
    int pos = 0;
    while (reg->chave > node->regKeys[pos] && node->regKeys[pos] != -1) {
        pos++;
    }

    // Movo os registros 1 pra frente
    for (int i=node->indexedKeysCount; i>pos; i++) {
        node->regKeys[i] = node->regKeys[i-1];
        node->regOffsets[i] = node->regOffsets[i-1];
        node->childrenRRNs[i] = node->childrenRRNs[i];
    }

    // Salvo o conteúdo da nova inserção
    node->regKeys[pos] = reg->chave;
    node->regOffsets[pos] = reg->PR;
    node->childrenRRNs[pos] = reg->p_ant;
    node->childrenRRNs[pos+1] = reg->p_prox;
    free(reg);

    node->indexedKeysCount++;
}

/**
 * @brief Moves an register from an node to the other with space
 * 
 * @param origem The node that will provide the register
 * @param dest The node that will receive the register
 * @param pos The position of the register in the origin node
 */
void RegTradeNode(BNode_t* origem, BNode_t* dest, int pos) {
    assert(dest->indexedKeysCount != BTREE_ORDER-1);

    // Salvando os valores do nó de origem
    BRegister_t* newReg = CreateRegister(origem->regKeys[pos], origem->regOffsets[pos], origem->childrenRRNs[pos], origem->childrenRRNs[pos+1]);

    // Atualizando o nó de origem
    origem->indexedKeysCount--;
    for (int i=pos; i<origem->indexedKeysCount; i++) {
        origem->childrenRRNs[i] = origem->childrenRRNs[i+1];
        origem->regKeys[i] = origem->regKeys[i+1];
        origem->regOffsets[i] = origem->regOffsets[i+1];
    }
    origem->childrenRRNs[origem->indexedKeysCount] = origem->childrenRRNs[origem->indexedKeysCount+1];

    origem->childrenRRNs[origem->indexedKeysCount] = -1;
    origem->regKeys[origem->indexedKeysCount] = -1;
    origem->regOffsets[origem->indexedKeysCount] = -1;
    origem->childrenRRNs[origem->indexedKeysCount+1] = -1;


    // Inserindo no nó de destino
    InsertOnNode(dest, newReg);
}

/**
 * @brief Partitions an node into two
 * 
 * @param header the header of the btree (used to create the new node)
 * @param node the node that is fullfilled
 * @param newReg the register to be inserted in the node
 * @return the new node created
 */
BRegister_t* PartitionsNode(BHeader_t* header, BNode_t* node, BRegister_t* newReg){
    // Criando o nó de partição
    BNode_t* particionado = BNode_CreateNoChildren(1, header->rrnNextNode);
    header->rrnNextNode++;

    // Pega os registros e muda os 2 maiores para o novo nó partição
    RegTradeNode(node, particionado, BTREE_ORDER-1);
    InsertOnNode(node, newReg);
    RegTradeNode(node, particionado, BTREE_ORDER-1);

    
    // Remover o 1o registro do nó particionado
    BRegister_t* promovido = CreateRegister(particionado->regKeys[0], particionado->regOffsets[0], node->rrn, particionado->rrn);
    
    particionado->regKeys[0] = particionado->regKeys[1];
    particionado->regKeys[1] = -1;

    particionado->regOffsets[0] = particionado->regOffsets[1];
    particionado->regOffsets[1] = -1;
    
    particionado->childrenRRNs[0] = particionado->childrenRRNs[1];
    particionado->childrenRRNs[1] = particionado->childrenRRNs[2];
    particionado->childrenRRNs[2] = -1;


    // TODO:    Escrever nó particionado no disco

    return promovido;
}

/**
 * @brief Insert recursieve the register in the tree, takes care of partitions and promotions
 * 
 * @param header the header of the tree
 * @param cache the cache to acess the nodes in ram
 * @param node the current node of the insertion
 * @param newReg the new register to be included
 * @return in case of promotion returns the promoted reg, otherwise returns NULL
 */
BRegister_t* Insert_Rec(BHeader_t* header, BTreeCache_t* cache, BNode_t* node, BRegister_t* newReg) {
    if (node->isLeaf) {      // Insere no próprio nó
        if (node->indexedKeysCount == (BTREE_ORDER - 1))
            return PartitionsNode(header, node, newReg);
        // Se o nó estiver cheio vai particionar e promover um nó

        else    InsertOnNode(node, newReg);
        // Caso tenha espaço no nó será realizada uma inserção simples
    }
    else {                  // Insere em um nó filho   
        BRegister_t* promovido = Insert_Rec(header, cache, BTreeCache_GetNode(cache, SearchNextNode(node, newReg->chave)), newReg);

        // confere se um dos nós do filho foi promovido
        if (promovido != NULL) {
            if (node->indexedKeysCount == (BTREE_ORDER - 1))
                return PartitionsNode(node, promovido, newReg);
            // Se o nó estiver cheio vai particionar e promover um nó

            else    InsertOnNode(node, newReg);
            // Caso tenha espaço no nó será realizada uma inserção simples
        }
    }

    // TODO:    escrever nó no disco

    return NULL;
}


void BTree_Insert(BHeader_t* header, BTreeCache_t* cache, REGKEY chave, OFFSET PR) {
    BRegister_t* newReg = CreateRegister(chave, PR, -1, -1);
    BRegister_t* promovido = Insert_Rec(header, cache, BTreeCache_GetNode(cache, header->rootRRN), newReg);
    
    if (promovido != NULL) {
        BNode_t* root = BNode_CreateNoChildren(0, header->rrnNextNode);
        header->rrnNextNode++;
        header->rootRRN = root->rrn;
        InsertOnNode(root, promovido);

        // TODO:    Escrever nó promovido no arquivo de indice
    }
    // TODO:    Escrever header no arquivo de índice
}



BNode_t* BNode_Create(char folha, int32_t nroChavesIndexadas, RRN RRNdoNo, OFFSET* PR, REGKEY* C, RRN* P) {
    BNode_t* node = (BNode_t*) malloc(sizeof(BNode_t));
    node->isLeaf = folha;
    node->indexedKeysCount = nroChavesIndexadas;
    node->rrn = RRNdoNo;

    for (int i=0; i<BTREE_ORDER-1; i++) {
        node->childrenRRNs[i] = P[i];
        node->regKeys[i] = C[i];
        node->regOffsets[i] = PR[i];
    }
    node->childrenRRNs[BTREE_ORDER-1] = P[BTREE_ORDER-1];

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
