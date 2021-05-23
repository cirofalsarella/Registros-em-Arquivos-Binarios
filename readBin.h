#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dataModel.h"

Vehicle **ReadBin_Vehicle(int *n_vehicles, char *fileName);
Vehicle **ReadBin_VehicleWhere(int *n_vehicles, char *fileName, char *argument, void *value);

BusLine **ReadBin_BusLines(int *n_buslines, char *fileName);
BusLine **ReadBin_BusLineWhere(int *n_buslines, char *fileName, char *argument, void *value);