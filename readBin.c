#include "readBin.h"
#include "printer.h"
#include <assert.h>

VehicleHeader_t *ReadVehicleHeader(FILE *srcFile){
    VehicleHeader_t *header = calloc(1, sizeof(VehicleHeader_t));

    fread(&header->status, sizeof(char), 1, srcFile);
    fread(&header->nextReg, sizeof(long), 1, srcFile);
    fread(&header->numReg, sizeof(int), 1, srcFile);
    fread(&header->numRegRemov, sizeof(int), 1, srcFile);

    fread(&header->describePrefix, sizeof(char), 18, srcFile);
    fread(&header->describeDate, sizeof(char), 35, srcFile);
    fread(&header->describePlaces, sizeof(char), 42, srcFile);
    fread(&header->describeLine, sizeof(char), 26, srcFile);
    fread(&header->describeModel, sizeof(char), 17, srcFile);
    fread(&header->describeCategory, sizeof(char), 20, srcFile);

    return header;
}

Vehicle *ReadVehicle(FILE *srcFile) {
    Vehicle *vehicle = calloc(1, sizeof(Vehicle));

    fread(&vehicle->removed, sizeof(char), 1, srcFile);
    fread(&vehicle->regSize, sizeof(int32_t), 1, srcFile);
    fread(&vehicle->prefix[0], sizeof(char), 5, srcFile);
    fread(&vehicle->date[0], sizeof(char), 10, srcFile);
    fread(&vehicle->numSeats, sizeof(int32_t), 1, srcFile);
    fread(&vehicle->lineCode[0], sizeof(char), 4, srcFile);

    // Variable-length fields
    fread(&vehicle->modelLength, sizeof(int32_t), 1, srcFile);
    vehicle->model = calloc(vehicle->modelLength, sizeof(char));
    fread(vehicle->model, sizeof(char), vehicle->modelLength, srcFile);

    fread(&vehicle->categoryLength, sizeof(int32_t), 1, srcFile);
    vehicle->category = calloc(vehicle->categoryLength, sizeof(char));
    fread(vehicle->category, sizeof(char), vehicle->categoryLength, srcFile);

    if (vehicle->removed == 1){
        Vehicle_Free(vehicle);
        vehicle = NULL;
    }

    return vehicle;
}

Vehicle **ReadBin_Vehicle(int *n_vehicles, char *fileName) {
    FILE *srcFile = fopen(fileName, "rb");

    // Reads the header
    VehicleHeader_t *header = ReadVehicleHeader(srcFile);

    int validRegisters = header->numReg - header->numRegRemov;
    assert(validRegisters > 0);

    // Allocates space for the vehicles
    Vehicle **vehicles = calloc(validRegisters, sizeof(Vehicle*));

    // Gets the vehicles from the file
    int i = 0;
    while (i < validRegisters) {
        Vehicle *aux = ReadVehicle(srcFile);
        if (aux != NULL) {
            vehicles[i] = aux;
            i++;
        }
    }

    fclose(srcFile);

    *n_vehicles = validRegisters;
    free(header);
    return vehicles;
}

int numerizeVehicleArgument(char *argument) {
    if (strcmp(argument, "prefixo") == 0) return 1;
    if (strcmp(argument, "data") == 0) return 2;
    if (strcmp(argument, "quantidadeLugares") == 0) return 3;
    if (strcmp(argument, "modelo") == 0) return 4;
    return 5;
}

Vehicle **ReadBin_VehicleWhere(int *n_vehicles, char *fileName, char *argument, void *value) {
    Vehicle **vehicles = ReadBin_Vehicle(n_vehicles, fileName);
}


BusLineHeader_t *ReadBusLineHeader(FILE *srcFile){
    BusLineHeader_t *header = calloc(1, sizeof(VehicleHeader_t));

    // freads the fields stored in the binary file, in order
    fread(&header->status, sizeof(char), 1, srcFile);
    fread(&header->nextReg, sizeof(int64_t), 1, srcFile);
    fread(&header->numReg, sizeof(int32_t), 1, srcFile);
    fread(&header->numRegRemov, sizeof(int32_t), 1, srcFile);

    fread(&header->describeCode, sizeof(char), 15, srcFile);
    fread(&header->describeCard, sizeof(char), 13, srcFile);
    fread(&header->describeName, sizeof(char), 13, srcFile);
    fread(&header->describeLine, sizeof(char), 24, srcFile);

    return header;
}

BusLine *ReadBusLine(FILE *srcFile) {
    BusLine *busLine = calloc(1, sizeof(BusLine));

    // freads the fields stored in the binary file, in order
    fread(&busLine->removed, sizeof(char), 1, srcFile);
    fread(&busLine->regSize, sizeof(int32_t), 1, srcFile);

    fread(&busLine->lineCode[0], sizeof(char), 4, srcFile);
    fread(&busLine->acceptsCreditCard, sizeof(char), 1, srcFile);

    fread(&busLine->nameLength, sizeof(int32_t), 1, srcFile);
    busLine->name = calloc(busLine->nameLength, sizeof(char));
    fread(busLine->name, sizeof(char), busLine->nameLength, srcFile);

    fread(&busLine->colorLength, sizeof(int32_t), 1, srcFile);
    busLine->color = calloc(busLine->colorLength, sizeof(char));
    fread(busLine->color, sizeof(char), busLine->colorLength, srcFile);

    if (busLine->removed == 1){
        BusLine_Free(busLine);
        busLine = NULL;
    }

    return busLine;
}

BusLine **ReadBin_BusLines(int *n_busLines, char *fileName) {
    FILE *srcFile = fopen(fileName, "rb");

    // Reads the header
    BusLineHeader_t *header = ReadBusLineHeader(srcFile);

    int validRegisters = header->numReg - header->numRegRemov;
    assert(validRegisters > 0);

    // Allocates space for the bus lines
    BusLine **busLines = calloc(validRegisters, sizeof(BusLine*));

    // Gets the bus lines from the file
    int i = 0;
    while (i < validRegisters) {
        BusLine *aux = ReadBusLine(srcFile);
        if (aux != NULL) {
            busLines[i] = aux;
            i++;
        }
    }

    fclose(srcFile);

    *n_busLines = validRegisters;
    free(header);
    return busLines;
}

BusLine **ReadBin_BusLineWhere(int *n_buslines, char *fileName, char *argument, void *value) {
    Vehicle **vehicles = ReadBin_BusLines(n_buslines, fileName);
}