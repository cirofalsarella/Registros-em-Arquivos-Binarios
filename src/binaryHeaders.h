#ifndef _BINARY_HEADERS_H_
#define _BINARY_HEADERS_H_

#include "dataModel.h"
#include "stringTable.h"

/**
 * @brief Creates a vehicle header from the given string table.
 * 
 * @param table 
 * @return VehicleHeader* 
 */
VehicleHeader* VehicleHeader_CreateFromTable(StringTable* table);

/**
 * @brief Creates a bus line header from the given string table.
 * 
 * @param table 
 * @return BusLineHeader* 
 */
BusLineHeader* BusLineHeader_CreateFromTable(StringTable *table);

#endif