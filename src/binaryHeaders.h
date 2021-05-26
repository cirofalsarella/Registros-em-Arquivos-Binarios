#ifndef _BINARY_HEADERS_H_
#define _BINARY_HEADERS_H_

#include "dataModel.h"
#include "stringTable.h"

/**
 * @brief Creates a new vehicle header.
 * 
 * @param describePrefix 
 * @param describeDate 
 * @param describePlaces 
 * @param describeLine 
 * @param describeModel 
 * @param describeCategory 
 * @return VehicleHeader* 
 */
VehicleHeader* VehicleHeader_Create(char* describePrefix, char* describeDate, char* describePlaces, char* describeLine, char* describeModel, char* describeCategory);

/**
 * @brief Frees the given vehicle header.
 * @param header 
 */
void VehicleHeader_Free(VehicleHeader* header);

/**
 * @brief Creates a new bus line header.
 * 
 * @param describeCode 
 * @param describeCard 
 * @param describeName 
 * @param describeLine 
 * @return BusLineHeader* 
 */
BusLineHeader* BusLineHeader_Create(char* describeCode, char* describeCard, char* describeName, char* describeLine);

/**
 * @brief Frees the given bus line header.
 * @param header
 */
void BusLineHeader_Free(BusLineHeader* header);

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