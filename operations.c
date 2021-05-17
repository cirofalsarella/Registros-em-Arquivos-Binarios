#include "operations.h"

void CreateTableVehicles() {
    char csvFile[128], binFile[128];
    scanf("%s %s", csvFile, binFile);

    StringTable* Table = StringTable_FromCsv(csvFile, ',');
    int flag = CsvToBinary_WriteVehicleFile(Table, binFile);

    StringTable_Free(Table);
}

void CreateTableBuslines() {
    char csvFile[128], binFile[128];
    scanf("%s %s", csvFile, binFile);

    StringTable* Table = StringTable_FromCsv(csvFile, ',');
    int flag = CsvToBinary_WriteBusLineFile(Table, binFile);

    StringTable_Free(Table);
}

void SelectVehicles() {}
void SelectBuslines() {}

void SelectVehiclesWhere() {}
void SelectBuslinesWhere() {}

void InsertIntoVehicles() {}
void InsertIntoBuslines() {}