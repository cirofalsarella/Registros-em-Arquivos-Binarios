#ifndef _CSV_TO_BINARY_H_
#define _CSV_TO_BINARY_H_

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "stringTable.h"
#include "dataModel.h"

/**
 * @brief Converts a string table row to our binary representation of a vehicle. Free the object returned.
 * 
 * @param table 
 * @param row 
 */
Vehicle* CsvToBinary_CreateVehicleFromRow(StringTable* table, int row);

/**
 * @brief Converts a string table row to our binary representation of a bus line. Free the object returned.
 * 
 * @param table 
 * @param row 
 */
BusLine* CsvToBinary_CreateBusLineFromRow(StringTable* table, int row);

/**
 * @brief Creates a vehicle file from the givens tring table.
 * 
 * @param table 
 * @param fileName 
 */
void CsvToBinary_WriteVehicleFile(StringTable *table, char *fileName);

/**
 * @brief Creates a bus line file from the given string table.
 * 
 * @param table 
 * @param fileName 
 */
void CsvToBinary_WriteBusLineFile(StringTable *table, char *fileName);

#endif