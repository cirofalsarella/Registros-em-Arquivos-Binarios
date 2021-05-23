#include "printer.h"

/**
 * @brief Prints an integer with the given label. Also handles the case when the field is NULL.
 * 
 * @param label 
 * @param value 
 */
void prettyPrintInt(const char* label, int value) {
    printf("%s: ", label);
    
    if (value == 0) {
        printf("campo com valor nulo\n");
    } else {
        printf("%d\n", value);
    }
}

/**
 * @brief Prints a string with the given label. Also handles the case when the field is NULL.
 * 
 * @param label 
 * @param str 
 */
void prettyPrint(const char* label, const char* str) {
    printf("%s: ", label);
    
    if (str == NULL) {
        printf("campo com valor nulo\n");
    } else {
        printf("%s\n", str);
    }
}

/**
 * @brief Prints a string that may not have a null terminator. For this to work, a maxLength needs to be specified.
 *        Also handles the case when str is NULL.
 * 
 * @param label 
 * @param str 
 * @param maxLength 
 */
void prettyPrintWithMaxLength(const char* label, const char* str, int maxLength) {
    printf("%s: ", label);
    
    if (str == NULL) {
        printf("campo com valor nulo\n");
    } else {
        // Prints each char from the string, and ensures we don't print unallocated memory
        int strLen = strlen(str);
        for (int i = 0; i < (strLen < maxLength ? strLen : maxLength); ++i) {
            printf("%c", str[i]);
        }
        printf("\n");
    }
}


void Printer_Vehicle(Vehicle *vehicle) {
    prettyPrintWithMaxLength("Prefixo do veiculo", vehicle->prefix, 5);
    prettyPrint("Modelo do veiculo", vehicle->model);
    prettyPrint("Categoria do veiculo", vehicle->category);
    prettyPrintWithMaxLength("Data de entrada do veiculo na frota", vehicle->date, 10);
    prettyPrintInt("Quantidade de lugares sentados disponiveis", vehicle->numSeats);
    printf("\n");
}

void Printer_BusLine(BusLine *busLine) {
    prettyPrintWithMaxLength("Codigo da linha", busLine->lineCode, 4);
    prettyPrint("Nome da linha", busLine->name);
    prettyPrint("Cor que descreve a linha", busLine->color);

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

    prettyPrint("Aceita cartao", acceptsCardStr);

    if (acceptsCardStr != NULL) free(acceptsCardStr);
    printf("\n");
}
