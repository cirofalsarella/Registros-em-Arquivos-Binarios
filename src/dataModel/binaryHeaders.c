#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "binaryHeaders.h"
#include "../core/utils.h"

// ANCHOR: Create functions

// Create an Vehicle header
VehicleHeader_t* BinaryHeaders_CreateVehicleHeader(int nextReg, int numReg, int numRegRemov) {
    // Describe lenghts: 18, 35, 42, 26, 17, 20
    VehicleHeader_t* header = (VehicleHeader_t*) malloc(sizeof(VehicleHeader_t));
    
    header->nextReg = nextReg;
    header->numReg = numReg;
    header->numRegRemov = numRegRemov;

    // Copies over fixed-length fields
    // TODO: deixar os campos de descrição setados
    Utils_StrCopyToFixedLen(header->describePrefix, "", 18);
    Utils_StrCopyToFixedLen(header->describeDate, "", 35);
    Utils_StrCopyToFixedLen(header->describePlaces, "", 42);
    Utils_StrCopyToFixedLen(header->describeLine, "", 26);
    Utils_StrCopyToFixedLen(header->describeModel, "", 17);
    Utils_StrCopyToFixedLen(header->describeCategory, "", 20);

    return header;
}


// Create an busline Header
BusLineHeader_t* BinaryHeaders_CreateBusLineHeader(char* describeCode, char* describeCard, char* describeName, char* describeLine) {
    // Describe lenghts: 15, 13, 13, 24
    BusLineHeader_t* header = (BusLineHeader_t*) malloc(sizeof(BusLineHeader_t));
    
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


// ANCHOR: Free functions

void BinaryHeaders_FreeVehicleHeader(VehicleHeader_t* header) {
    free(header);
}

void BinaryHeaders_FreeBusLineHeader(BusLineHeader_t* header) {
    free(header);
}