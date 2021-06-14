#include <stdio.h>
#include <stdlib.h>

#include "dataModel.h"

VehicleHeader_t* ReadVehicleHeader(FILE *srcFile) {
    VehicleHeader_t* header = calloc(1, sizeof(VehicleHeader_t));

    fread(&header->status, sizeof(char), 1, srcFile);
    fread(&header->nextReg, sizeof(int64_t), 1, srcFile);
    fread(&header->numReg, sizeof(int32_t), 1, srcFile);
    fread(&header->numRegRemov, sizeof(int32_t), 1, srcFile);

    fread(&header->describePrefix, sizeof(char), 18, srcFile);
    fread(&header->describeDate, sizeof(char), 35, srcFile);
    fread(&header->describePlaces, sizeof(char), 42, srcFile);
    fread(&header->describeLine, sizeof(char), 26, srcFile);
    fread(&header->describeModel, sizeof(char), 17, srcFile);
    fread(&header->describeCategory, sizeof(char), 20, srcFile);

    return header;
}

Vehicle_t* ReadVehicle(FILE *srcFile) {
    Vehicle_t* vehicle = calloc(1, sizeof(Vehicle_t));

    //  Fixed length fields
    fread(&vehicle->removed, sizeof(char), 1, srcFile);
    
    fread(&vehicle->regSize, sizeof(int32_t), 1, srcFile);  // 4
    fread(&vehicle->prefix[0], sizeof(char), 5, srcFile);   // 5
    fread(&vehicle->date[0], sizeof(char), 10, srcFile);    // 10
    fread(&vehicle->numSeats, sizeof(int32_t), 1, srcFile); // 4
    fread(&vehicle->lineCode, sizeof(int32_t), 1, srcFile); // 4


    // Variable length fields
    fread(&vehicle->modelLength, sizeof(int32_t), 1, srcFile);
    vehicle->model = calloc(vehicle->modelLength+1, sizeof(char));
    fread(vehicle->model, sizeof(char), vehicle->modelLength, srcFile);

    fread(&vehicle->categoryLength, sizeof(int32_t), 1, srcFile);
    vehicle->category = calloc(vehicle->categoryLength+1, sizeof(char));
    fread(vehicle->category, sizeof(char), vehicle->categoryLength, srcFile);

    return vehicle;
}

Vehicle_t** binaryReader_Vehicles(VehicleHeader_t** header, char* fileName) {
    FILE* srcFile = fopen(fileName, "rb");
    if (srcFile == NULL) {
        return NULL;
    } else {
        char status;
        fread(&status, sizeof(char), 1, srcFile);
        if (status == '0'){
            fclose(srcFile);
            return NULL;
        }
        fseek(srcFile, 0, SEEK_SET);
    }

    // Reads the header
    *header = ReadVehicleHeader(srcFile);

    // Allocates space for the vehicles
    Vehicle_t** vehicles = calloc((*header)->numReg, sizeof(Vehicle_t*));
    int n_registers = (*header)->numReg + (*header)->numRegRemov;

    // Gets the vehicles that arent removed from the file
    int j = 0;
    for (int i=0; i < n_registers; i++) {
        Vehicle_t* aux = ReadVehicle(srcFile);
        if (aux != NULL) {
            if (aux->removed == '0') {
                Vehicle_Free(aux);
            } else {
                vehicles[j] = aux;
                j++;
            }
        }
    }

    fclose(srcFile);
    return vehicles;
}


BusLineHeader_t* ReadBusLineHeader(FILE *srcFile) {
    BusLineHeader_t* header = calloc(1, sizeof(VehicleHeader_t));

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

BusLine_t* ReadBusLine(FILE *srcFile) {
    BusLine_t* busLine = calloc(1, sizeof(BusLine_t));

    //  Fixed length fields
    fread(&busLine->removed, sizeof(char), 1, srcFile);
    fread(&busLine->regSize, sizeof(int32_t), 1, srcFile);

    fread(&busLine->lineCode, sizeof(int32_t), 1, srcFile);
    fread(&busLine->acceptsCreditCard, sizeof(char), 1, srcFile);


    // Variable length fields
    fread(&busLine->nameLength, sizeof(int32_t), 1, srcFile);
    busLine->name = calloc(busLine->nameLength+1, sizeof(char));
    fread(busLine->name, sizeof(char), busLine->nameLength, srcFile);

    fread(&busLine->colorLength, sizeof(int32_t), 1, srcFile);
    busLine->color = calloc(busLine->colorLength+1, sizeof(char));
    fread(busLine->color, sizeof(char), busLine->colorLength, srcFile);

    return busLine;
}

BusLine_t** binaryReader_BusLines(BusLineHeader_t** header, char* fileName) {
    FILE* srcFile = fopen(fileName, "rb");
    if (srcFile == NULL) {
        return NULL;
    } else {
        char status;
        fread(&status, sizeof(char), 1, srcFile);
        if (status == '0'){
            fclose(srcFile);
            return NULL;
        }
        fseek(srcFile, 0, SEEK_SET);
    }
    
    // Reads the header
    *header = ReadBusLineHeader(srcFile);

    // Allocates space for the bus lines
    BusLine_t** busLines = calloc((*header)->numReg, sizeof(BusLine_t*));
    int n_registers = (*header)->numReg + (*header)->numRegRemov;

    // Gets the bus lines that arent removed from the file
    int j = 0;
    for (int i=0; i < n_registers; i++) {
        BusLine_t* aux = ReadBusLine(srcFile);
        if (aux != NULL) {
            if (aux->removed == '0') {
                BusLine_Free(aux);
            } else {
                busLines[j] = aux;
                j++;
            }
        }
    }

    fclose(srcFile);
    return busLines;
}

