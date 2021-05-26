#include "dataModel.h"
#include "utils.h"
#include <string.h>
#include <stdlib.h>

// TODO: NULOS + Fix hashes (nextReg is different)

Vehicle* Vehicle_Create(char removed, char* prefix, char* date, int32_t numSeats, int32_t lineCode, char* model, char* category) {    
    int modelLength = strlen(model);
    int categoryLength = strlen(category);

    // This code is basically just copying fields
    Vehicle* vehicle = malloc(sizeof(Vehicle));
    vehicle->removed = removed;
    // Reg size = Variable fields offset + 2x 4-byte integers + length of the two strings
    vehicle->regSize = VEHICLE_VAR_OFFSET + sizeof(int32_t)*2 + modelLength + categoryLength;
    Utils_StrCopyToFixedLen(&vehicle->prefix[0], prefix, 5);
    Utils_StrCopyToFixedLen(&vehicle->date[0], date, 10);

    vehicle->numSeats = numSeats;
    vehicle->lineCode = lineCode;
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

BusLine* BusLine_Create(char removed, int32_t lineCode, char acceptsCreditCard, char* name, char* color) {
    int nameLength = strlen(name);
    int colorLength = strlen(color);

    // This code is basically just copying fields
    BusLine* busLine = malloc(sizeof(BusLine));
    busLine->removed = removed;
    // Reg size = Variable fields offset + 2x 4-byte integers + length of the two strings
    busLine->regSize = BUS_LINE_VAR_OFFSET + sizeof(int32_t)*2 + nameLength + colorLength;
    busLine->lineCode = lineCode;
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