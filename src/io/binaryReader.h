#ifndef _READ_BINARY_H_
#define _READ_BINARY_H_

#include "../dataModel/dataModel.h"
#include "../bTree/bTreeDataModel.h"
#include "../bTree/bTree.h"

// ANCHOR: Read registers & reg file headers

/**
 * @brief Reads one vehicle from src file.
 * 
 * @param srcFile The file to read the vehicle from.
 * @return Vehicle_t* 
 */
Vehicle_t* BinaryReader_Vehicle(FILE* srcFile);

/**
 * @brief Reads one vehicle from src file.
 * 
 * @param srcFile The file to read the vehicle from.
 * @return BusLine_t* 
 */
BusLine_t* BinaryReader_BusLine(FILE* srcFile);


/**
 * @brief Reads vehicle header from src file. Assumes file pointer is at the correct place!
 * 
 * @param srcFile The file to read the vehicle from.
 * @return VehicleHeader_t* 
 */
VehicleHeader_t* BinaryReader_VehicleHeader(FILE *srcFile);

/**
 * @brief Reads bus line header from src file. Assumes file pointer is at the correct place!
 * 
 * @param srcFile The file to read the bus line from.
 * @return BusLineHeader_t* 
 */
BusLineHeader_t* BinaryReader_BusLineHeader(FILE *srcFile);


// ANCHOR: B-Tree header & nodes

/**
 * @brief reads the header of an B-Tree index File, if there isn't, creates an empty one
 * 
 * @param pt the index file ponter
 * @return BHeader_t* 
 */
BHeader_t* BinaryReader_BTreeHeader(FILE* bTreeFP);

/**
 * @brief Gets a node from the B-Tree cache by RRN.
 *        If the node is not cached, loads from disk.
 * 
 * @param cache
 * @param nodeRRN Must be -1 or a valid RRN.
 * 
 * @return an BNode_t*, the node or NULL if not found / rrn invalid
 */
BNode_t* BinaryReader_BTreeNode(BTreeMetadata_t* cache, RRN_t nodeRRN);



#endif