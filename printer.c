#include "printer.h"

/*
void printStringWithMaxLength(const char* str, int maxLength) {
    int strLen = strlen(str);
    for (int i = 0; i < (strLen < maxLength ? strLen : maxLength); ++i) {
        printf("%c", str[i]);
    }
    printf("\n");
}
*/


void PrintVehicle(Vehicle *vehicle) {
    printf("Prefixo do veiculo: %s\n", vehicle->prefix);
    printf("Modelo do veiculo: %s\n", vehicle->model);
    printf("Categoria do veiculo: %s\n", vehicle->category);
    printf("Data de entrada do veiculo na frota: %s\n", vehicle->date);
    printf("Quantidade de lugares sentados disponiveis: %d\n", vehicle->numSeats);
    printf("\n");

//  printStringWithMaxLength(&vehicle->prefix[0], 5);
//  printStringWithMaxLength(&vehicle->date[0], 10);
//  printStringWithMaxLength(&vehicle->lineCode[0], 4);
}

void PrintBusLine(BusLine *busLine) {
    printf("Nome da linha: %s\n", busLine->name);
    printf("Cor que descreve a linha: %s\n", busLine->color);
    printf("Aceita cartao: %s\n", (busLine->acceptsCreditCard ? "SOMENTE COM CARTAO SEM PRESENCA DE COBRADOR" : "PAGAMENTO EM CARTAO SEM COBRADOR"));
    printf("\n");

//  printStringWithMaxLength(&busLine->lineCode[0], 4);
}
