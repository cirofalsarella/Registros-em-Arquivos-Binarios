#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dataModel.h"

void *SelectWhere_setCondition(char *fieldName);
void *SelectWhere_setPattern(char *fieldName);

BusLine **SelectWhere_selectBusLines(void *functionPt(void*, BusLine*), void *pattern, BusLine ***busLines, int *n_busLines);
Vehicle **SelectWhere_selectVehicles(void *functionPt(void*, Vehicle*), void *pattern, Vehicle ***vehicles, int *n_vehicles);
