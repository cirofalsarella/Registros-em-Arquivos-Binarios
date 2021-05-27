#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "dataModel.h"
#include "utils.h"

Vehicle* Vehicle_Create(char removed, char* prefix, char* date, int32_t numSeats, int32_t lineCode, char* model, char* category) {    
    // This code is basically just copying fields
    Vehicle* vehicle = malloc(sizeof(Vehicle));

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

Vehicle** Vehicle_Read(int n) {
    Vehicle** vehicles = calloc(n, sizeof(Vehicle*));
    
    for (int i = 0; i < n; i++) {
        // Scans prefix, date, numSeats and lineCode (all fixed-length fields)
        char prefix[5] = { '\0' };
        Utils_ScanQuoteString(prefix);

        char date[10] = { '\0' };
        Utils_ScanQuoteString(date);

        char numSeats[64] = { '\0' };
        scanf("%s", &numSeats[0]);

        char lineCode[64] = { '\0' };
        scanf("%s", &lineCode[0]);

        // the lineCode must not be NULL
        if ( !strcmp(lineCode, "NULO")) {
            for (int j=0; j<i; j++) {
                Vehicle_Free(vehicles[i]);
            }
            free(vehicles);
            return NULL;
        }

        // Scans model and category (not fixed-length)
        char* model = calloc(100, sizeof(char));
        char* category = calloc(100, sizeof(char));

        Utils_ScanQuoteString(model);
        Utils_ScanQuoteString(category);

        // Creates a new vehicle and pushes it
        vehicles[i] = Vehicle_Create('1', prefix, date, Utils_StrToInt(numSeats), Utils_StrToInt(lineCode), model, category);        
    }


    return vehicles;
}

void Vehicle_Free(Vehicle* vehicle) {
    free(vehicle->model);
    free(vehicle->category);
    free(vehicle);
}

BusLine* BusLine_Create(char removed, int32_t lineCode, char acceptsCreditCard, char* name, char* color) {
    // This code is basically just copying fields
    BusLine* busLine = malloc(sizeof(BusLine));

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

BusLine** BusLine_Read(int n) {
    BusLine** buslines = calloc(n, sizeof(BusLine*));
    
    for (int i = 0; i < n; i++) {
        // Scans lineCode, date, numSeats and lineCode (all fixed-length fields)
        int lineCode;
        scanf("%d", &lineCode);

        char aceitaCartao[1] = { '\0' };
        Utils_ScanQuoteString(aceitaCartao);
        
        // variable len fields
        char* nomeLinha = calloc(100, sizeof(char));
        char* corLinha = calloc(100, sizeof(char));

        Utils_ScanQuoteString(nomeLinha);
        Utils_ScanQuoteString(corLinha);

        // the lineCode must not be NULL
//        if ( !strcmp(lineCode, "NULO")) {
//            for (int j=0; j<i; j++) {
//                BusLine_Free(buslines[i]);
//            }
//            free(buslines);
//            return NULL;
//        }

        // Creates a new vehicle and pushes it
        buslines[i] = BusLine_Create('1', lineCode, aceitaCartao[0], nomeLinha, corLinha);
    }

    return buslines;
}

void BusLine_Free(BusLine* busLine) {
    if (busLine != NULL){
        if (busLine->name != NULL)  free(busLine->name);
        if (busLine->color != NULL) free(busLine->color);
        free(busLine);
        busLine = NULL;
    }
}