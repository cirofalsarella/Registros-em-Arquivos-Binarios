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

    free(vehicles);
}

void SelectBusLines() {
    char binFile[128];
    scanf("%s", binFile);

    int n_busLines;
    BusLine **buslines = ReadBin_BusLines(&n_busLines, binFile);

    printf("NUMERO DE LINHAS: %d\n", n_busLines);
    for (int i = 0; i < n_busLines; i++) {
        Printer_BusLine(buslines[i]);
        BusLine_Free(buslines[i]);
    }

    free(buslines);
}


void SelectVehiclesWhere() {
    char binFile[128];
    scanf("%s", binFile);

    int n_vehicles;
    Vehicle **vehicles = ReadBin_Vehicle(&n_vehicles, binFile);

    char fieldName[64];
    scanf("%s", fieldName);
    void *functionPt = SelectWhere_setCondition(fieldName);
    void *pattern = SelectWhere_setPattern(fieldName);
    vehicles = SelectWhere_selectVehicles(functionPt, pattern, &vehicles, &n_vehicles);

    printf("NUMERO DE VEICULOS: %d\n", n_vehicles);
    for (int i = 0; i < n_vehicles; i++) {
        Printer_Vehicle(vehicles[i]);
        Vehicle_Free(vehicles[i]);
    }

    free(vehicles);
}

void SelectBuslinesWhere() {
    char binFile[128];
    scanf("%s", binFile);

    int n_busLines;
    BusLine **buslines = ReadBin_BusLines(&n_busLines, binFile);

    char fieldName[64];
    scanf("%s", fieldName);
    void *functionPt = SelectWhere_setCondition(fieldName);
    void *pattern = SelectWhere_setPattern(fieldName);
    buslines = SelectWhere_selectBusLines(functionPt, pattern, &buslines, &n_busLines);

    printf("NUMERO DE LINHAS: %d\n", n_busLines);
    for (int i = 0; i < n_busLines; i++) {
        Printer_BusLine(buslines[i]);
        BusLine_Free(buslines[i]);
    }

    free(buslines);
}

void InsertIntoVehicles() {}
void InsertIntoBuslines() {}