/* * * * * * * * * * * * * * * * * * * * 
 *  11795593   Ciro Grossi Falsarella  *
 *  11795641   Pedro Martelleto        *
 * * * * * * * * * * * * * * * * * * * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "operations.h"

int main() {
    // Gets op number
    int op;
    scanf("%d", &op);

    // Chooses the right operation
    switch (op) {
    case 9:
        Op_CreateBTreeVehicles();
        break;
    case 10:
        Op_CreateBTreeBusLines();
        break;
    case 11:
        Op_FindVehicle();
        break;
    case 12:
        Op_FindBusLine();
        break;
    case 13:
        Op_PushVehicles();
        break;
    case 14:
        Op_PushBusLines();
        break;
    }

    return 0;
}