#include "stringTable.h"
#include "csvToBinary.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printStringWithMaxLength(const char* str, int maxLength) {
    // CIRAO: Como os campos de tamanho fixo nao tem terminador de string (\0), a gente tem que usar essa funcao pra printa as strings
    //        que tem tamanho fixo (ex. char[5]). Se nao ele printa lixo.
    int strLen = strlen(str);
    for (int i = 0; i < (strLen < maxLength ? strLen : maxLength); ++i) {
        printf("%c", str[i]);
    }
    printf("\n");
}

int main() {
    StringTable* busLines = StringTable_FromCsv("linha.csv", ',');

    // CIRAO 1: rowcount inclui a primeira linha (que tem os nomes das colunas) -> por isso o -1
    // CIRAO 2: na funcao CsvToBinary_CreateBusLineFromRow, se vc passar row=0, ele vai te dar a PRIMEIRA LINHA COM DADOS (e nao a linha com os nomes das colunas)
    for (int row = 0; row < busLines->rowCount-1; ++row) {
        BusLine* busLine = CsvToBinary_CreateBusLineFromRow(busLines, row);
        printf("LINHA DE BUSAO\n");
        printf("rem %d, cartao %d, nome %s, cor %s, tam em bytes %d\n", busLine->removed, busLine->acceptsCreditCard, busLine->name, busLine->color, busLine->regSize);
        printStringWithMaxLength(&busLine->lineCode[0], 4);
        BusLine_Free(busLine);
    }

    StringTable_Free(busLines);

    StringTable* vehicles = StringTable_FromCsv("veiculo.csv", ',');

    for (int row = 0; row < 5; ++row) {
        Vehicle* vehicle = CsvToBinary_CreateVehicleFromRow(vehicles, row);
        printf("VEICULO\n");
        printf("rem %d, assentos %d, modelo %s, categoria %s, tam em bytes %d\n", vehicle->removed, vehicle->numSeats, vehicle->model, vehicle->category, vehicle->regSize);
        printStringWithMaxLength(&vehicle->prefix[0], 5);
        printStringWithMaxLength(&vehicle->date[0], 10);
        printStringWithMaxLength(&vehicle->lineCode[0], 4);
        Vehicle_Free(vehicle);
    }

    StringTable_Free(vehicles);

    return 0;
}