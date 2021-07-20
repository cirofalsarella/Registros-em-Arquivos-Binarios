#ifndef _gama_Order_H_
#define _gama_Order_H_

#include "../dataModel/dataModel.h"

// ANCHOR:  RAM

/**
 * @brief Merge Sort to order in ram the vehicles by the field codLinha
 * 
 * @param vehicles The vehicles that will be sorted
 * @param ini The begin of the array (initialy is 0)
 * @param end The end of the array (initialy is nReg - 1)
 */
void Order_Vehicles(Vehicle_t **vehicles, int ini, int end);

/**
 * @brief Merge Sort to order in ram the buslines by the field codLinha
 * 
 * @param buslines The buslines that will be sorted
 * @param ini The begin of the array (initialy is 0)
 * @param end The end of the array (initialy is nReg - 1)
 */
void Order_BusLines(BusLine_t **buslines, int ini, int end);

#endif