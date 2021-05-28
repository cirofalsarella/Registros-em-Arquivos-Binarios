#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "operations.h"

int main() {
    // Gets op number
    int op;
    scanf("%d", &op);

    // Depending on the requested op, executes a specific function
    switch (op) {
    case 1:
        Op_CreateTableVehicles();
        break;
    case 2:
        Op_CreateTableBuslines();
        break;
    case 3:
        Op_SelectVehicles();
        break;
    case 4:
        Op_SelectBusLines();
        break;
    case 5:
        Op_SelectVehiclesWhere();
        break;
    case 6:
        Op_SelectBuslinesWhere();
        break;
    case 7:
        Op_PushVehicles();
        break;
    case 8:
        Op_PushBuslines();
        break;
    }

    return 0;
}