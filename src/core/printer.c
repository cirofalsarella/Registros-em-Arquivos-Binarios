#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../bTree/bTree.h"
#include "../bTree/bTreeDataModel.h"
#include "../dataModel/dataModel.h"
#include "../core/utils.h"

/**
 * @brief Prints the given integer value, while also checking for NULL values (-1).
 * 
 * @param label 
 * @param value 
 */
void PrettyPrintInt(const char* label, int value) {
    printf("%s: ", label);
    
    if (value == -1) {
        printf("campo com valor nulo\n");
    } else {
        printf("%d\n", value);
    }
}

/**
 * @brief Prints the given string value, while also checking for NULL values (NULO).
 * 
 * @param label 
 * @param value 
 */
void PrettyPrintStr(const char* label, const char* str) {
    printf("%s: ", label);
    
    if (str == NULL || strlen(str) == 0) {
        printf("campo com valor nulo\n");
    } else {
        printf("%s\n", str);
    }
}

/**
 * @brief Prints the given date.
 * 
 * @param label 
 * @param value 
 */
void PrettyPrintDate(const char *date) {
    if (date == NULL || strlen(date) == 0) {
        printf("campo com valor nulo\n");
    } else {
        char mes[32];
        switch (date[6]) {
            case '1':   strcpy(mes, ( (date[5] == '0') ? "janeiro" : "novembro"));
                break;
            case '2':   strcpy(mes, ( (date[5] == '0') ? "fevereiro" : "dezembro"));
                break;
            case '3':   strcpy(mes, "março");
                break;
            case '4':   strcpy(mes, "abril");
                break;
            case '5':   strcpy(mes, "maio");
                break;
            case '6':   strcpy(mes, "junho");
                break;
            case '7':   strcpy(mes, "julho");
                break;
            case '8':   strcpy(mes, "agosto");
                break;
            case '9':   strcpy(mes, "setembro");
                break;
            case '0':   strcpy(mes, "outubro");
                break;
        }
        printf("%c%c de %s de %c%c%c%c\n", date[8], date[9], mes, date[0], date[1], date[2], date[3]);
    }
}

/**
 * @brief Prints a fixed-length string.
 * 
 * @param label 
 * @param str 
 * @param maxLength 
 */
void PrettyPrintWithMaxLength(const char* label, const char* str, int maxLength) {
    printf("%s: ", label);
    
    if (str == NULL || strlen(str) == 0) {
        printf("campo com valor nulo\n");
    } else {
        // Prints each char from the string, and ensures we don't print unallocated memory
        Utils_StrPrintWithFixedLen(str, maxLength);
    }
}


//  Printam um registro seguindo as normas

void Printer_Vehicle(const Vehicle_t *vehicle) {
    PrettyPrintWithMaxLength("Prefixo do veiculo", vehicle->prefix, 5);
    PrettyPrintStr("Modelo do veiculo", vehicle->model);
    PrettyPrintStr("Categoria do veiculo", vehicle->category);
    printf("Data de entrada do veiculo na frota: ");
    PrettyPrintDate(vehicle->date);
    PrettyPrintInt("Quantidade de lugares sentados disponiveis", vehicle->numSeats);
}

void Printer_BusLine(const BusLine_t *busLine) {
    PrettyPrintInt("Codigo da linha", busLine->lineCode);
    PrettyPrintStr("Nome da linha", busLine->name);
    PrettyPrintStr("Cor que descreve a linha", busLine->color);

    // Converts accepts credit card char to a string
    char* acceptsCardStr = calloc(100, sizeof(char));
    
    if (busLine->acceptsCreditCard == 'S') {
        strcpy(acceptsCardStr, "PAGAMENTO SOMENTE COM CARTAO SEM PRESENCA DE COBRADOR");        
    } else if (busLine->acceptsCreditCard == 'N') {
        strcpy(acceptsCardStr, "PAGAMENTO EM CARTAO E DINHEIRO");
    } else if (busLine->acceptsCreditCard == 'F') {
        strcpy(acceptsCardStr, "PAGAMENTO EM CARTAO SOMENTE NO FINAL DE SEMANA");
    } else {
        free(acceptsCardStr);
        acceptsCardStr = NULL;
    }

    PrettyPrintStr("Aceita cartao", acceptsCardStr);

    if (acceptsCardStr != NULL) free(acceptsCardStr);
}

void Printer_Merge(const Vehicle_t *vehicle, const BusLine_t* busLine) {
    PrettyPrintWithMaxLength("Prefixo do veiculo", vehicle->prefix, 5);
    PrettyPrintStr("Modelo do veiculo", vehicle->model);
    PrettyPrintStr("Categoria do veiculo", vehicle->category);
    printf("Data de entrada do veiculo na frota: ");
    PrettyPrintDate(vehicle->date);
    PrettyPrintInt("Quantidade de lugares sentados disponiveis", vehicle->numSeats);

    PrettyPrintInt("Codigo da linha", busLine->lineCode);
    PrettyPrintStr("Nome da linha", busLine->name);
    PrettyPrintStr("Cor que descreve a linha", busLine->color);

    // Converts accepts credit card char to a string
    char* acceptsCardStr = calloc(100, sizeof(char));
    
    if (busLine->acceptsCreditCard == 'S') {
        strcpy(acceptsCardStr, "PAGAMENTO SOMENTE COM CARTAO SEM PRESENCA DE COBRADOR");        
    } else if (busLine->acceptsCreditCard == 'N') {
        strcpy(acceptsCardStr, "PAGAMENTO EM CARTAO E DINHEIRO");
    } else if (busLine->acceptsCreditCard == 'F') {
        strcpy(acceptsCardStr, "PAGAMENTO EM CARTAO SOMENTE NO FINAL DE SEMANA");
    } else {
        free(acceptsCardStr);
        acceptsCardStr = NULL;
    }

    PrettyPrintStr("Aceita cartao", acceptsCardStr);

    if (acceptsCardStr != NULL) free(acceptsCardStr);

    printf("\n");
}


/**
 * @brief Prints an node
 * @param rrn (use rrn-1)
 */
void Printer_Node(const BNode_t* node) {
    if (node == NULL) {
        printf("Printer_Node: NULL node.\n");
        return;
    }

    printf("%d %c -> ", node->rrn, (node->isLeaf ? 's' : 'n'));
    for (int i=0; i<4; i++) {
        printf("(%d) %d ", node->childrenRRNs[i], node->keys[i]);
    }
    printf("(%d)", node->childrenRRNs[4]);
    printf(" %d,", node->indexedKeysCount);

    printf("\n");
}
