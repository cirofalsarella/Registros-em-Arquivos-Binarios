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
    case 9:
        Op_CreateBTreeVehicles();
        break;
    case 10:
        Op_CreateBTreeBusLines();
        break;
    }

    return 0;
}