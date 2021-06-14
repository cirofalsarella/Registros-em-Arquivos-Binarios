#ifndef _BINARY_WRITER_H_
#define _BINARY_WRITER_H_


//  FUNÇÕES RESPONSÁVEIS POR ESCREVER ARQUIVOS BINÁRIOS

//  Escrevem um arquivo inteiro
void BinaryWriter_CreateVehicleFile(Vehicle_t** vehicles, int vehiclesCount, VehicleHeader_t* header, char* fileName);
void BinaryWriter_CreateBusLineFile(BusLine_t** busLines, int busLinesCount, BusLineHeader_t* header, char* fileName);


//  Escrevem mais de um registro no final do arquivo
//  Atualizam o cabeçalho
int BinaryWriter_IncrementVehicleFile(Vehicle_t** vehicles, int vehiclesCount, char* fileName);
int BinaryWriter_IncrementBusLineFile(BusLine_t** luslines, int buslinesCount, char* fileName);

#endif
