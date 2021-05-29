#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "stringTable.h"
#include "dataModel.h"
#include "utils.h"


// Create an Vehicle header
VehicleHeader* CreateVehicleHeader(char* describePrefix, char* describeDate, char* describePlaces, char* describeLine, char* describeModel, char* describeCategory) {
    // Describe lenghts: 18, 35, 42, 26, 17, 20
    VehicleHeader* header = (VehicleHeader*) malloc(sizeof(VehicleHeader));
    
    header->status = '0';
    header->nextReg = 0;
    header->numReg = 0;
    header->numRegRemov = 0;

    // Copies over fixed-length fields
    Utils_StrCopyToFixedLen(header->describePrefix, describePrefix, 18);
    Utils_StrCopyToFixedLen(header->describeDate, describeDate, 35);
    Utils_StrCopyToFixedLen(header->describePlaces, describePlaces, 42);
    Utils_StrCopyToFixedLen(header->describeLine, describeLine, 26);
    Utils_StrCopyToFixedLen(header->describeModel, describeModel, 17);
    Utils_StrCopyToFixedLen(header->describeCategory, describeCategory, 20);

    return header;
}

VehicleHeader* BinaryHeaders_CreateVehicleHeader(StringTable* table) {
    return CreateVehicleHeader(StringTable_GetLabel(table, 0),
                                StringTable_GetLabel(table, 1),
                                StringTable_GetLabel(table, 2),
                                StringTable_GetLabel(table, 3),
                                StringTable_GetLabel(table, 4),
                                StringTable_GetLabel(table, 5));
}


// Create an busline Header
BusLineHeader* CreateBusLineHeader(char* describeCode, char* describeCard, char* describeName, char* describeLine) {
    // Describe lenghts: 15, 13, 13, 24
    BusLineHeader* header = (BusLineHeader*) malloc(sizeof(BusLineHeader));
    
    header->status = '0';
    header->nextReg = 0;
    header->numReg = 0;
    header->numRegRemov = 0;

    // Copies over fixed-length fields
    Utils_StrCopyToFixedLen(header->describeCode, describeCode, 15);
    Utils_StrCopyToFixedLen(header->describeCard, describeCard, 13);
    Utils_StrCopyToFixedLen(header->describeName, describeName, 13);
    Utils_StrCopyToFixedLen(header->describeLine, describeLine, 24);

    return header;                        
}

BusLineHeader* BinaryHeaders_CreateBusLineHeader(StringTable *table) {
    return CreateBusLineHeader(StringTable_GetLabel(table, 0),
                                StringTable_GetLabel(table, 1),
                                StringTable_GetLabel(table, 2),
                                StringTable_GetLabel(table, 3));
}


// Free the register header
void BinaryHeaders_FreeVehicleHeader(VehicleHeader* header) {
    free(header);
}

void BinaryHeaders_FreeBusLineHeader(BusLineHeader* header) {
    free(header);
}
