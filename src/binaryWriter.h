
#ifndef _BINARY_WRITER_H_
#define _BINARY_WRITER_H_

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "dataModel.h"

/**
 * @brief Writes a vehicle to the given file.
 * 
 * @param vehicle 
 * @param destFile 
 */
void BinaryWriter_WriteVehicle(const Vehicle* vehicle, FILE* destFile);

/**
 * @brief Writes a bus line to the given file.
 * 
 * @param busLine 
 * @param destFile 
 */
void BinaryWriter_WriteBusLine(const BusLine* busLine, FILE* destFile);

/**
 * @brief Writes the vehicles header to the given file.
 * 
 * @param header 
 * @param destFile 
 */
void BinaryWriter_WriteVehicleHeader(const VehicleHeader* header, FILE *destFile);

/**
 * @brief Writes the bus lines header to the given file.
 * 
 * @param header 
 * @param destFile 
 */
void BinaryWriter_WriteBusLineHeader(const BusLineHeader* header, FILE *destFile);

#endif
