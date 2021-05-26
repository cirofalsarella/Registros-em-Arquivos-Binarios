#include "binaryHeaders.h"
#include "utils.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

VehicleHeader* VehicleHeader_Create(char status, int64_t nextReg, int32_t numReg, int32_t numRegRemov, char* describePrefix,
                    char* describeDate, char* describePlaces, char* describeLine, char* describeModel, char* describeCategory) {
    // Describe lenghts: 18, 35, 42, 26, 17, 20
    VehicleHeader* header = (VehicleHeader*) malloc(sizeof(VehicleHeader));
    
    header->status = status;
    header->nextReg = nextReg;
    header->numReg = numReg;
    header->numRegRemov = numRegRemov;

    // Copies over fixed-length fields
    Utils_StrCopyToFixedLen(header->describePrefix, describePrefix, 18);
    Utils_StrCopyToFixedLen(header->describeDate, describeDate, 35);
    Utils_StrCopyToFixedLen(header->describePlaces, describePlaces, 42);
    Utils_StrCopyToFixedLen(header->describeLine, describeLine, 26);
    Utils_StrCopyToFixedLen(header->describeModel, describeModel, 17);
    Utils_StrCopyToFixedLen(header->describeCategory, describeCategory, 20);

    return header;
}

void VehicleHeader_Free(VehicleHeader* header) {
    free(header);
}

BusLineHeader* BusLineHeader_Create(char status, int64_t nextReg, int32_t numReg, int32_t numRegRemov, char* describeCode,
                                    char* describeCard, char* describeName, char* describeLine) {
    // Describe lenghts: 15, 13, 13, 24
    BusLineHeader* header = (BusLineHeader*) malloc(sizeof(BusLineHeader));
    
    header->status = status;
    header->nextReg = nextReg;
    header->numReg = numReg;
    header->numRegRemov = numRegRemov;

    // Copies over fixed-length fields
    Utils_StrCopyToFixedLen(header->describeCode, describeCode, 15);
    Utils_StrCopyToFixedLen(header->describeCard, describeCard, 13);
    Utils_StrCopyToFixedLen(header->describeName, describeName, 13);
    Utils_StrCopyToFixedLen(header->describeLine, describeLine, 24);

    return header;                        
}

void BusLineHeader_Free(BusLineHeader* header) {
    free(header);
}

VehicleHeader* VehicleHeader_CreateFromTable(StringTable* table) {
    return VehicleHeader_Create('0', 0, 0, 0,
                                StringTable_GetLabel(table, 0),
                                StringTable_GetLabel(table, 1),
                                StringTable_GetLabel(table, 2),
                                StringTable_GetLabel(table, 3),
                                StringTable_GetLabel(table, 4),
                                StringTable_GetLabel(table, 5));
}

BusLineHeader* BusLineHeader_CreateFromTable(StringTable *table) {
    return BusLineHeader_Create('0', 0, 0, 0,
                                StringTable_GetLabel(table, 0),
                                StringTable_GetLabel(table, 1),
                                StringTable_GetLabel(table, 2),
                                StringTable_GetLabel(table, 3));
}