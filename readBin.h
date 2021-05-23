#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dataModel.h"

Vehicle **ReadBin_Vehicle(int *n_vehicles, char *binFile);
Vehicle **ReadBin_VehicleWhere(int *n_vehicles, char *binFile, char *argument, void *value);

BusLine **ReadBin_BusLines(int *n_buslines, char *binFile);
BusLine **ReadBin_BusLineWhere(int *n_buslines, char *binFile, char *argument, void *value);