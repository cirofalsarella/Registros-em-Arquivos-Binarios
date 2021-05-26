#include "binaryHeaders.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

VehicleHeader* VehicleHeader_CreateFromTable(StringTable* table) {
    VehicleHeader* header = (VehicleHeader*) malloc(sizeof(VehicleHeader));
    
    strcpy(header->describePrefix, table->cells[0 + (0) * table->columnCount]);
    strcpy(header->describeDate, table->cells[1 + (0) * table->columnCount]);
    strcpy(header->describePlaces, table->cells[2 + (0) * table->columnCount]);
    strcpy(header->describeLine, table->cells[3 + (0) * table->columnCount]);
    strcpy(header->describeModel, table->cells[4 + (0) * table->columnCount]);
    strcpy(header->describeCategory, table->cells[5 + (0) * table->columnCount]);

    header->numRegRemov = 0;
    header->status = '0';

    // TODO: Nem tudo esta inicializado?

    return header;
}

BusLineHeader* BusLineHeader_CreateFromTable(StringTable *table) {
    BusLineHeader* header = (BusLineHeader*) malloc(sizeof(BusLineHeader));
    
    strcpy(header->describeCard, StringTable_GetCellAt(table, 0, 0));
    strcpy(header->describeCode, StringTable_GetCellAt(table, 1, 0));
    strcpy(header->describeName, StringTable_GetCellAt(table, 2, 0));
    strcpy(header->describeLine, StringTable_GetCellAt(table, 3, 0));
    
    header->numRegRemov = 0;
    header->status = 0;
    
    // TODO: Nem tudo esta inicializado?

    return header;
}