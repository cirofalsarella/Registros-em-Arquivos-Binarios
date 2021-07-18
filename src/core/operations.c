#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../dataModel/dataModel.h"
#include "../dataModel/binaryHeaders.h"
#include "../fileIO/binaryReader.h"
#include "../fileIO/binaryWriter.h"
#include "../core/printer.h"
#include "../core/utils.h"

#define BAD_FILE_ERROR "Falha no processamento do arquivo.\n"
#define NO_REGS_ERROR "Registro inexistente.\n"


// ANCHOR: Brute-force and index based join

/**
 * @brief Reads field names from terminal. These field names are the ones used during join operations.
 * 		  Currently only line code (codLinha) is supported.
 * 
 * @return int 
 */
int ScanFieldNames() {
	char vehiclesField[64] = { "\0" };
	scanf("%s", vehiclesField);

	char busLinesField[64] = { "\0" };
	scanf("%s", busLinesField);

	if (strcmp(vehiclesField, "codLinha") != 0 || strcmp(busLinesField, "codLinha") != 0) {
		printf(NO_REGS_ERROR);
		return 0;
	}

	return 1;
}

void Op_NestedLoopJoin(const char* vehiclesFileName, const char* busLinesFileName) {
	if (!ScanFieldNames()) {
		return;
	}

	char busLineIndexFileName[128] = { "\0" };
	scanf("%s", busLineIndexFileName);

	FILE* vehiclesFile = fopen(vehiclesFileName, "rb");
	FILE* busLinesFile = fopen(busLinesFileName, "rb");

	// Checks for nonexistent files and for files with invalid status
	if (!BinaryReader_ValidateStatus(vehiclesFile) || !BinaryReader_ValidateStatus(busLinesFile)) {
		printf(BAD_FILE_ERROR);
		return;
	}

	VehicleHeader_t* vehicleHeader = BinaryReader_VehicleHeader(vehiclesFile);
	BusLineHeader_t* busLineHeader = BinaryReader_BusLineHeader(busLinesFile);

	// Checks for inconsistent headers
	if (!BinaryHeaders_IsVehicleHeaderValid(vehicleHeader) || !BinaryHeaders_IsBusLineHeaderValid(busLineHeader)) {
		printf(BAD_FILE_ERROR);
		return;
	}

	char foundAnyMatches = FALSE;

	// Goes through each vehicle...
	for (int vehicleIndex = 0; vehicleIndex < vehicleHeader->numReg; ++vehicleIndex) {
		Vehicle_t* vehicle = BinaryReader_Vehicle(vehiclesFile);
		BusLine_t* matchingBusLine = NULL;

		// Performs a linear search through the bus lines to find the matching bus line
		fseek(busLinesFile, 1, SEEK_SET);
		for (int busLineIndex = 0; busLineIndex < busLineHeader->numReg; ++busLineIndex) {
			// Reads a single bus line
			BusLine_t* busLine = BinaryReader_BusLine(busLinesFile);
			if (busLine->lineCode == vehicle->lineCode) {
				matchingBusLine = busLine;
				break;
			}
			BusLine_Free(busLine);
		}

		// Prints the joint register
		if (matchingBusLine != NULL) {
			Printer_Vehicle(vehicle);
			Printer_BusLine(matchingBusLine);
			foundAnyMatches = TRUE;
			BusLine_Free(matchingBusLine);
		}

		// Frees the vehicle
		Vehicle_Free(vehicle);
	}

	if (!foundAnyMatches) {
		printf(NO_REGS_ERROR);
	}

	fclose(vehiclesFile);
	fclose(busLinesFile);
	BinaryHeaders_FreeVehicleHeader(vehicleHeader);
	BinaryHeaders_FreeBusLineHeader(busLineHeader);
}

