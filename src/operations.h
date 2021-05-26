#ifndef _OPERATIONS_H_
#define _OPERATIONS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stringTable.h"
#include "csvToBinary.h"
#include "printer.h"
#include "readBinary.h"
#include "selectWhere.h"
#include "funcao-fornecida.h"


/**
 * @brief Creates a binary file containing vehicles from a csv. (OP 1)
 */
void Op_CreateTableVehicles();

/**
 * @brief Creates a binary file containing bus lines from a csv.  (OP 2)
 */
void Op_CreateTableBuslines();



/**
 * @brief Prints all vehicles in the file. (OP 3)
 */
void Op_SelectVehicles();

/**
 * @brief Prints all bus lines in the file. (OP 4)
 */
void Op_SelectBusLines();



/**
 * 
 * @brief Prints all vehicles such that a specific field equals to some value given by the user. (OP 5)
 */
void Op_SelectVehiclesWhere();

/**
 * @brief Prints all bus lines such that a specific field equals to some value given by the user. (OP 6)
 */
void Op_SelectBuslinesWhere();



/**
 * @brief Pushes a set of user-defined vehicles to the binary file. (OP 7)
 */
void Op_PushVehicles();

/**
 * @brief Pushes a set of user-defined bus lines to the binary file. (OP 8)
 */
void Op_PushBuslines();



#endif