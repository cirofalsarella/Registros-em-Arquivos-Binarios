#include "csvToBinary.h"
#include "binaryHeaders.h"
#include "binaryWriter.h"

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

void CsvToBinary_WriteVehicleFile(StringTable *table, char *fileName) {
    Vehicle** vehicles = (Vehicle**) malloc ((table->rowCount - 1) * sizeof(Vehicle*));

    VehicleHeader* header = VehicleHeader_CreateFromTable(table);
    header->numReg = table->rowCount -1;
    for (int i = 0; i < table->rowCount-1; i++) {
        vehicles[i] = CsvToBinary_CreateVehicleFromRow(table, i);
        if (vehicles[i]->removed) {
            header->numRegRemov++;
            header->numReg--;
        }
    }

    FILE* destFile = fopen(fileName, "wb");
    BinaryWriter_WriteVehicleHeader(header, destFile);

    for (int i=0; i < table->rowCount-1; i++){
        BinaryWriter_WriteVehicle(vehicles[i], destFile);
        Vehicle_Free(vehicles[i]);
    }

    fseek(destFile, 0, SEEK_END);
    header->nextReg = ftell(destFile);
    fseek(destFile, 1, SEEK_SET);
    fwrite(&header->nextReg, sizeof(int64_t), 1, destFile);

    header->status = '1';
    fseek(destFile, 0, SEEK_SET);
    fwrite(&header->status, 1, 1, destFile);

    for (int i = 0; i < table->rowCount - 1; ++i) {
        Vehicle_Free(vehicles[i]);
    }

    fclose(destFile);
    free(vehicles);
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

void CsvToBinary_WriteBusLineFile(StringTable *table, char *fileName){
    BusLineHeader* header = BusLineHeader_CreateFromTable(table);
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
    BinaryWriter_WriteBusLineHeader(header, destFile);

    for (int i=0; i<table->rowCount-1; i++){
        BinaryWriter_WriteBusLine(busLines[i], destFile);
        BusLine_Free(busLines[i]);
    }

    header->status = 1;
    fseek(destFile, 0, SEEK_SET);
    fwrite(&header->status, 1, 1, destFile);

    fclose(destFile);
    free(busLines);
    free(header);
}
