#include "dataModel.h"
#include "utils.h"
#include <string.h>
#include <stdlib.h>

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

void BusLine_Free(BusLine* busLine) {
    free(busLine->name);
    free(busLine->color);
    free(busLine);
}