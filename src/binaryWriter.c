#include "binaryWriter.h"

// MARK: Single-instance write

void BinaryWriter_WriteVehicle(const Vehicle* vehicle, FILE* destFile) {
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

void BinaryWriter_WriteBusLine(const BusLine* busLine, FILE* destFile) {
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