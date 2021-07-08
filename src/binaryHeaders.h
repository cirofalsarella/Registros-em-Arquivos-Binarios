#ifndef _BINARY_HEADERS_H_
#define _BINARY_HEADERS_H_

#include <stdint.h>

// FUNÇÕES RESPONSÁVEIS PELA MANIPULAÇÃO DOS HEADERS

//  Criam um cabeçalho a partir de uma tabela
VehicleHeader_t* BinaryHeaders_CreateVehicleHeader(char* describePrefix, char* describeDate, char* describePlaces, char* describeLine, char* describeModel, char* describeCategory);
BusLineHeader_t* BinaryHeaders_CreateBusLineHeader(char* describeCode, char* describeCard, char* describeName, char* describeLine);

void BinaryHeaders_FreeVehicleHeader(VehicleHeader_t* header);
void BinaryHeaders_FreeBusLineHeader(BusLineHeader_t* header);

#endif