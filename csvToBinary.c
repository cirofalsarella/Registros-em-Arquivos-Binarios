#include "csvToBinary.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Vehicle* CsvToBinary_CreateVehicleFromRow(StringTable* table, int row) {
    // Gets each cell from csv, and converts to int when necessary
    char* prefix = StringTable_GetCellAt(table, 0, row);
    char* date = StringTable_GetCellAt(table, 1, row);
    char* numSeats = StringTable_GetCellAt(table, 2, row);
    char* lineCode = StringTable_GetCellAt(table, 3, row);
    char* model = StringTable_GetCellAt(table, 4, row);
    char* category = StringTable_GetCellAt(table, 5, row);

    char removed = prefix[0] == '*';
    return Vehicle_Create(removed, prefix, date, atoi(numSeats), lineCode, strdup(model), strdup(category));
}

BusLine* CsvToBinary_CreateBusLineFromRow(StringTable* table, int row) {
    // Gets each cell from csv, and converts to int when necessary
    char* lineCode = StringTable_GetCellAt(table, 0, row);
    char* acceptsCreditCard = StringTable_GetCellAt(table, 1, row);
    char* name = StringTable_GetCellAt(table, 2, row);
    char* color = StringTable_GetCellAt(table, 3, row);

    char removed = lineCode[0] == '*';
    return BusLine_Create(removed, lineCode, acceptsCreditCard[0], strdup(name), strdup(color));
}

void CsvToBinary_WriteVehicle(const Vehicle* vehicle, FILE* destFile) {
    // Writes each field from vehicle in the given file
    fwrite(&vehicle->removed, sizeof(char), 1, destFile);
    fwrite(&vehicle->regSize, sizeof(int32_t), 1, destFile);

    fwrite(&vehicle->prefix[0], sizeof(char), 5, destFile);
    fwrite(&vehicle->date[0], sizeof(char), 10, destFile);
    fwrite(&vehicle->numSeats, sizeof(int32_t), 1, destFile);
    fwrite(&vehicle->lineCode[0], sizeof(char), 4, destFile);

    // Variable-length fields
    fwrite(&vehicle->modelLength, sizeof(int32_t), 1, destFile);
    fwrite(vehicle->model, sizeof(char), vehicle->modelLength, destFile);
    fwrite(&vehicle->categoryLength, sizeof(int32_t), 1, destFile);
    fwrite(vehicle->category, sizeof(char), vehicle->categoryLength, destFile);
}

void CsvToBinary_WriteBusLine(const BusLine* busLine, FILE* destFile) {
    // Writes each field from bus line in the given file
    fwrite(&busLine->removed, sizeof(char), 1, destFile);
    fwrite(&busLine->regSize, sizeof(int32_t), 1, destFile);

    fwrite(&busLine->lineCode[0], sizeof(char), 4, destFile);
    fwrite(&busLine->acceptsCreditCard, sizeof(char), 1, destFile);

    // Variable-length fields
    fwrite(&busLine->nameLength, sizeof(int32_t), 1, destFile);
    fwrite(busLine->name, sizeof(char), busLine->nameLength, destFile);
    fwrite(&busLine->colorLength, sizeof(int32_t), 1, destFile);
    fwrite(busLine->color, sizeof(char), busLine->colorLength, destFile);
}