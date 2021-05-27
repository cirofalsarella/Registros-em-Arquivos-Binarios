
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

/**
 * @brief Creates the entire binary file with the vehicles data. Only the "describe" values need to be set in the header, everything else is calculated during the function.
 * 
 * @param vehicles 
 * @param vehiclesCount 
 * @param header 
 * @param fileName 
 */
void BinaryWriter_CreateVehicleFile(Vehicle** vehicles, int vehiclesCount, VehicleHeader* header, char* fileName);

/**
 * @brief Creates the entire binary file with the bus lines data. Only the "describe" values need to be set in the header, everything else is calculated during the function.
 * 
 * @param vehicles 
 * @param vehiclesCount 
 * @param header 
 * @param fileName 
 */
void BinaryWriter_CreateBusLineFile(BusLine** busLines, int busLinesCount, BusLineHeader* header, char* fileName);


void BinaryWriter_IncrementVehicleFile(Vehicle** vehicles, int vehiclesCount, char* fileName);
void BinaryWriter_IncrementBusLineFile(BusLine** luslines, int buslinesCount, char* fileName);

#endif
