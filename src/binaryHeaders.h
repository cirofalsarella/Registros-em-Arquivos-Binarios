#ifndef _BINARY_HEADERS_H_
#define _BINARY_HEADERS_H_


// FUNÇÕES RESPONSÁVEIS PELA MANIPULAÇÃO DOS HEADERS

//  Criam um cabeçalho a partir de uma tabela
VehicleHeader* BinaryHeaders_CreateVehicleHeader(StringTable* table);
BusLineHeader* BinaryHeaders_CreateBusLineHeader(StringTable *table);

//  Liberam a memória ocupada pelo cabeçalho
void BinaryHeaders_FreeVehicleHeader(VehicleHeader* header);
void BinaryHeaders_FreeBusLineHeader(BusLineHeader* header);

#endif