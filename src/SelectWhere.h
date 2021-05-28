#ifndef _SELECT_WHERE_H_
#define _SELECT_WHERE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dataModel.h"

void *SelectWhere_SetCondition(char *fieldName);
void *SelectWhere_SetPattern(char *fieldName);

BusLine **SelectWhere_SelectBusLines(void *functionPt(void*, BusLine*), void *pattern, BusLine **busLines, int nReg, int *nSelectedReg);
Vehicle **SelectWhere_SelectVehicles(void *functionPt(void*, Vehicle*), void *pattern, Vehicle **vehicles, int nReg, int *nSelectedReg);

#endif