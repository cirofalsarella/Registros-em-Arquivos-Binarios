#ifndef _BINARY_WRITER_H_
#define _BINARY_WRITER_H_

#include "../bTree/bTreeDataModel.h"
#include "../bTree/bTree.h"
#include "../dataModel/dataModel.h"

// ANCHOR: Single Register

/**
 * @brief Writes a register in the source file.
 * 
 * @param register The register that will be written
 * @param destFile The file pointer. Assumes it is at the correct place!
 */
void BinaryWriter_Vehicle(const Vehicle_t* vehicle, FILE* destFile);
/**
 * @brief Writes a register in the source file.
 * 
 * @param register The register that will be written
 * @param destFile The file pointer. Assumes it is at the correct place!
 */
void BinaryWriter_BusLine(const BusLine_t* busLine, FILE* destFile);


// ANCHOR: Complete File

/**
 * @brief Writes a complete registers file.
 * 
 * @param registers The registers that will be written.
 * @param n_registers The number of registers.
 * @param fileName The name of the destiny file.
 */
void BinaryWriter_VehicleFile(Vehicle_t** vehicles, int n_vehicles, const char* fileName);
/**
 * @brief Writes a complete registers file.
 * 
 * @param registers The registers that will be written.
 * @param n_registers The number of registers.
 * @param fileName The name of the destiny file.
 */
void BinaryWriter_BusLineFile(BusLine_t** buslines, int n_buslines, const char* fileName);


// ANCHOR: B-Tree

/**
 * @brief Seek and Write the given node.
 * 
 * @param node The node to be written
 * @param meta The BTree metadata
 * @return The status (1 == error, 0 == success)
 */
int BinaryWriter_SeekAndWriteNode(BNode_t* node, BTreeMetadata_t* meta);

/**
 * @brief Write the header of a B-Tree in the B-Tree file.
 * 
 * @param meta The cache that contains the header and file pointer.
 */
void BinaryWriter_BTreeHeader(BTreeMetadata_t* meta);

/**
 * @brief Creates a new B-Tree index file for the vehicles.
 * 
 * @param meta The the B-Tree metadata.
 * @return The status (1 == error, 0 == success)
 */
int BinaryWriter_BTreeIndexFileVehicles(BTreeMetadata_t* meta);

/**
 * @brief Creates a new B-Tree index file for the bus lines.
 * 
 * @param meta The cache that contains the header and file pointer.
 * @return The status (1 == error, 0 == success)
 */
int BinaryWriter_BTreeIndexFileBusLines(BTreeMetadata_t* meta);

#endif
