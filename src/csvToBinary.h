#ifndef _CSV_TO_BINARY_H_
#define _CSV_TO_BINARY_H_

//  FUNÇÕES RESPONSÁVEIS PELA CONVERSÃO DE CSV EM BINÁRIO


// Creates a binary file from the given string table.
void CsvToBinary_WriteVehicleFile(StringTable *table, char *fileName);

void CsvToBinary_WriteBusLineFile(StringTable *table, char *fileName);


#endif