#include "selectWhere.h"
#include "utils.h"
#include "printer.h"


// *((tipo*) pattern) -> cast pra patern ser um ponteiro de tipo definido e vou para o endereço apontado

int BusLine_codLinha(void *pattern, BusLine *busLine) {
    return (busLine->lineCode != *((int32_t*)pattern));
}
int BusLine_aceitaCartao(void *pattern, BusLine *busLine) {
    return (busLine->acceptsCreditCard != *((char*)pattern));
}
int BusLine_nomeLinha(void *pattern, BusLine *busLine) {
    return strcmp(busLine->name, (char*) pattern);
}
int BusLine_corLinha(void *pattern, BusLine *busLine) {
    return strcmp(busLine->color, (char*) pattern);
}

int Vehicle_prefixo(void *pattern, Vehicle *vehicle) {
    return strcmp(vehicle->prefix, (char*) pattern);
}
int Vehicle_data(void *pattern, Vehicle *vehicle) {
    return strcmp(vehicle->date, (char*) pattern);   
}
int Vehicle_quantidadeLugares(void *pattern, Vehicle *vehicle) {
    return (vehicle->numSeats != *((int32_t*) pattern));
}
int Vehicle_modelo(void *pattern, Vehicle *vehicle) {
    return strcmp(vehicle->model, (char*) pattern);   
}
int Vehicle_categoria(void *pattern, Vehicle *vehicle) {
    return strcmp(vehicle->category, (char*) pattern);
}

void* SelectWhere_SetCondition(char *fieldName) {
    if (!strcmp(fieldName, "codLinha"))     return BusLine_codLinha;
    if (!strcmp(fieldName, "aceitaCartao")) return BusLine_aceitaCartao;
    if (!strcmp(fieldName, "nomeLinha"))    return BusLine_nomeLinha;
    if (!strcmp(fieldName, "corLinha"))     return BusLine_corLinha;

    if (!strcmp(fieldName, "prefixo"))           return Vehicle_prefixo;
    if (!strcmp(fieldName, "data"))              return Vehicle_data;
    if (!strcmp(fieldName, "quantidadeLugares")) return Vehicle_quantidadeLugares;
    if (!strcmp(fieldName, "modelo"))            return Vehicle_modelo;
    if (!strcmp(fieldName, "categoria"))         return Vehicle_categoria;

    return NULL;
}

void* SelectWhere_SetPattern(char *fieldName) {
    void *pattern = NULL;

    if (!strcmp(fieldName, "nomeLinha") || !strcmp(fieldName, "corLinha")
     || !strcmp(fieldName, "prefixo") || !strcmp(fieldName, "data")
     || !strcmp(fieldName, "modelo") || !strcmp(fieldName, "categoria") ) {
        char* aux = (char*) calloc(128, sizeof(char));
        Utils_ScanQuoteString(aux);
        pattern = aux;
    } else {
        if (!strcmp(fieldName, "codLinha") || !strcmp(fieldName, "quantidadeLugares") ) {
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


Vehicle** SelectWhere_SelectVehicles(void *functionPt(void*, Vehicle*), void* pattern, Vehicle **vehicles, int nReg, int *nSelectedReg) {
    Vehicle** selectedVehicles = NULL;

    int k = 0;
    for (int i=0; i<nReg; i++) {
        if (!functionPt(pattern, vehicles[i])) {
            selectedVehicles = realloc(selectedVehicles, (k+1)*sizeof(Vehicle*));
            selectedVehicles[k] = vehicles[i];
            k++;
        }
    }

    *nSelectedReg = k;
    return selectedVehicles;
}

BusLine **SelectWhere_SelectBusLines(void *functionPt(void*, BusLine*), void *pattern, BusLine** busLines, int nReg, int *nSelectedReg) {
    BusLine** selectedBusLines = NULL;

    int k = 0;
    for (int i=0; i<nReg; i++) {
        if (!functionPt(pattern, busLines[i]) ) {
            selectedBusLines = realloc(selectedBusLines, (k+1)*sizeof(Vehicle*));
            selectedBusLines[k] = busLines[i];
            k++;
        }
    }

    *nSelectedReg = k;
    return selectedBusLines;
}