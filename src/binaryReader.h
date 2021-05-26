#ifndef _READ_BINARY_H_
#define _READ_BINARY_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dataModel.h"

/**
 * @brief Reads a binary file containing a list of vehicles. Returns that list.
 * 
 * @param vehiclesCount Reference to the int that will hold the number of vehicles.
 * @param fileName The name of the file.
 * @return Vehicle** 
 */
Vehicle** BinaryReader_Vehicles(int *vehiclesCount, char *fileName);

/**
 * @brief Reads a binary file containing a list of bus lines. Returns that list.
 * 
 * @param busLinesCount Reference to the int that will hold the number of bus lines.
 * @param fileName The name of the file.
 * @return BusLine** 
 */
BusLine** BinaryReader_BusLines(int *busLinesCount, char *fileName);

#endif