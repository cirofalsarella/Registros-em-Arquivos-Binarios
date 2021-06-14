#ifndef _READ_BINARY_H_
#define _READ_BINARY_H_

// FUNÇÕES RESPONSÁVEIS PELA LEITURA DE ARQUIVOS BINÁRIOS

//  Leem os registros não removidos de um arquivo binário
//  O header é um parâmetro por referência, para podermos usar seus valores depois
Vehicle_t** binaryReader_Vehicles(VehicleHeader_t** header, char *fileName);
BusLine_t** binaryReader_BusLines(BusLineHeader_t** header, char *fileName);

#endif