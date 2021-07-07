/* * * * * * * * * * * * * * * * * * * * 
 *  11795593   Ciro Grossi Falsarella  *
 *  11795641   Pedro Martelleto        *
 * * * * * * * * * * * * * * * * * * * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "operations.h"

int main() {
    // Pega o número da Operação
    int op;
    scanf("%d", &op);

    // Seleciona qual a operação que sera realizada
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
    case 9:
        Op_CreateBTreeVehicles();
        break;
    case 10:
        Op_CreateBTreeBusLines();
        break;
    }

    return 0;
}