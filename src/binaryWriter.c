#include "binaryWriter.h"
#include "binaryHeaders.h"

// MARK: Single-instance write

void BinaryWriter_WriteVehicle(const Vehicle* vehicle, FILE* destFile) {
    // Writes each field from vehicle in the given file
    fwrite(&vehicle->removed, sizeof(char), 1, destFile);
    fwrite(&vehicle->regSize, sizeof(int32_t), 1, destFile);
   
    fwrite(&vehicle->prefix[0], sizeof(char), 5, destFile);
    
    fwrite(&vehicle->date[0], sizeof(char), 10, destFile);
    fwrite(&vehicle->numSeats, sizeof(int32_t), 1, destFile);

    fwrite(&vehicle->lineCode, sizeof(int32_t), 1, destFile);

    // Variable-length fields
    fwrite(&vehicle->modelLength, sizeof(int32_t), 1, destFile);

    if (vehicle->model != NULL) {
        fwrite(vehicle->model, sizeof(char), vehicle->modelLength, destFile);
    }

    fwrite(&vehicle->categoryLength, sizeof(int32_t), 1, destFile);

    if (vehicle->category != NULL) {
        fwrite(vehicle->category, sizeof(char), vehicle->categoryLength, destFile);
    }
}

void BinaryWriter_WriteBusLine(const BusLine* busLine, FILE* destFile) {
    // Writes each field from bus line in the given file
    fwrite(&busLine->removed, sizeof(char), 1, destFile);
    fwrite(&busLine->regSize, sizeof(int32_t), 1, destFile);

    fwrite(&busLine->lineCode, sizeof(int32_t), 1, destFile);
    fwrite(&busLine->acceptsCreditCard, sizeof(char), 1, destFile);

    // Variable-length fields
    fwrite(&busLine->nameLength, sizeof(int32_t), 1, destFile);

    if (busLine->name != NULL) {
        fwrite(busLine->name, sizeof(char), busLine->nameLength, destFile);
    }

    fwrite(&busLine->colorLength, sizeof(int32_t), 1, destFile);

    if (busLine->color != NULL) {
        fwrite(busLine->color, sizeof(char), busLine->colorLength, destFile);
    }
}


// MARK: Header writes

void BinaryWriter_WriteVehicleHeader(const VehicleHeader* header, FILE *destFile) {
    fwrite(&header->status, sizeof(char), 1, destFile);
    fwrite(&header->nextReg, sizeof(int64_t), 1, destFile);
    fwrite(&header->numReg, sizeof(int32_t), 1, destFile);
    fwrite(&header->numRegRemov, sizeof(int32_t), 1, destFile);

    fwrite(&header->describePrefix[0], sizeof(char), 18, destFile);
    fwrite(&header->describeDate[0], sizeof(char), 35, destFile);
    fwrite(&header->describePlaces[0], sizeof(char), 42, destFile);
    fwrite(&header->describeLine[0], sizeof(char), 26, destFile);
    fwrite(&header->describeModel[0], sizeof(char), 17, destFile);
    fwrite(&header->describeCategory[0], sizeof(char), 20, destFile);
}

void BinaryWriter_WriteBusLineHeader(const BusLineHeader* header, FILE *destFile) {
    fwrite(&header->status, sizeof(char), 1, destFile);
    fwrite(&header->nextReg, sizeof(int64_t), 1, destFile);
    fwrite(&header->numReg, sizeof(int32_t), 1, destFile);
    fwrite(&header->numRegRemov, sizeof(int32_t), 1, destFile);

    fwrite(&header->describeCode[0], sizeof(char), 15, destFile);
    fwrite(&header->describeCard[0], sizeof(char), 13, destFile);
    fwrite(&header->describeName[0], sizeof(char), 13, destFile);
    fwrite(&header->describeLine[0], sizeof(char), 24, destFile);
}


void BinaryWriter_CreateVehicleFile(Vehicle** vehicles, int vehiclesCount, VehicleHeader* header, char* fileName) {
    header->status = '1';
    // These are adjusted throughout this function
    header->numReg = vehiclesCount;
    header->numRegRemov = 0;
    header->nextReg = 0;

    // First, calculates number of removed registers
    for (int i = 0; i < vehiclesCount; i++) {
        if (vehicles[i]->removed == '0') {
            header->numRegRemov++;
            header->numReg--;
        }
    }

    // Opens the file and writes the header
    FILE* destFile = fopen(fileName, "wb");
    BinaryWriter_WriteVehicleHeader(header, destFile);

    for (int i = 0; i < vehiclesCount; i++){
        BinaryWriter_WriteVehicle(vehicles[i], destFile);
        Vehicle_Free(vehicles[i]);
    }

    // Calculates and writes nextReg
    fseek(destFile, 0, SEEK_END);
    header->nextReg = ftell(destFile);
    fseek(destFile, 1, SEEK_SET);
    fwrite(&header->nextReg, sizeof(int64_t), 1, destFile);

    // Writes status
    fseek(destFile, 0, SEEK_SET);
    fwrite(&header->status, 1, 1, destFile);

    // Frees and closes everything
    fclose(destFile);
    free(vehicles);
    VehicleHeader_Free(header);
}

void BinaryWriter_CreateBusLineFile(BusLine** busLines, int busLinesCount, BusLineHeader* header, char* fileName) {
    header->status = '1';
    // These are adjusted throughout this function
    header->numReg = busLinesCount;
    header->numRegRemov = 0;
    header->nextReg = 0;

    // First, calculates number of removed registers
    for (int i = 0; i < busLinesCount; i++) {
        if (busLines[i]->removed == '0') {
            header->numRegRemov++;
            header->numReg--;
        }
    }

    // Opens the file and writes the header
    FILE* destFile = fopen(fileName, "wb");
    BinaryWriter_WriteBusLineHeader(header, destFile);

    for (int i = 0; i < busLinesCount; i++){
        BinaryWriter_WriteBusLine(busLines[i], destFile);
        BusLine_Free(busLines[i]);
    }

    // Calculates and writes nextReg
    fseek(destFile, 0, SEEK_END);
    header->nextReg = ftell(destFile);
    fseek(destFile, 1, SEEK_SET);
    fwrite(&header->nextReg, sizeof(int64_t), 1, destFile);

    // Writes status
    fseek(destFile, 0, SEEK_SET);
    fwrite(&header->status, 1, 1, destFile);

    // Frees and closes everything
    fclose(destFile);
    free(busLines);
    BusLineHeader_Free(header);
}

void BinaryWriter_IncrementVehicleFile(Vehicle** vehicles, int vehiclesCount, char* fileName) {
    FILE *destFile = fopen(fileName, "rb+");
    if (destFile == NULL) {
        return;
    }

    // Set as editing mode
    char status = '0';
    fwrite(&status, sizeof(char), 1, destFile);

    // TODO: Incrementar numero de veiculos no header

    // Placing the pointer
    int64_t proxReg;
    fread(&proxReg, sizeof(int64_t), 1, destFile);
    fseek(destFile, proxReg, SEEK_SET);

    // Writing the registers
    for (int i = 0; i < vehiclesCount; i++){
        BinaryWriter_WriteVehicle(vehicles[i], destFile);
        Vehicle_Free(vehicles[i]);
    }

    // Adjusting the proxReg field
    proxReg = ftell(destFile);
    fseek(destFile, 1, SEEK_SET);
    fwrite(&proxReg, sizeof(int64_t), 1, destFile);

    // Set as default mode
    status = '1';
    fseek(destFile, 0, SEEK_SET);
    fwrite(&status, sizeof(char), 1, destFile);

    fclose(destFile);
}