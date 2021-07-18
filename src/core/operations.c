#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../dataModel/dataModel.h"
#include "../dataModel/binaryHeaders.h"
#include "../fio/binaryReader.h"
#include "../fio/binaryWriter.h"
#include "../core/printer.h"
#include "../core/utils.h"

void Op_NestedLoopJoin(const char* vehicleFile, const char* buslineFile) {

}

void Op_SingleLoopJoin(const char* vehicleFile, const char* buslineFile) {

}


void Op_SortVehiclesByLineCode(const char* unorderedFile, const char* orderedFile) {
	char field[64] = { "\0" };
	scanf("%s", field);
	
	// Read unorderedFile to ram
	int n_vehicles;
	Vehicle_t** vehicles = BinaryReader_Vehicles(unorderedFile, &n_vehicles);

	// Order file acording to the especified field


	// Write file in orderedFile
	BinaryWriter_VehicleFile(vehicles, n_vehicles, orderedFile);

	for (int i=0; i<n_vehicles; i++)	Vehicle_Free(vehicles[i]);
	free(vehicles);
}

void Op_SortBusLinesByLineCode(const char* unorderedFile, const char* orderedFile) {
	char field[64] = { "\0" };
	scanf("%s", field);
	
	// Read unorderedFile to ram
	int n_buslines;
	BusLine_t** buslines = BinaryReader_BusLines(unorderedFile, &n_buslines);

	// Order file acording to the especified field


	// Write file in orderedFile
	BinaryWriter_BusLineFile(buslines, n_buslines, orderedFile);

	for (int i=0; i<n_buslines; i++)	BusLine_Free(buslines[i]);
	free(buslines);
}

void Op_SortMergeJoin(const char* vehicleFile, const char* buslineFile) {
	// Get the fields
	char fieldVehicles[64] = { "\0" };
	scanf("%s", fieldVehicles);

	char fieldBusLines[64] = { "\0" };
	scanf("%s", fieldBusLines);


	// Get registers
	int n_vehicles;
	Vehicle_t** vehicles = BinaryReader_Vehicles(vehicleFile, &n_vehicles);

	int n_buslines;
	BusLine_t** buslines = BinaryReader_BusLines(buslineFile, &n_buslines);

	// Order registers by "codLinha" field

	// Print registers merging

	// Free the memory
	for (int i=0; i<n_vehicles; i++)	Vehicle_Free(vehicles[i]);
	free(vehicles);
	for (int i=0; i<n_buslines; i++)	BusLine_Free(buslines[i]);
	free(buslines);
}