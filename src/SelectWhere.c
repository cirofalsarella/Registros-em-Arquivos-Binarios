#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dataModel.h"
#include "selectWhere.h"
#include "utils.h"
#include "printer.h"


// Funções que serão usadas para executar comparações

int BusLine_codLinha(void *pattern, Vehicle* v, BusLine *busLine) {
    return (busLine->lineCode == *((int32_t*)pattern));
}
int BusLine_aceitaCartao(void *pattern, Vehicle* v, BusLine *busLine) {
    return (busLine->acceptsCreditCard == *((char*)pattern));
}
int BusLine_nomeLinha(void *pattern, Vehicle* v, BusLine *busLine) {
    return strcmp(busLine->name, (char*) pattern) == 0;
}
int BusLine_corLinha(void *pattern, Vehicle* v, BusLine *busLine) {
    return strcmp(busLine->color, (char*) pattern) == 0;
}

int Vehicle_prefixo(void *pattern, Vehicle *vehicle, BusLine *b) {
    return Utils_StrEqualsFixedLen(vehicle->prefix, (char*) pattern, 5);
}
int Vehicle_data(void *pattern, Vehicle *vehicle, BusLine *b) {
    return Utils_StrEqualsFixedLen(vehicle->date, (char*) pattern, 10);
}
int Vehicle_quantidadeLugares(void *pattern, Vehicle *vehicle, BusLine *b) {
    return (vehicle->numSeats == *((int32_t*) pattern));
}
int Vehicle_modelo(void *pattern, Vehicle *vehicle, BusLine *b) {
    return strcmp(vehicle->model, (char*) pattern) == 0;   
}
int Vehicle_categoria(void *pattern, Vehicle *vehicle, BusLine *b) {
    return strcmp(vehicle->category, (char*) pattern) == 0;
}


//  Retorna o ponteiro para alguma das funções acima
SelectWhereFnType SelectWhere_SetCondition(char *fieldName) {
    if (strcmp(fieldName, "codLinha") == 0)     return BusLine_codLinha;
    if (strcmp(fieldName, "aceitaCartao") == 0) return BusLine_aceitaCartao;
    if (strcmp(fieldName, "nomeLinha") == 0)    return BusLine_nomeLinha;
    if (strcmp(fieldName, "corLinha") == 0)     return BusLine_corLinha;

    if (strcmp(fieldName, "prefixo") == 0)           return Vehicle_prefixo;
    if (strcmp(fieldName, "data") == 0)              return Vehicle_data;
    if (strcmp(fieldName, "quantidadeLugares") == 0) return Vehicle_quantidadeLugares;
    if (strcmp(fieldName, "modelo") == 0)            return Vehicle_modelo;
    if (strcmp(fieldName, "categoria") == 0)         return Vehicle_categoria;

    return NULL;
}

//  Salva o modelo em uma variável
void* SelectWhere_SetPattern(char *fieldName) {
    void *pattern = NULL;

    if (strcmp(fieldName, "nomeLinha") == 0 || strcmp(fieldName, "corLinha") == 0
     || strcmp(fieldName, "prefixo") == 0   || strcmp(fieldName, "data") == 0
     || strcmp(fieldName, "modelo") == 0    || strcmp(fieldName, "categoria") == 0) {
        char* aux = (char*) calloc(128, sizeof(char));
        Utils_ScanQuoteString(aux);
        pattern = aux;
    } else {
        if (strcmp(fieldName, "codLinha") == 0 || strcmp(fieldName, "quantidadeLugares") == 0) {
            int32_t* aux = malloc(sizeof(int32_t));
            scanf("%d", aux);
            pattern = aux;
        } else {
            char* aux = malloc(sizeof(char));
            scanf("%c", aux);
            pattern = aux;
        }
    }

    return pattern;
}



//  Seleciona os Registros

Vehicle** SelectWhere_SelectVehicles(SelectWhereFnType shouldSelectFunc, void* pattern, Vehicle **vehicles, int nReg, int *nSelectedReg) {
    Vehicle** selectedVehicles = NULL;
    
    int k = 0;
    for (int i = 0; i < nReg; i++) {
        if (shouldSelectFunc(pattern, vehicles[i], NULL)) {
            selectedVehicles = realloc(selectedVehicles, (k+1)*sizeof(Vehicle*));
            selectedVehicles[k] = vehicles[i];
            k++;
        }
    }

    *nSelectedReg = k;
    return selectedVehicles;
}

BusLine **SelectWhere_SelectBusLines(SelectWhereFnType shouldSelectFunc, void *pattern, BusLine** busLines, int nReg, int *nSelectedReg) {
    BusLine** selectedBusLines = NULL;

    int k = 0;
    for (int i = 0; i < nReg; i++) {
        if (shouldSelectFunc(pattern, NULL, busLines[i])) {
            selectedBusLines = realloc(selectedBusLines, (k+1)*sizeof(Vehicle*));
            selectedBusLines[k] = busLines[i];
            k++;
        }
    }

    *nSelectedReg = k;
    return selectedBusLines;
}