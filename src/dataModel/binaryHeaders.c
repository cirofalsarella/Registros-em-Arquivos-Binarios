#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "binaryHeaders.h"
#include "../core/utils.h"

// ANCHOR: Create functions

VehicleHeader_t* BinaryHeaders_CreateVehicleHeader(int nextReg, int numReg, int numRegRemov) {
    // Describe lenghts: 18, 35, 42, 26, 17, 20
    VehicleHeader_t* header = (VehicleHeader_t*) malloc(sizeof(VehicleHeader_t));
    
    header->nextReg = nextReg;
    header->validRegCount = numReg;
    header->removedRegCount = numRegRemov;

    // Copies over fixed-length fields
    Utils_StrCopyToFixedLen(header->describePrefix, "Prefixo do veiculo", 18);
    Utils_StrCopyToFixedLen(header->describeDate, "Data de entrada do veiculo na frota", 35);
    Utils_StrCopyToFixedLen(header->describePlaces, "Quantidade de lugares sentados disponiveis", 42);
    Utils_StrCopyToFixedLen(header->describeLine, "Linha associada ao veiculo", 26);
    Utils_StrCopyToFixedLen(header->describeModel, "Modelo do veiculo", 17);
    Utils_StrCopyToFixedLen(header->describeCategory, "Categoria do veiculo", 20);

    return header;
}

BusLineHeader_t* BinaryHeaders_CreateBusLineHeader(int nextReg, int numReg, int numRegRemov) {
    // Describe lenghts: 15, 13, 13, 24
    BusLineHeader_t* header = (BusLineHeader_t*) malloc(sizeof(BusLineHeader_t));
    
    header->nextReg = nextReg;
    header->validRegCount = numReg;
    header->removedRegCount = numRegRemov;

    // Copies over fixed-length fields
    Utils_StrCopyToFixedLen(header->describeCode, "Codigo da linha", 15);
    Utils_StrCopyToFixedLen(header->describeCard, "Aceita cartao", 13);
    Utils_StrCopyToFixedLen(header->describeName, "Nome da linha", 13);
    Utils_StrCopyToFixedLen(header->describeLine, "Cor que descreve a linha", 24);

    return header;                        
}


// ANCHOR: Validation functions

char BinaryHeaders_IsVehicleHeaderValid(VehicleHeader_t* header) {
	return header->validRegCount >= 0 && header->removedRegCount >= 0;
}

char BinaryHeaders_IsBusLineHeaderValid(BusLineHeader_t* header) {
	return header->validRegCount >= 0 && header->removedRegCount >= 0;
}


// ANCHOR: Free functions

void BinaryHeaders_FreeVehicleHeader(VehicleHeader_t* header) {
    free(header);
}

void BinaryHeaders_FreeBusLineHeader(BusLineHeader_t* header) {
    free(header);
}