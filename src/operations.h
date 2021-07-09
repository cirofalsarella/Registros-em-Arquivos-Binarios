#ifndef _OPERATIONS_H_
#define _OPERATIONS_H_

/**
 * Operations 9 & 10
 *      Create a B-Tree index file.
 */
void Op_CreateBTreeVehicles();
void Op_CreateBTreeBusLines();

/**
 * Operations 11 & 12
 *      Queries for registers with the given keys.
 */
void Op_FindVehicle();
void Op_FindBusLine();

/**
 * Operations 13 & 14
 *      Pushes a list of new vehicles/bus lines to the files (b-tree and register).
 */
void Op_PushVehicles();
void Op_PushBusLines();

#endif