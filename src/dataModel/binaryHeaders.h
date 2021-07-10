#ifndef _BINARY_HEADERS_H_
#define _BINARY_HEADERS_H_

#include "dataModel.h"

// ANCHOR: Create functions

/**
 * @brief Creates a new vehicle header with the given strings.
 * 
 * @param describePrefix 
 * @param describeDate 
 * @param describePlaces 
 * @param describeLine 
 * @param describeModel 
 * @param describeCategory 
 * @return The created VehicleHeader_t*
 */
VehicleHeader_t* BinaryHeaders_CreateVehicleHeader(char* describePrefix, char* describeDate, char* describePlaces, char* describeLine, char* describeModel, char* describeCategory);

/**
 * @brief Creates a new bus line header with the given strings.
 * 
 * @param describeCode 
 * @param describeCard 
 * @param describeName 
 * @param describeLine 
 * @return The created BusLineHeader_t*
 */
BusLineHeader_t* BinaryHeaders_CreateBusLineHeader(char* describeCode, char* describeCard, char* describeName, char* describeLine);


// ANCHOR: Free functions

/**
 * @brief Frees the vehicles header.
 * 
 * @param header The structure to be freed
 */
void BinaryHeaders_FreeVehicleHeader(VehicleHeader_t* header);

/**
 * @brief Frees the bus line header.
 * 
 * @param header The structure to be freed
 */
void BinaryHeaders_FreeBusLineHeader(BusLineHeader_t* header);

#endif