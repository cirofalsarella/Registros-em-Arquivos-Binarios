#include "operations.h"

void CreateTableVehicles() {
    char csvFile[128], binFile[128];
    scanf("%s %s", csvFile, binFile);

    StringTable* Table = StringTable_FromCsv(csvFile, ',');
    CsvToBinary_WriteVehicleFile(Table, binFile);

    StringTable_Free(Table);
    binarioNaTela(binFile);
}

void CreateTableBuslines() {
    char csvFile[128], binFile[128];
    scanf("%s %s", csvFile, binFile);

    StringTable* Table = StringTable_FromCsv(csvFile, ',');
    CsvToBinary_WriteBusLineFile(Table, binFile);

    StringTable_Free(Table);
    binarioNaTela(binFile);
}


void SelectVehicles() {
    char binFile[128];
    scanf("%s", binFile);

    int n_vehicles;
    Vehicle **vehicles = ReadBin_Vehicle(&n_vehicles, binFile);
    printf("NUMERO DE VEICULOS: %d\n", n_vehicles);
    for (int i = 0; i < n_vehicles; i++) {
        Printer_Vehicle(vehicles[i]);
        Vehicle_Free(vehicles[i]);
    }
}

void SelectBusLines() {
    char binFile[128];
    scanf("%s", binFile);

    int n_busLines;
    BusLine **buslines = ReadBin_BusLines(&n_busLines, binFile);

    for (int i = 0; i < n_busLines; i++) {
        Printer_BusLine(buslines[i]);
        BusLine_Free(buslines[i]);
    }
}

void SelectVehiclesWhere() {}
void SelectBuslinesWhere() {}

void InsertIntoVehicles() {}
void InsertIntoBuslines() {}