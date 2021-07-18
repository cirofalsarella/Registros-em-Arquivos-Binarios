#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "dataModel.h"
#include "../core/utils.h"


Vehicle_t* Vehicle_Create(char removed, char* prefix, char* date, int32_t numSeats, int32_t lineCode, char* model, char* category, int64_t offset) {    
    // This code is basically just copying fields
    Vehicle_t* vehicle = malloc(sizeof(Vehicle_t));

    vehicle->offset = offset;

    vehicle->modelLength = model == NULL ? 0 : strlen(model);
    vehicle->model = model;
    vehicle->categoryLength = category == NULL ? 0 : strlen(category);
    vehicle->category = category;

    // Checks for NULL

    if (strcmp(model, "NULO") == 0) {
        vehicle->model = NULL;
        vehicle->modelLength = 0;
        free(model);
    }
    
    if (strcmp(category, "NULO") == 0) {
        vehicle->category = NULL;
        vehicle->categoryLength = 0;
        free(category);
    }

    vehicle->removed = removed;
    // Reg size = Fixed length + Length of the two strings
    vehicle->regSize = VEHICLE_FIXED_LENGTH + vehicle->modelLength + vehicle->categoryLength;
    Utils_StrCopyToFixedLen(&vehicle->prefix[0], prefix, 5);
    Utils_StrCopyToFixedLen(&vehicle->date[0], date, 10);

    vehicle->numSeats = numSeats;
    vehicle->lineCode = lineCode;

    return vehicle;
}

Vehicle_t* Vehicle_Read() {
    // Scans prefix, date, numSeats and lineCode (all fixed-length fields)
    char prefix[20] = { '\0' };
    Utils_ScanQuoteString(&prefix[0]);
    
    // Checks if the register is removed
    char removed = '1';
    if (prefix[0] == '*') {
        removed = '0';
        // Removes '*' from prefix
        char* prefixCopy = Utils_StrCopy(prefix+1); // Copies without the '*'
        int prefixCopyLen = strlen(prefixCopy);
        strcpy(prefix, prefixCopy);
        prefix[prefixCopyLen] = '\0';
        free(prefixCopy);
    }

    char date[20] = { '\0' };
    Utils_ScanQuoteString(&date[0]);

    char numSeats[64] = { '\0' };
    scanf("%s", &numSeats[0]);

    char lineCode[64] = { '\0' };
    scanf("%s", &lineCode[0]);

    // Scans model and category (not fixed-length)
    char* model = calloc(200, sizeof(char));
    char* category = calloc(200, sizeof(char));

    Utils_ScanQuoteString(model);
    Utils_ScanQuoteString(category);

    // Creates a new vehicle and pushes it
    return Vehicle_Create(removed, prefix, date, Utils_StrToInt(numSeats), Utils_StrToInt(lineCode), model, category, -1);        
}

void Vehicle_Free(Vehicle_t* vehicle) {
    if (vehicle == NULL) return;

    if (vehicle->model != NULL) {
        free(vehicle->model);
    }

    if (vehicle->category != NULL) {
        free(vehicle->category);
    }

    free(vehicle);
}



BusLine_t* BusLine_Create(char removed, int32_t lineCode, char acceptsCreditCard, char* name, char* color, int64_t offset) {
    // This code is basically just copying fields
    BusLine_t* busLine = malloc(sizeof(BusLine_t));

    busLine->offset = offset;

    busLine->nameLength = name == NULL ? 0 : strlen(name);
    busLine->name = name;
    busLine->colorLength = color == NULL ? 0 : strlen(color);
    busLine->color = color;

    // Checks for NULL

    if (strcmp(name, "NULO") == 0) {
        busLine->name = NULL;
        busLine->nameLength = 0;
        free(name);
    }

    if (strcmp(color, "NULO") == 0) {
        busLine->color = NULL;
        busLine->colorLength = 0;
        free(color);
    }

    busLine->removed = removed;
    // Reg size = Variable fields offset + 2x 4-byte integers + length of the two strings
    busLine->regSize = BUSLINE_FIXED_LENGTH + busLine->nameLength + busLine->colorLength;
    busLine->lineCode = lineCode;
    busLine->acceptsCreditCard = acceptsCreditCard;
    
    return busLine;
}

BusLine_t* BusLine_Read() {
    // Lê o código da linha como string, confere se é removido
    char removed = '1';

    char lineCodeStr[64] = { '\0' };
    Utils_ScanQuoteString(&lineCodeStr[0]);

    if (lineCodeStr[0] == '*') {
        removed = '0';
        // Removes '*' from linecode
        char* lineCodeStrCopy = Utils_StrCopy(lineCodeStr+1); // Copies without the '*'
        int lineCodeStrCopyLen = strlen(lineCodeStrCopy);
        strcpy(lineCodeStr, lineCodeStrCopy);
        lineCodeStr[lineCodeStrCopyLen] = '\0';
        free(lineCodeStrCopy);
    }

    // Checks if NULL
    if (!strcmp(lineCodeStr, "NULO"))   return NULL;

    int lineCode = atoi(lineCodeStr);

    char aceitaCartao[10] = { '\0' };
    Utils_ScanQuoteString(&aceitaCartao[0]);
    
    // Variable len fields
    char* nomeLinha = calloc(200, sizeof(char));
    char* corLinha = calloc(200, sizeof(char));

    Utils_ScanQuoteString(nomeLinha);
    Utils_ScanQuoteString(corLinha);


    // Creates a new busline
    return BusLine_Create(removed, lineCode, aceitaCartao[0], nomeLinha, corLinha, -1);
}

void BusLine_Free(BusLine_t* busLine) {
    if (busLine != NULL) {
        if (busLine->name != NULL) free(busLine->name);
        if (busLine->color != NULL) free(busLine->color);
        free(busLine);
        busLine = NULL;
    }
}