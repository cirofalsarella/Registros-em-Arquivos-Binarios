#ifndef _READ_BINARY_H_
#define _READ_BINARY_H_

// FUNÇÕES RESPONSÁVEIS PELA LEITURA DE ARQUIVOS BINÁRIOS

//  Leem os registros não removidos de um arquivo binário
//  O header é um parâmetro por referência, para podermos usar seus valores depois
Vehicle** binaryReader_Vehicles(VehicleHeader** header, char *fileName);
BusLine** binaryReader_BusLines(BusLineHeader** header, char *fileName);

#endif