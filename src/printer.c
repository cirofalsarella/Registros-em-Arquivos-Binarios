#include "printer.h"
#include "utils.h"

/**
 * @brief Prints an integer with the given label. Also handles the case when the field is NULL.
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
 * @brief Prints a string with the given label. Also handles the case when the field is NULL.
 * 
 * @param label 
 * @param str 
 */
void PrettyPrint(const char* label, const char* str) {
    printf("%s: ", label);
    
    if (str == NULL || strlen(str) == 0) {
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
void PrettyPrintWithMaxLength(const char* label, const char* str, int maxLength) {
    printf("%s: ", label);
    
    if (str == NULL || strlen(str) == 0) {
        printf("campo com valor nulo\n");
    } else {
        // Prints each char from the string, and ensures we don't print unallocated memory
        Utils_StrPrintWithFixedLen(str, maxLength);
    }
}

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


void Printer_Vehicle(Vehicle *vehicle) {
    PrettyPrintWithMaxLength("Prefixo do veiculo", vehicle->prefix, 5);
    PrettyPrint("Modelo do veiculo", vehicle->model);
    PrettyPrint("Categoria do veiculo", vehicle->category);
    printf("Data de entrada do veiculo na frota: ");
    PrettyPrintDate(vehicle->date);
    PrettyPrintInt("Quantidade de lugares sentados disponiveis", vehicle->numSeats);
    printf("\n");
}

void Printer_BusLine(BusLine *busLine) {
    PrettyPrintInt("Codigo da linha", busLine->lineCode);
    PrettyPrint("Nome da linha", busLine->name);
    PrettyPrint("Cor que descreve a linha", busLine->color);

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

    PrettyPrint("Aceita cartao", acceptsCardStr);

    if (acceptsCardStr != NULL) free(acceptsCardStr);
    printf("\n");
}
