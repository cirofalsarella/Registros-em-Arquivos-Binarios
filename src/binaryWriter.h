#ifndef _BINARY_WRITER_H_
#define _BINARY_WRITER_H_

#include "bTreeDataModel.h"
#include "bTree.h"


//  FUNÇÕES RESPONSÁVEIS POR ESCREVER ARQUIVOS BINÁRIOS

/**
 * @brief Creates a vehicle binary file with the given vehicles.
 * 
 * @param vehicles vector of vehicles that will be writen
 * @param vehiclesCount number of vehicles to be writen
 * @param header header of the file
 * @param fileName name of the file
 */
void BinaryWriter_CreateVehicleFile(Vehicle_t** vehicles, int vehiclesCount, VehicleHeader_t* header, char* fileName);

/**
 * @brief Creates a bus line binary file with the given bus lines.
 * 
 * @param BusLine vector of BusLine that will be writen
 * @param BusLineCount number of BusLine to be writen
 * @param header header of the file
 * @param fileName name of the file
 */
void BinaryWriter_CreateBusLineFile(BusLine_t** busLines, int busLinesCount, BusLineHeader_t* header, char* fileName);


/**
 * @brief Writes the header of an BTree file
 * 
 * @param cache is the Btree to be wroten
 */
void BinaryWriter_BTreeHeader(BTreeCache_t* cache);


/**
 * @brief Writes a vector of vehicles to the end of dest file.
 * 
 * @param vehicles vector of vehicles that will be writen
 * @param vehiclesCount number of vehicles to be writen
 * @param fileName name of the file
 * 
 * @return int 
 */
int BinaryWriter_IncrementVehicleFile(Vehicle_t** vehicles, int vehiclesCount, char* fileName);

/**
 * @brief Writes a vector of bus lines to the end of dest file.
 * 
 * @param busLines 
 * @param buslinesCount 
 * @param fileName 
 *
 * @return 1 == error, 0 == success
 */
int BinaryWriter_IncrementBusLineFile(BusLine_t** busLines, int buslinesCount, char* fileName);

/**
 * @brief
 * 
 * @param BusLine vector of BusLine that will be writen
 * @param BusLineCount number of BusLine to be writen
 * @param header header of the file
 * @param fileName name of the file
 * 
 * @return 1 == error, 0 == success
 */
int BinaryWriter_IncrementBtree(BNode_t* node, BTreeCache_t* cache);


#endif