void Op_SingleLoopJoin(const char* vehiclesFileName, const char* busLinesFileName) {
	if (!ScanFieldNames()) {
		return;
	}

	char busLineIndexFileName[128] = { "\0" };
	scanf("%s", busLineIndexFileName);

	FILE* vehiclesFile = fopen(vehiclesFileName, "rb");

	// Checks for nonexistent files and for files with invalid status
	if (!BinaryReader_ValidateStatus(vehiclesFile)) {
		printf(BAD_FILE_ERROR);
		return;
	}

	VehicleHeader_t* vehicleHeader = BinaryReader_VehicleHeader(vehiclesFile);

	// Checks for inconsistent headers
	if (!BinaryHeaders_IsVehicleHeaderValid(vehicleHeader)) {
		printf(BAD_FILE_ERROR);
		return;
	}

	BTreeMetadata_t* busLineIndex = BTreeMetadata_Create(busLineIndexFileName, "rb", busLinesFileName, "rb");

	char foundAnyMatches = FALSE;

	// For each vehicle...
	for (int i = 0; i < vehicleHeader->numReg; ++i) {
		Vehicle_t* vehicle = BinaryReader_Vehicle(vehiclesFile);

		// If the vehicle is not removed...
		if (vehicle->removed == '1') {
			// Finds a bus line with the same line code
			RegKey_t keyToFind = vehicle->lineCode;
			BNode_t* nodeFound = BTreeMetadata_GetNodeByKey(busLineIndex, keyToFind);
			int keyIndex = BNode_GetKeyIndex(nodeFound, keyToFind);

			// If we could find a corresponding bus line...
			if (keyIndex >= 0) {
				// Reads the bus line reg
				ByteOffset_t busLineOffset = nodeFound->offsets[keyIndex];
				fseek(busLineIndex->registersFile, busLineOffset, SEEK_SET);
				BusLine_t* busLine = BinaryReader_BusLine(busLineIndex->registersFile);

				// Checks for corrupted files (something is very wrong, clean up and return)
				if (busLine->lineCode != keyToFind || busLine->removed != '0') {
					printf(BAD_FILE_ERROR);
					BNode_Free(nodeFound);
					Vehicle_Free(vehicle);
					fclose(vehiclesFile);
					BTreeMetadata_Free(busLineIndex);
					BinaryHeaders_FreeVehicleHeader(vehicleHeader);
					return;
				}

				// If everything is as expected, prints the joint register
				Printer_Vehicle(vehicle);
				Printer_BusLine(busLine);
				foundAnyMatches = TRUE;
				printf("\n");
			}

			BNode_Free(nodeFound);
		}

		Vehicle_Free(vehicle);
	}

	if (!foundAnyMatches) {
		printf(NO_REGS_ERROR);
	}

	fclose(vehiclesFile);
	BTreeMetadata_Free(busLineIndex);
	BinaryHeaders_FreeVehicleHeader(vehicleHeader);
}




// ANCHOR: Sort-merge join related operations

void Op_SortVehiclesByLineCode(const char* unorderedFile, const char* orderedFile) {
	char field[64] = { "\0" };
	scanf("%s", field);
	
	// Read unorderedFile to ram
	int n_vehicles;
	Vehicle_t** vehicles = BinaryReader_Vehicles(unorderedFile, &n_vehicles);

	// Order file acording to the especified field


	// Write file in orderedFile
	BinaryWriter_VehicleFile(vehicles, n_vehicles, orderedFile);

	for (int i = 0; i < n_vehicles; i++) {
		Vehicle_Free(vehicles[i]);
	}
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

	for (int i = 0; i < n_buslines; i++) {
		BusLine_Free(buslines[i]);
	}
	free(buslines);
}

void Op_SortMergeJoin(const char* vehicleFile, const char* buslineFile) {
	if (!ScanFieldNames()) {
		return;
	}

	// Get registers
	int n_vehicles;
	Vehicle_t** vehicles = BinaryReader_Vehicles(vehicleFile, &n_vehicles);

	int n_buslines;
	BusLine_t** buslines = BinaryReader_BusLines(buslineFile, &n_buslines);

	// Orders registers by "codLinha" field

	// Prints registers merging

	// Frees the memory
	for (int i = 0; i < n_vehicles; i++) {
		Vehicle_Free(vehicles[i]);
	}
	
	free(vehicles);
	
	for (int i = 0; i < n_buslines; i++) {
		BusLine_Free(buslines[i]);
	}
	
	free(buslines);
}