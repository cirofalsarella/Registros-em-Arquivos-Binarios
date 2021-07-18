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
VehicleHeader_t* BinaryHeaders_CreateVehicleHeader(int nextReg, int numReg, int numRegRemov);

/**
 * @brief Returns TRUE if the vehicle header is valid.
 * 
 * @param header Header to consider.
 * @return boolean 
 */
char BinaryHeaders_IsVehicleHeaderValid(VehicleHeader_t* header);

/**
 * @brief Returns TRUE if the bus lines header is valid.
 * 
 * @param header Header to consider.
 * @return boolean 
 */
char BinaryHeaders_IsBusLineHeaderValid(BusLineHeader_t* header);

/**
 * @brief Creates a new bus line header with the given strings.
 * 
 * @param describeCode 
 * @param describeCard 
 * @param describeName 
 * @param describeLine 
 * @return The created BusLineHeader_t*
 */
BusLineHeader_t* BinaryHeaders_CreateBusLineHeader(int nextReg, int numReg, int numRegRemov);


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