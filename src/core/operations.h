#ifndef _OPERATIONS_H_
#define _OPERATIONS_H_

/**
 * @brief Operation 15. Brute-force join operation. Pretty prints the resulting (joined) registers. O(a * b),
 *        where a = # number of vehicles and b = # number of bus lines.
 * 
 * @param vehicleFile Vehicles file to get registers from.
 * @param buslineFile Bus lines file to get registers from.
 */
void Op_NestedLoopJoin(const char* vehicleFile, const char* buslineFile);

/**
 * @brief Operation 16. For each vehicle, performs a B-Tree index search for the corresponding bus line. O(a log b), where
 *        a = # of vehicles, b = # number of bus lines.
 * 
 * @param vehicleFile Vehicles file to get registers from.
 * @param buslineFile Bus lines file to get registers from.
 */
void Op_SingleLoopJoin(const char* vehicleFile, const char* buslineFile);

/**
 * @brief Operation 17. Sorts a vehicles file by line code. O(n log n).
 * 
 * @param unorderedFile Source file.
 * @param orderedFile   Dest file.
 */
int Op_SortVehiclesByLineCode(const char* unorderedFile, const char* orderedFile);

/**
 * @brief Operation 18. Sorts a bus lines file by line code. O(n log n).
 * 
 * @param unorderedFile Source file.
 * @param orderedFile   Dest file.
 */
int Op_SortBusLinesByLineCode(const char* unorderedFile, const char* orderedFile);

/**
 * @brief Operation 19. Performs a linear search simultaneously on both (sorted) files.
 *        O(n), where n = Max(# of vehicles, # of bus lines)
 * 
 * @param vehicleFile Path to the vehicles file.
 * @param buslineFile Path to the bus lines file.
 */
int Op_SortMergeJoin(const char* vehicleFile, const char* buslineFile);

#endif