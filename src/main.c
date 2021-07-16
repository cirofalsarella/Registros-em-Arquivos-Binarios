/* * * * * * * * * * * * * * * * * * * * 
 *  11795593   Ciro Grossi Falsarella  *
 *  11795641   Pedro Martelleto        *
 * * * * * * * * * * * * * * * * * * * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "core/utils.h"
#include "core/operations.h"

int main() {
    // Gets op number
    int op;
    scanf("%d", &op);

    // Gets file names from terminal
    char file_1[128] = { '\0' };
    scanf("%s", file_1);

    char file_2[128] = { '\0' };
    scanf("%s", file_2);

    // Chooses the right operation
    switch (op) {
    case 15:
        Op_15(file_1, file_2);
        break;
    case 16:
        Op_16(file_1, file_2);
        break;
    case 17:
        Op_17(file_1, file_2);
        break;
    case 18:
        Op_18(file_1, file_2);
        break;
    case 19:
        Op_19(file_1, file_2);
        break;
    }

    return 0;
}