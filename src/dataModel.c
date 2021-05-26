#include "dataModel.h"
#include "utils.h"
#include <string.h>
#include <stdlib.h>

// TODO: Deal with NULOs

Vehicle* Vehicle_Create(char removed, char* prefix, char* date, int32_t numSeats, char* lineCode, char* model, char* category) {    
    int modelLength = strlen(model);
    int categoryLength = strlen(category);

    // This code is basically just copying fields
    Vehicle* vehicle = malloc(sizeof(Vehicle));
    vehicle->removed = removed;
    // Reg size = Variable fields offset + 2x 4-byte integers + length of the two strings
    vehicle->regSize = VEHICLE_VAR_OFFSET + sizeof(int32_t)*2 + modelLength + categoryLength;
    Utils_StrCopyToFixedLen(prefix, &vehicle->prefix[0], 5);
    Utils_StrCopyToFixedLen(date, &vehicle->date[0], 10);
    vehicle->numSeats = numSeats;
    Utils_StrCopyToFixedLen(lineCode, &vehicle->lineCode[0], 4);
    vehicle->modelLength = modelLength;
    vehicle->model = model;
    vehicle->categoryLength = categoryLength;
    vehicle->category = category;
    return vehicle;
}

void Vehicle_Free(Vehicle* vehicle) {
    free(vehicle->model);
    free(vehicle->category);
    free(vehicle);
}

BusLine* BusLine_Create(char removed, char* lineCode, char acceptsCreditCard, char* name, char* color) {
    int nameLength = strlen(name);
    int colorLength = strlen(color);

    // This code is basically just copying fields
    BusLine* busLine = malloc(sizeof(BusLine));
    busLine->removed = removed;
    // Reg size = Variable fields offset + 2x 4-byte integers + length of the two strings
    busLine->regSize = BUS_LINE_VAR_OFFSET + sizeof(int32_t)*2 + nameLength + colorLength;
    Utils_StrCopyToFixedLen(lineCode, &busLine->lineCode[0], 4);
    busLine->acceptsCreditCard = acceptsCreditCard;
    busLine->nameLength = nameLength;
    busLine->name = name;
    busLine->colorLength = colorLength;
    busLine->color = color;
    return busLine;
}

void BusLine_Free(BusLine* busLine) {
    free(busLine->name);
    free(busLine->color);
    free(busLine);
}