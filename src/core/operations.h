#ifndef _OPERATIONS_H_
#define _OPERATIONS_H_

/**
 * Operations 9 & 10
 *      Create a B-Tree index file.
 */
void Op_CreateBTreeVehicles(char* regsFileName, char* bTreeFileName);
void Op_CreateBTreeBusLines(char* regsFileName, char* bTreeFileName);

/**
 * Operations 11 & 12
 *      Queries for registers with the given keys.
 */
void Op_FindVehicle(char* regsFileName, char* bTreeFileName, char* fieldName, char* prefix);
void Op_FindBusLine(char* regsFileName, char* bTreeFileName, char* fieldName, char* lineCodeStr);

/**
 * Operations 13 & 14
 *      Pushes a list of new vehicles/bus lines to the files (b-tree and register).
 */
void Op_PushVehicles(char* regsFileName, char* bTreeFileName);
void Op_PushBusLines(char* regsFileName, char* bTreeFileName);

#endif