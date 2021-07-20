#ifndef _READ_BINARY_H_
#define _READ_BINARY_H_

#include "../dataModel/dataModel.h"
#include "../bTree/bTreeDataModel.h"
#include "../bTree/bTree.h"

/**
 * @brief Checks if the file does not exist or has an invalid status (is corrupted).
 * 
 * @param file file to check.
 * @return Returns TRUE if the status is OK ('1'). FALSE otherwise. 
 */
char BinaryReader_ValidateStatus(FILE* file);


//ANCHOR: Single Register

/**
 * @brief Read register from source file.
 * 
 * @param srcFile The file pointer. Assumes is at the correct place!
 * @return The read register.
 */
Vehicle_t* BinaryReader_Vehicle(FILE* srcFile);
/**
 * @brief Read register from source file.
 * 
 * @param srcFile The file pointer. Assumes is at the correct place!
 * @return The read register.
 */
BusLine_t* BinaryReader_BusLine(FILE* srcFile);


//ANCHOR: Header

/**
 * @brief Read file header from source file. Ignores removed registers.
 * 
 * @param srcFile The file pointer. Assumes is at the correct place!
 * @return The read header.
 */
VehicleHeader_t* BinaryReader_VehicleHeader(FILE *srcFile);
/**
 * @brief Read file header from source file. Ignores removed registers.
 * 
 * @param srcFile The file pointer. Assumes is at the correct place!
 * @return The read header.
 */
BusLineHeader_t* BinaryReader_BusLineHeader(FILE *srcFile);


//ANCHOR: Registers from file

/**
 * @brief Read the registers from an given file.
 * 
 * @param fileName The name of the file.
 * @param n_registers The number of registers in the file
 * @return The read registers.
 */
Vehicle_t** BinaryReader_Vehicles(const char* fileName, int* n_vehicles);
/**
 * @brief Read the registers from an given file.
 * 
 * @param fileName The name of the file.
 * @param n_registers The number of registers in the file
 * @return The read registers.
 */
BusLine_t** BinaryReader_BusLines(const char* fileName, int* n_buslines);


// ANCHOR: B-Tree

/**
 * @brief Read the header of an B-Tree index File.
 * 
 * @param pt The index file ponter.
 * @return  The read header.
 *          If none exists, return an empty one.
 */
BHeader_t* BinaryReader_BTreeHeader(FILE* bTreeFP);

/**
 * @brief Read a node from the B-Tree by RRN.
 * 
 * @param cache The B-Tree metadata.
 * @param nodeRRN The node RRN.
 *                Must be -1 or a valid RRN.
 * 
 * @return The read node.
 *         NULL if not found / rrn invalid.
 */
BNode_t* BinaryReader_BTreeNode(BTreeMetadata_t* cache, RRN_t nodeRRN);



#endif