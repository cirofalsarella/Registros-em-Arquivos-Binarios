#ifndef _BINARY_WRITER_H_
#define _BINARY_WRITER_H_

#include "bTreeDataModel.h"
#include "bTree.h"

// MARK: Register writing functions

/**
 * @brief Write a vehicle to the given file.
 * 
 * @param vehicle 
 * @param destFile 
 */
void BinaryWriter_Vehicle(const Vehicle_t* vehicle, FILE* destFile);

/**
 * @brief Writes a bus line to the given file.
 * 
 * @param busLine 
 * @param destFile 
 */
void BinaryWriter_BusLine(const BusLine_t* busLine, FILE* destFile);



// MARK: Appending functions

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
 * @param busLines vector of vehicles that will be written
 * @param buslinesCount number of vehicles to be written
 * @param fileName name of the file
 *
 * @return 1 == error, 0 == success
 */
int BinaryWriter_IncrementBusLineFile(BusLine_t** busLines, int buslinesCount, char* fileName);



// MARK: B-Tree related writing


/**
 * @brief Increments a B-Tree index with the given nodes.
 * 
 * @param node
 * @param meta
 * @return 1 == error, 0 == success
 */
int BinaryWriter_IncrementBTree(BNode_t* node, BTreeMetadata_t* meta);

/**
 * @brief Writes the header of a BTree in the B-Tree file.
 * 
 * @param meta The cache that contains the header and file pointer.
 */
void BinaryWriter_BTreeHeader(BTreeMetadata_t* meta);

/**
 * @brief Creates a new B-Tree index file for the vehicles.
 * 
 * @param meta 
 */
void BinaryWriter_BTreeIndexFileVehicles(BTreeMetadata_t* meta);

/**
 * @brief Creates a new B-Tree index file for the bus lines.
 * 
 * @param meta 
 */
void BinaryWriter_BTreeIndexFileBusLines(BTreeMetadata_t* meta);

#endif
