#ifndef _READ_BINARY_H_
#define _READ_BINARY_H_

#include "dataModel.h"
#include "bTreeDataModel.h"
#include "bTree.h"
// FUNÇÕES RESPONSÁVEIS PELA LEITURA DE ARQUIVOS BINÁRIOS

// Arquivos de Registros

/**
 * @brief Reads the entire binary file containing vehicle information.
 * 
 * @param header Pointer to the variable that will store the header.
 * @param fileName Name of the file.
 * @return Vehicle_t** 
 */
Vehicle_t** BinaryReader_Vehicles(VehicleHeader_t** header, char *fileName);

/**
 * @brief Reads the entire binary file containing bus line information.
 * 
 * @param header Pointer to the variable that will store the header.
 * @param fileName Name of the file.
 * @return BusLine_t** 
 */
BusLine_t** BinaryReader_BusLines(BusLineHeader_t** header, char *fileName);


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
 * @return BusLine_t* 
 */
BusLine_t* BinaryReader_BusLineHeader(FILE *srcFile);



// Arvore-B
    
BHeader_t* BinaryReader_BTreeHeaderAndRoot(BTreeMetadata_t* cache, char* fileName);

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