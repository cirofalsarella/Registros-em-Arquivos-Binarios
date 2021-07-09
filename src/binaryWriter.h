#ifndef _BINARY_WRITER_H_
#define _BINARY_WRITER_H_

#include "bTreeDataModel.h"
#include "bTree.h"
#include "dataModel.h"

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
 * @param vehicles vector of vehicles that will be writen.
 * @param vehiclesCount number of vehicles to be writen.
 * @param file the registers file.
 * @param destOffsets array to store the offsets of the inserted vehicles.
 * 
 * @return 1 == error, 0 == success
 */
int BinaryWriter_AppendVehicles(Vehicle_t** vehicles, int vehiclesCount, FILE* file, ByteOffset_t* destOffsets);

/**
 * @brief Writes a vector of bus lines to the end of dest file.
 * 
 * @param busLines vector of vehicles that will be written.
 * @param buslinesCount number of vehicles to be written.
 * @param file the registers file.
 * @param destOffsets array to store the offsets of the inserted bus lines.
 * 
 * @return 1 == error, 0 == success
 */
int BinaryWriter_AppendBusLines(BusLine_t** busLines, int buslinesCount, FILE* file, ByteOffset_t* destOffsets);



// MARK: B-Tree related writing


/**
 * @brief Seeks to the right place in the file and writes the given BNode_t.
 * 
 * @param node
 * @param meta
 * @return 1 == error, 0 == success
 */
int BinaryWriter_SeekAndWriteNode(BNode_t* node, BTreeMetadata_t* meta);

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
