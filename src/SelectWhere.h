#ifndef _SELECT_WHERE_H_
#define _SELECT_WHERE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dataModel.h"

typedef int (*SelectWhereFnType)(void*, Vehicle*, BusLine*);

SelectWhereFnType SelectWhere_SetCondition(char *fieldName);
void *SelectWhere_SetPattern(char *fieldName);

BusLine **SelectWhere_SelectBusLines(SelectWhereFnType shouldSelect, void *pattern, BusLine **busLines, int nReg, int *nSelectedReg);
Vehicle **SelectWhere_SelectVehicles(SelectWhereFnType shouldSelect, void *pattern, Vehicle **vehicles, int nReg, int *nSelectedReg);

#endif