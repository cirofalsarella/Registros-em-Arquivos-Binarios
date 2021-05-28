#ifndef _BINARY_HEADERS_H_
#define _BINARY_HEADERS_H_

// FUNÇÕES RESPONSÁVEIS PELA MANIPULAÇÃO DOS HEADERS

//  Criam um cabeçalho a partir de uma tabela
VehicleHeader* binaryHeaders_CreateVehicleHeader(StringTable* table);
BusLineHeader* binaryHeaders_CreateBusLineHeader(StringTable *table);

//  Liberam a memória ocupada pelo cabeçalho
void binaryHeaders_FreeVehicleHeader(VehicleHeader* header);
void binaryHeaders_FreeBusLineHeader(BusLineHeader* header);

#endif