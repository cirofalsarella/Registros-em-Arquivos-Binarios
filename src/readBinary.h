#ifndef _READ_BINARY_H_
#define _READ_BINARY_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dataModel.h"

/**
 * @brief Reads a binary file containing a list of vehicles. Returns that list.
 * 
 * @param n_vehicles Reference to the int that will hold the number of vehicles.
 * @param fileName The name of the file.
 * @return Vehicle** 
 */
Vehicle** ReadBinary_Vehicle(int *n_vehicles, char *fileName);

/**
 * @brief Reads a binary file containing a list of bus lines. Returns that list.
 * 
 * @param n_vehicles Reference to the int that will hold the number of bus lines.
 * @param fileName The name of the file.
 * @return BusLine** 
 */
BusLine** ReadBinary_BusLines(int *n_buslines, char *fileName);

#endif