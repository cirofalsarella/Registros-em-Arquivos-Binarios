#include "readBin.h"
#include "printer.h"


VehicleHeader_t *ReadVehicleHeader(FILE *srcFile){
    VehicleHeader_t *header = malloc (sizeof(VehicleHeader_t));

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
    Vehicle *vehicle = malloc (sizeof(Vehicle));

    fread(&vehicle->removed, sizeof(char), 1, srcFile);
    fread(&vehicle->regSize, sizeof(int32_t), 1, srcFile);
    fread(&vehicle->prefix[0], sizeof(char), 5, srcFile);
    fread(&vehicle->date[0], sizeof(char), 10, srcFile);
    fread(&vehicle->numSeats, sizeof(int32_t), 1, srcFile);
    fread(&vehicle->lineCode[0], sizeof(char), 4, srcFile);

    // Variable-length fields
    fread(&vehicle->modelLength, sizeof(int32_t), 1, srcFile);
    vehicle->model = malloc (vehicle->modelLength * sizeof(char));
    fread(vehicle->model, sizeof(char), vehicle->modelLength, srcFile);

    fread(&vehicle->categoryLength, sizeof(int32_t), 1, srcFile);
    vehicle->category = malloc (vehicle->categoryLength * sizeof(char));
    fread(vehicle->category, sizeof(char), vehicle->categoryLength, srcFile);

    if (vehicle->removed == 1){
        Vehicle_Free(vehicle);
        vehicle = NULL;
    }

    return vehicle;
}

Vehicle **ReadBin_Vehicle(int *n_vehicles, char *fileName) {
    FILE *srcFile = fopen(fileName, "rb");

    VehicleHeader_t *header = ReadVehicleHeader(srcFile);
    int validRegisters = (header->numReg - header->numRegRemov);

    Vehicle **vehicles = malloc ( validRegisters * sizeof(Vehicle*) );

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

BusLineHeader_t *ReadBusLineHeader(FILE *srcFile){
    BusLineHeader_t *header = malloc (sizeof(VehicleHeader_t));

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
    BusLine *busLine = malloc (sizeof(BusLine));

    fread(&busLine->removed, sizeof(char), 1, srcFile);
    fread(&busLine->regSize, sizeof(int32_t), 1, srcFile);

    fread(&busLine->lineCode[0], sizeof(char), 4, srcFile);
    fread(&busLine->acceptsCreditCard, sizeof(char), 1, srcFile);

    fread(&busLine->nameLength, sizeof(int32_t), 1, srcFile);
    busLine->name = malloc (busLine->nameLength * sizeof(char));
    fread(busLine->name, sizeof(char), busLine->nameLength, srcFile);

    fread(&busLine->colorLength, sizeof(int32_t), 1, srcFile);
    busLine->color = malloc (busLine->colorLength * sizeof(char));
    fread(busLine->color, sizeof(char), busLine->colorLength, srcFile);

    if (busLine->removed == 1){
        BusLine_Free(busLine);
        busLine = NULL;
    }

    return busLine;
}

BusLine **ReadBin_BusLines(int *n_busLines, char *fileName) {
    FILE *srcFile = fopen(fileName, "rb");

    BusLineHeader_t *header = ReadBusLineHeader(srcFile);
    int validRegisters = (header->numReg - header->numRegRemov);

    BusLine **busLines = malloc ( validRegisters * sizeof(BusLine*) );

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
