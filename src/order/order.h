#ifndef _gama_Order_H_
#define _gama_Order_H_

#include "../dataModel/dataModel.h"

void Order_Vehicles(Vehicle_t **vehicles, int nReg, int end);
void Order_BusLines(BusLine_t **buslines, int nReg, int end);

#endif