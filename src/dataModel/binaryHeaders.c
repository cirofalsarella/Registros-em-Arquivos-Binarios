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
    header->validRegCount = numReg;
    header->removedRegCount = numRegRemov;

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
BusLineHeader_t* BinaryHeaders_CreateBusLineHeader(int nextReg, int numReg, int numRegRemov) {
    // Describe lenghts: 15, 13, 13, 24
    BusLineHeader_t* header = (BusLineHeader_t*) malloc(sizeof(BusLineHeader_t));
    
    header->nextReg = nextReg;
    header->validRegCount = numReg;
    header->removedRegCount = numRegRemov;

    // Copies over fixed-length fields
    // TODO: deixar os campos de descrição setados
    Utils_StrCopyToFixedLen(header->describeCode, "", 15);
    Utils_StrCopyToFixedLen(header->describeCard, "", 13);
    Utils_StrCopyToFixedLen(header->describeName, "", 13);
    Utils_StrCopyToFixedLen(header->describeLine, "", 24);

    return header;                        
}

// ANCHOR: Validation functions

char BinaryHeaders_IsVehicleHeaderValid(VehicleHeader_t* header) {
	return header->validRegCount >= 0 && header->removedRegCount <= header->validRegCount;
}

char BinaryHeaders_IsBusLineHeaderValid(BusLineHeader_t* header) {
	return header->validRegCount >= 0 && header->removedRegCount <= header->validRegCount;
}


// ANCHOR: Free functions

void BinaryHeaders_FreeVehicleHeader(VehicleHeader_t* header) {
    free(header);
}

void BinaryHeaders_FreeBusLineHeader(BusLineHeader_t* header) {
    free(header);
}