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
    char regsFileName[128] = { '\0' };
    scanf("%s", &regsFileName[0]);

    char bTreeFileName[128] = { '\0' };
    scanf("%s", &bTreeFileName[0]);
    
    // Expects "prefixo" or "codLinha", depending on the operation
	char fieldName[128] = { '\0' };

    // Either a prefix or lineCode
	char fieldValue[128] = { '\0' };

    // Chooses the right operation
    switch (op) {
    case 9:
        Op_CreateBTreeVehicles(regsFileName, bTreeFileName);
        break;
    case 10:
        Op_CreateBTreeBusLines(regsFileName, bTreeFileName);
        break;
    case 11:
        scanf("%s", &fieldName[0]);
        Utils_ScanQuoteString(&fieldValue[0]);
        Op_FindVehicle(regsFileName, bTreeFileName, fieldName, fieldValue);
        break;
    case 12:
        scanf("%s", &fieldName[0]);
        scanf("%s", &fieldValue[0]);
        Op_FindBusLine(regsFileName, bTreeFileName, fieldName, fieldValue);
        break;
    case 13:
        Op_PushVehicles(regsFileName, bTreeFileName);
        break;
    case 14:
        Op_PushBusLines(regsFileName, bTreeFileName);
        break;
    }

    return 0;
}