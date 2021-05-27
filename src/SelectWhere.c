#include "selectWhere.h"
#include "utils.h"

int BusLine_codLinha(void *pattern, BusLine *busLine){
    return (busLine->lineCode != (int32_t) pattern );
}
int BusLine_aceitaCartao(void *pattern, BusLine *busLine){
    return (busLine->acceptsCreditCard != (char) pattern);
}
int BusLine_nomeLinha(void *pattern, BusLine *busLine){
    return strcmp(busLine->name, (char*) pattern);
}
int BusLine_corLinha(void *pattern, BusLine *busLine){
    return strcmp(busLine->color, (char*) pattern);
}

int Vehicle_prefixo(void *pattern, Vehicle *vehicle){
    return strcmp(vehicle->prefix, (char*) pattern);
}
int Vehicle_data(void *pattern, Vehicle *vehicle){
    return strcmp(vehicle->date, (char*) pattern);   
}
int Vehicle_quantidadeLugares(void *pattern, Vehicle *vehicle){
    return (vehicle->numSeats != (int32_t) pattern);
}
int Vehicle_modelo(void *pattern, Vehicle *vehicle){
    return srtcmp(vehicle->model, (char*) pattern);   
}
int Vehicle_categoria(void *pattern, Vehicle *vehicle){
    return strcmp(vehicle->category, (char*) pattern);
}

void* SelectWhere_SetCondition(char *fieldName) {
    if (!strcmp(fieldName, "codLinha"))     return BusLine_codLinha;
    if (!strcmp(fieldName, "aceitaCartao")) return BusLine_aceitaCartao;
    if (!strcmp(fieldName, "nomeLinha"))    return BusLine_nomeLinha;
    if (!strcmp(fieldName, "corLinha"))     return BusLine_codLinha;

    if (!strcmp(fieldName, "prefixo"))              return Vehicle_prefixo;
    if (!strcmp(fieldName, "data"))                 return Vehicle_data;
    if (!strcmp(fieldName, "quantidadeLugares"))    return Vehicle_quantidadeLugares;
    if (!strcmp(fieldName, "modelo"))               return Vehicle_modelo;
    if (!strcmp(fieldName, "categoria"))            return Vehicle_categoria;

    return NULL;
}

void *SelectWhere_SetPattern(char *fieldName) {
    void *pattern;
    if (!strcmp(fieldName, "nomeLinha") || !strcmp(fieldName, "corLinha")
     || !strcmp(fieldName, "prefixo") || !strcmp(fieldName, "data")
     || !strcmp(fieldName, "modelo") || !strcmp(fieldName, "categoria") ) {
            Utils_ScanQuoteString(pattern);
    } else {
        if (!strcmp(fieldName, "codLinha") || !strcmp(fieldName, "quantidadeLugares") ) {
            scanf("%d", (int32_t*) pattern);
        } else {
            scanf("%c", (char*) pattern);
        }
    }

    return NULL;
}


Vehicle** SelectWhere_SelectVehicles(void *functionPt(void*, Vehicle*), void *pattern, Vehicle ***vehicles, int* vehiclesCount) {
    int n = *vehiclesCount;
    for (int i = 0; i < *vehiclesCount; i++) {
        if (functionPt(pattern, *vehicles[i])){
            Vehicle_Free(*vehicles[i]);
            *vehicles[i] = NULL;
            n--;
        }
    }

    Vehicle** selectedVehicles = calloc(n, sizeof(Vehicle*));
    n = 0;
    for (int i = 0; i < *vehiclesCount; i++) {
        if (*vehicles[i] != NULL) {
            selectedVehicles[n] = *vehicles[i];
            n++;
        }
    }

    free(*vehicles);
    *vehiclesCount = n;
    return selectedVehicles;
}

BusLine **SelectWhere_SelectBusLines(void *functionPt(void*, BusLine*), void *pattern, BusLine*** busLines, int* busLinesCount) {
    int n = *busLinesCount;
    for (int i = 0; i < *busLinesCount; i++) {
        if (functionPt(pattern, *busLines[i])){
            BusLine_Free(*busLines[i]);
            *busLines[i] = NULL;
            n--;
        }
    }

    BusLine **selectedBusLines = calloc(n, sizeof(BusLine*));
    n = 0;
    for (int i = 0; i < *busLinesCount; i++) {
        if (*busLines[i] != NULL) {
            selectedBusLines[n] = *busLines[i];
            n++;
        }
    }

    free(*busLines);
    *busLinesCount = n;
    return selectedBusLines;
}