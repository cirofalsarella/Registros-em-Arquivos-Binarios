#ifndef _BINARY_HEADERS_H_
#define _BINARY_HEADERS_H_

#include <stdint.h>

// FUNÇÕES RESPONSÁVEIS PELA MANIPULAÇÃO DOS HEADERS

//  Criam um cabeçalho a partir de uma tabela
VehicleHeader_t* BinaryHeaders_CreateVehicleHeader(StringTable* table);
BusLineHeader_t* BinaryHeaders_CreateBusLineHeader(StringTable *table);

//  Liberam a memória ocupada pelo cabeçalho
void BinaryHeaders_FreeVehicleHeader(VehicleHeader_t* header);
void BinaryHeaders_FreeBusLineHeader(BusLineHeader_t* header);

#endif