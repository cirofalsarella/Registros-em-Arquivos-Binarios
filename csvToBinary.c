#include "csvToBinary.h"

//TODO -> nextReg


// Vehicle

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

VehicleHeader_t *CreateVehicleHeader(StringTable *table) {
    VehicleHeader_t *header = malloc (sizeof(VehicleHeader_t));
    
    strcpy(header->describePrefix, StringTable_GetCellAt(table, 0, 0));
    strcpy(header->describeDate, StringTable_GetCellAt(table, 1, 0));
    strcpy(header->describePlaces, StringTable_GetCellAt(table, 2, 0));
    strcpy(header->describeLine, StringTable_GetCellAt(table, 3, 0));
    strcpy(header->describeModel, StringTable_GetCellAt(table, 4, 0));
    strcpy(header->describeCategory, StringTable_GetCellAt(table, 5, 0));

    return header;
}

void WriteVehicleHeader(VehicleHeader_t *header, FILE *destFile) {
    fwrite(&header->status, sizeof(char), 1, destFile);
    fwrite(&header->nextReg, sizeof(int64_t), 1, destFile);
    fwrite(&header->numReg, sizeof(int32_t), 1, destFile);
    fwrite(&header->numRegRemov, sizeof(int32_t), 1, destFile);

    fwrite(&header->describePrefix, sizeof(char), 18, destFile);
    fwrite(&header->describeDate, sizeof(char), 35, destFile);
    fwrite(&header->describePlaces, sizeof(char), 42, destFile);
    fwrite(&header->describeLine, sizeof(char), 26, destFile);
    fwrite(&header->describeModel, sizeof(char), 17, destFile);
    fwrite(&header->describeCategory, sizeof(char), 20, destFile);
}

void CsvToBinary_WriteVehicleFile(StringTable *table, char *fileName){
    VehicleHeader_t *header = CreateVehicleHeader(table);
    Vehicle **Vehicles = (Vehicle**) malloc ((table->rowCount - 1) * sizeof(Vehicle*));

    header->numReg = table->rowCount -1;
    header->nextReg = 0;
    for (int i = 0; i < table->rowCount-1; i++) {
        Vehicles[i] = CsvToBinary_CreateVehicleFromRow(table, i);
        if (Vehicles[i]->removed) {
            header->numRegRemov++;
        }
    }

    FILE *destFile = fopen(fileName, "wb");
    WriteVehicleHeader(header, destFile);
    for (int i=0; i<table->rowCount-1; i++){
        CsvToBinary_WriteVehicle(Vehicles[i], destFile);
        Vehicle_Free(Vehicles[i]);
    }

    header->status = 1;
    fseek(destFile, 0, SEEK_SET);
    fwrite(&header->status, 1, 1, destFile);

    fclose(destFile);
    free(Vehicles);
    free(header);
}


//  BusLine

BusLine* CsvToBinary_CreateBusLineFromRow(StringTable* table, int row) {
    // Gets each cell from csv, and converts to int when necessary
    char* lineCode = StringTable_GetCellAt(table, 0, row);
    char* acceptsCreditCard = StringTable_GetCellAt(table, 1, row);
    char* name = StringTable_GetCellAt(table, 2, row);
    char* color = StringTable_GetCellAt(table, 3, row);

    char removed = lineCode[0] == '*';
    return BusLine_Create(removed, lineCode, acceptsCreditCard[0], strdup(name), strdup(color));
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

BusLineHeader_t *CreateBusLineHeader(StringTable *table) {
    BusLineHeader_t *header = malloc (sizeof(BusLineHeader_t));
    
    strcpy(header->describeCard, StringTable_GetCellAt(table, 0, 0));
    strcpy(header->describeCode, StringTable_GetCellAt(table, 1, 0));
    strcpy(header->describeName, StringTable_GetCellAt(table, 2, 0));
    strcpy(header->describeLine, StringTable_GetCellAt(table, 3, 0));
    header->numRegRemov = 0;
    header->status = 0;

    return header;
}

void WriteBusLineHeader(BusLineHeader_t *header, FILE *destFile) {
    fwrite(&header->status, sizeof(char), 1, destFile);
    fwrite(&header->nextReg, sizeof(int64_t), 1, destFile);
    fwrite(&header->numReg, sizeof(int32_t), 1, destFile);
    fwrite(&header->numRegRemov, sizeof(int32_t), 1, destFile);

    fwrite(&header->describeCode, sizeof(char), 15, destFile);
    fwrite(&header->describeCard, sizeof(char), 13, destFile);
    fwrite(&header->describeName, sizeof(char), 13, destFile);
    fwrite(&header->describeLine, sizeof(char), 24, destFile);
}

void CsvToBinary_WriteBusLineFile(StringTable *table, char *fileName){
    BusLineHeader_t *header = CreateBusLineHeader(table);
    BusLine **busLines = (BusLine**) malloc ((table->rowCount - 1) * sizeof(BusLine*));

    header->numReg = table->rowCount -1;
    header->nextReg = 0;
    for (int i = 0; i < table->rowCount-1; i++) {
        busLines[i] = CsvToBinary_CreateBusLineFromRow(table, i);
        if (busLines[i]->removed) {
            header->numRegRemov++;
        }
    }

    FILE *destFile = fopen(fileName, "wb");
    WriteBusLineHeader(header, destFile);
    for (int i=0; i<table->rowCount-1; i++){
        CsvToBinary_WriteBusLine(busLines[i], destFile);
        BusLine_Free(busLines[i]);
    }

    header->status = 1;
    fseek(destFile, 0, SEEK_SET);
    fwrite(&header->status, 1, 1, destFile);

    fclose(destFile);
    free(busLines);
    free(header);
}
