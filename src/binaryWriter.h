#ifndef _BINARY_WRITER_H_
#define _BINARY_WRITER_H_


//  FUNÇÕES RESPONSÁVEIS POR ESCREVER ARQUIVOS BINÁRIOS

//  Escrevem um arquivo inteiro
void BinaryWriter_CreateVehicleFile(Vehicle** vehicles, int vehiclesCount, VehicleHeader* header, char* fileName);
void BinaryWriter_CreateBusLineFile(BusLine** busLines, int busLinesCount, BusLineHeader* header, char* fileName);


//  Escrevem mais de um registro no final do arquivo
//  Atualizam o cabeçalho
int BinaryWriter_IncrementVehicleFile(Vehicle** vehicles, int vehiclesCount, char* fileName);
int BinaryWriter_IncrementBusLineFile(BusLine** luslines, int buslinesCount, char* fileName);

#endif
