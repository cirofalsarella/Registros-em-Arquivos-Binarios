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
#include "../order/order.h"

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

	FILE* vehiclesFile = fopen(vehiclesFileName, "rb");
	if (!BinaryReader_ValidateStatus(vehiclesFile)) {
		if (vehiclesFile != NULL) fclose(vehiclesFile);
		printf(BAD_FILE_ERROR);
		return;
	}

	FILE* busLinesFile = fopen(busLinesFileName, "rb");
	if (!BinaryReader_ValidateStatus(busLinesFile)) {
		if (vehiclesFile != NULL) fclose(vehiclesFile);
		if (busLinesFile != NULL) fclose(busLinesFile);
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
	for (int vehicleIndex = 0; vehicleIndex < vehicleHeader->validRegCount + vehicleHeader->removedRegCount; ++vehicleIndex) {
		Vehicle_t* vehicle = BinaryReader_Vehicle(vehiclesFile);
		BusLine_t* matchingBusLine = NULL;

		if (vehicle->removed == '1') {
			// Performs a linear search through the bus lines to find the matching bus line
			fseek(busLinesFile, BUSLINE_HEADER_SIZE, SEEK_SET);
			for (int busLineIndex = 0; busLineIndex < busLineHeader->validRegCount + busLineHeader->removedRegCount; ++busLineIndex) {
				// Reads a single bus line
				BusLine_t* busLine = BinaryReader_BusLine(busLinesFile);
				if (busLine->removed == '1' && busLine->lineCode == vehicle->lineCode) {
					matchingBusLine = busLine;
					break;
				}
				BusLine_Free(busLine);
			}

			// Prints the joint register
			if (matchingBusLine != NULL) {
				Printer_Vehicle(vehicle);
				Printer_BusLine(matchingBusLine);
				printf("\n");
				foundAnyMatches = TRUE;
				BusLine_Free(matchingBusLine);
			}
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

/**
 * @brief Helper function called during single loop join that cleanups and prints error during early exits.
 */
void CleanupWithError(BNode_t* nodeFound, Vehicle_t* vehicle, BusLine_t* busLine, FILE* vehiclesFile, BTreeMetadata_t* busLineIndex, VehicleHeader_t* vehicleHeader) {
	printf(BAD_FILE_ERROR);
	BNode_Free(nodeFound);
	Vehicle_Free(vehicle);
	BusLine_Free(busLine);
	if (vehiclesFile != NULL) fclose(vehiclesFile);
	BTreeMetadata_Free(busLineIndex);
	BinaryHeaders_FreeVehicleHeader(vehicleHeader);
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
		if (vehiclesFile != NULL) fclose(vehiclesFile);
		printf(BAD_FILE_ERROR);
		return;
	}

	VehicleHeader_t* vehicleHeader = BinaryReader_VehicleHeader(vehiclesFile);

	// Checks for inconsistent headers
	if (!BinaryHeaders_IsVehicleHeaderValid(vehicleHeader)) {
		if (vehiclesFile != NULL) fclose(vehiclesFile);
		printf(BAD_FILE_ERROR);
		return;
	}

	BTreeMetadata_t* busLineIndex = BTreeMetadata_Create(busLineIndexFileName, "rb", busLinesFileName, "rb");

	if (busLineIndex->registersFile == NULL || busLineIndex->bTreeIndexFile == NULL) {
		printf(BAD_FILE_ERROR);
		BTreeMetadata_Free(busLineIndex);
		BinaryHeaders_FreeVehicleHeader(vehicleHeader);
		fclose(vehiclesFile);
		return;
	}

	// Gets bus line file size
	fseek(busLineIndex->registersFile, 0, SEEK_END);
	ByteOffset_t busLineFileSize = ftell(busLineIndex->registersFile);
	fseek(busLineIndex->registersFile, 1, SEEK_SET);

	char foundAnyMatches = FALSE;

	// For each vehicle...
	for (int i = 0; i < vehicleHeader->validRegCount + vehicleHeader->removedRegCount; ++i) {
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

				if (busLineOffset < 0 || busLineOffset >= busLineFileSize) {
					CleanupWithError(nodeFound, vehicle, NULL, vehiclesFile, busLineIndex, vehicleHeader);
					return;
				}

				if (fseek(busLineIndex->registersFile, busLineOffset, SEEK_SET) != 0) {
					CleanupWithError(nodeFound, vehicle, NULL, vehiclesFile, busLineIndex, vehicleHeader);
					return;
				}

				BusLine_t* busLine = BinaryReader_BusLine(busLineIndex->registersFile);

				// Checks for corrupted files (something is very wrong, clean up and return)
				if (busLine->lineCode != keyToFind || busLine->removed != '1') {
					CleanupWithError(nodeFound, vehicle, busLine, vehiclesFile, busLineIndex, vehicleHeader);
					return;
				}

				// If everything is as expected, prints the joint register
				Printer_Vehicle(vehicle);
				Printer_BusLine(busLine);
				BusLine_Free(busLine);
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

int Op_SortVehiclesByLineCode(const char* unorderedFile, const char* orderedFile) {
	char field[64] = { "\0" };
	scanf("%s", field);
	if (strcmp(field, "codLinha") != 0) {
		printf(BAD_FILE_ERROR);
		return 1;
	}

	// Read unorderedFile to ram
	int n_vehicles;
	Vehicle_t** vehicles = BinaryReader_Vehicles(unorderedFile, &n_vehicles);
	if (vehicles == NULL || n_vehicles < 0) {
		printf(BAD_FILE_ERROR);
		return 1;
	}

	// Write ordered registers
	if (n_vehicles > 0) {
		Order_Vehicles(vehicles, 0, n_vehicles-1);
	}
	BinaryWriter_VehicleFile(vehicles, n_vehicles, orderedFile);

	for (int i = 0; i < n_vehicles; i++)	Vehicle_Free(vehicles[i]);
	free(vehicles);

	return 0;
}

int Op_SortBusLinesByLineCode(const char* unorderedFile, const char* orderedFile) {
	char field[64] = { "\0" };
	scanf("%s", field);
	if (strcmp(field, "codLinha") != 0) {
		printf(BAD_FILE_ERROR);
		return 1;
	}
	// Read unorderedFile to ram
	int n_buslines;
	BusLine_t** buslines = BinaryReader_BusLines(unorderedFile, &n_buslines);
	if (buslines == NULL || n_buslines < 0) {
		printf(BAD_FILE_ERROR);
		return 1;
	}

	// Write ordered registers
	Order_BusLines(buslines, 0, n_buslines-1);
	BinaryWriter_BusLineFile(buslines, n_buslines, orderedFile);

	for (int i = 0; i < n_buslines; i++)	BusLine_Free(buslines[i]);
	free(buslines);

	return 0;
}

int Op_SortMergeJoin(const char* vehicleFile, const char* buslineFile) {
	if (!ScanFieldNames()) {
		return 1;
	}

	// Get registers and order
	int n_vehicles;
	Vehicle_t** vehicles = BinaryReader_Vehicles(vehicleFile, &n_vehicles);
	if (vehicles == NULL) {
		printf(BAD_FILE_ERROR);
		return 1;
	}

	if (n_vehicles <= 0) {
		printf(NO_REGS_ERROR);
		free(vehicles);
		return 1;
	}
	
	Order_Vehicles(vehicles, 0, n_vehicles-1);

	// Sorts registers
	int n_buslines;
	BusLine_t** buslines = BinaryReader_BusLines(buslineFile, &n_buslines);

	if (buslines == NULL) {
		printf(BAD_FILE_ERROR);

		for (int i = 0; i < n_vehicles; i++)	Vehicle_Free(vehicles[i]);
		free(vehicles);
		
		return 1;
	}

	if (n_buslines <= 0) {
		printf(NO_REGS_ERROR);

		for (int i = 0; i < n_vehicles; i++)	Vehicle_Free(vehicles[i]);
		free(vehicles);
		free(buslines);
		
		return 1;
	}

	Order_BusLines(buslines, 0, n_buslines-1);

	// Prints registers merging
	char flag = 1;
	int vehiclesIterator = 0;
	int busLinesIterator = 0;
	
	while (vehiclesIterator < n_vehicles && busLinesIterator < n_buslines) {
		// Se v == b printa os 2 e anda com v
		if (vehicles[vehiclesIterator]->lineCode == buslines[busLinesIterator]->lineCode) {
			flag = 0;
			Printer_Merge(vehicles[vehiclesIterator], buslines[busLinesIterator]);
			vehiclesIterator++;
		} else {
			// Se v > b anda com b
			if (vehicles[vehiclesIterator]->lineCode > buslines[busLinesIterator]->lineCode) {
				busLinesIterator++;
			}
			// Se b > v anda com v
			else {
				vehiclesIterator++;
			}
		}
	}
	if (flag) {
		printf(NO_REGS_ERROR);
	}

	// Frees the memory
	for (int i = 0; i < n_vehicles; i++)	Vehicle_Free(vehicles[i]);
	for (int i = 0; i < n_buslines; i++)	BusLine_Free(buslines[i]);
	free(vehicles);
	free(buslines);

	return 0;
}

