#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "operations.h"

int main() {
    int op;
    scanf("%d", &op);

    switch (op) {
    case 1: CreateTableVehicles();
        break;    
    case 2: CreateTableBuslines();
        break;
    case 3: SelectVehicles();
        break;
    case 4: SelectBusLines();
        break;
    case 5: SelectVehiclesWhere();
        break;
    case 6: SelectBuslinesWhere();
        break;
    case 7: InsertIntoVehicles();
        break;
    case 8: InsertIntoBuslines();
        break;
    }

    return 0;
}