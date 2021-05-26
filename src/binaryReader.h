#ifndef _READ_BINARY_H_
#define _READ_BINARY_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dataModel.h"

/**
 * @brief Reads a binary file containing a list of vehicles. Returns that list.
 * 
 * @param header This pointer will receive the header of the file.
 * @param fileName The name of the file.
 * @return Vehicle** 
 */
Vehicle** BinaryReader_Vehicles(VehicleHeader** header, char *fileName);

/**
 * @brief Reads a binary file containing a list of bus lines. Returns that list.
 * 
 * @param header This pointer will receive the header of the file.
 * @param fileName The name of the file.
 * @return BusLine** 
 */
BusLine** BinaryReader_BusLines(BusLineHeader** header, char *fileName);

#endif