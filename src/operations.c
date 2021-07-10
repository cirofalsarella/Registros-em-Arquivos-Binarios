#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dataModel.h"
#include "binaryHeaders.h"
#include "binaryReader.h"
#include "binaryWriter.h"
#include "printer.h"
#include "utils.h"

//  Printa uma Hash do arquivo (binário na tela)
void PrintHash(char* nomeArquivoBinario) {
	unsigned long i, cs;
	unsigned char *mb;
	size_t fl;
	FILE *fs;

	if (nomeArquivoBinario == NULL || !(fs = fopen(nomeArquivoBinario, "rb"))) {
		fprintf(stderr, "ERRO AO ESCREVER O BINARIO NA TELA (função PrintHash): não foi possível abrir o arquivo que me passou para leitura. Ele existe e você tá passando o nome certo? Você lembrou de fechar ele com fclose depois de usar?\n");
		return;
	}
	
	fseek(fs, 0, SEEK_END);
	fl = ftell(fs);
	fseek(fs, 0, SEEK_SET);
	mb = (unsigned char *) malloc(fl);
	fread(mb, 1, fl, fs);

	cs = 0;
	for (i = 0; i < fl; i++) {
		cs += (unsigned long) mb[i];
	}

	printf("%lf\n", (cs / (double) 100));
	free(mb);
	fclose(fs);
}


void Op_CreateBTreeVehicles() {
    // Gets file names from terminal
    char regsFileName[128] = { '\0' };
    scanf("%s", &regsFileName[0]);

    char bTreeFileName[128] = { '\0' };
    scanf("%s", &bTreeFileName[0]);

	BTreeMetadata_t* meta = BTreeMetadata_Create(bTreeFileName, "w+b", regsFileName, "rb");
	if (meta->bTreeIndexFile == NULL || meta->registersFile == NULL){
		printf("Falha no processamento do arquivo.\n");
		return;
	}

	if (BinaryWriter_BTreeIndexFileVehicles(meta)) {
        printf("Falha no processamento do arquivo.\n");
		return;
	}


    // Prints hash of resulting file
	BTreeMetadata_Free(meta);
    PrintHash(bTreeFileName);
}

void Op_CreateBTreeBusLines() {
    // Gets file names from terminal
    char regsFileName[128] = { '\0' };
    scanf("%s", &regsFileName[0]);

    char bTreeFileName[128] = { '\0' };
    scanf("%s", &bTreeFileName[0]);

	BTreeMetadata_t* meta = BTreeMetadata_Create(bTreeFileName, "w+b", regsFileName, "rb");
	if (meta->bTreeIndexFile == NULL || meta->registersFile == NULL){
		printf("Falha no processamento do arquivo.\n");
		return;
	}

	if (BinaryWriter_BTreeIndexFileBusLines(meta)) {
        printf("Falha no processamento do arquivo.\n");
		return;
	}

    // Prints hash of resulting file
	BTreeMetadata_Free(meta);
    PrintHash(bTreeFileName);
}


void Op_FindVehicle() {
	// Gets file names from terminal
    char regsFileName[128] = { '\0' };
    scanf("%s", &regsFileName[0]);

    char bTreeFileName[128] = { '\0' };
    scanf("%s", &bTreeFileName[0]);

	// Expects "prefixo"
	char fieldName[128] = { '\0' };
    scanf("%s", &fieldName[0]);

	char prefix[128] = { '\0' };
    Utils_ScanQuoteString(&prefix[0]);

	// Checks if field name is valid
	if (strcmp(fieldName, "prefixo") != 0) {
		printf("Falha no processamento do arquivo.\n");
		return;
	}

	// Checks for invalid queries
	if (strcmp(prefix, "NULO") == 0 || strlen(prefix) <= 0) {
		printf("Registro inexistente.\n");
		return;
	}

	// Creates metadata
	BTreeMetadata_t* meta = BTreeMetadata_Create(bTreeFileName, "rb", regsFileName, "rb");
	if (meta->bTreeIndexFile == NULL || meta->registersFile == NULL){
		printf("Falha no processamento do arquivo.\n");
		return;
	}

	// Checks for errors
	if (meta == NULL || meta->bTreeIndexFile == NULL || meta->registersFile == NULL || meta->root == NULL ||
		meta->header == NULL || meta->header->rootRRN < 0 || meta->header->status != '1') {
		printf("Falha no processamento do arquivo.\n");
		return;
	}

	RegKey_t prefixAsKey = Utils_VehiclePrefixHash(prefix);

	if (prefixAsKey < 0) {
		printf("Registro inexistente.\n");
		return;
	}

	BNode_t* node = BTreeMetadata_GetNodeByKey(meta, prefixAsKey);

	if (node == NULL) {
		printf("Registro inexistente.\n");
		return;
	}

	// At this point, node is not NULL
	int keyIndex = BNode_GetKeyIndex(node, prefixAsKey);

	if (keyIndex < 0) {
		printf("Registro inexistente.\n");
		return;
	}

	// Seeks to the right spot in the file
	fseek(meta->registersFile, node->offsets[keyIndex], SEEK_SET);
	Vehicle_t* vehicle = BinaryReader_Vehicle(meta->registersFile);

	Printer_Vehicle(vehicle);

	Vehicle_Free(vehicle);
	BTreeMetadata_Free(meta);
}

void Op_FindBusLine() {
	// Gets file names from terminal
    char regsFileName[128] = { '\0' };
    scanf("%s", &regsFileName[0]);

    char bTreeFileName[128] = { '\0' };
    scanf("%s", &bTreeFileName[0]);

	// Expects "codLinha"
	char fieldName[128] = { '\0' };
    scanf("%s", &fieldName[0]);

	char lineCodeStr[128] = { '\0' };
    scanf("%s", &lineCodeStr[0]);

	// Checks if field name is valid
	if (strcmp(fieldName, "codLinha") != 0) {
		printf("Falha no processamento do arquivo.\n");
		return;
	}

	// Checks for invalid queries
	if (strcmp(lineCodeStr, "NULO") == 0 || strlen(lineCodeStr) <= 0) {
		printf("Registro inexistente.\n");
		return;
	}

	const RegKey_t lineCode = atoi(lineCodeStr);

	// Creates metadata
	BTreeMetadata_t* meta = BTreeMetadata_Create(bTreeFileName, "rb", regsFileName, "rb");
	if (meta->bTreeIndexFile == NULL || meta->registersFile == NULL){
		printf("Falha no processamento do arquivo.\n");
		return;
	}

	// Checks for errors
	if (meta == NULL || meta->bTreeIndexFile == NULL || meta->registersFile == NULL || meta->root == NULL ||
		meta->header == NULL || meta->header->rootRRN < 0 || meta->header->status != '1') {
		printf("Falha no processamento do arquivo.\n");
		return;
	}

	if (lineCode < 0) {
		printf("Registro inexistente.\n");
		return;
	}

	BNode_t* node = BTreeMetadata_GetNodeByKey(meta, lineCode);

	if (node == NULL) {
		printf("Registro inexistente.\n");
		return;
	}

	// At this point, node is not NULL
	int keyIndex = BNode_GetKeyIndex(node, lineCode);

	if (keyIndex < 0) {
		printf("Registro inexistente.\n");
		return;
	}

	// Seeks to the right spot in the file
	fseek(meta->registersFile, node->offsets[keyIndex], SEEK_SET);
	BusLine_t* busLine = BinaryReader_BusLine(meta->registersFile);

	Printer_BusLine(busLine);

	BusLine_Free(busLine);
	BTreeMetadata_Free(meta);
}


void Op_PushVehicles() {
	// Gets file names from terminal
    char regsFileName[128] = { '\0' };
    scanf("%s", &regsFileName[0]);

    char bTreeFileName[128] = { '\0' };
    scanf("%s", &bTreeFileName[0]);

	// Creates metadata
	BTreeMetadata_t* meta = BTreeMetadata_Create(bTreeFileName, "rb", regsFileName, "rb");
	if (meta->bTreeIndexFile == NULL || meta->registersFile == NULL || meta->header->rootRRN < 0){
		printf("Falha no processamento do arquivo.\n");
		return;
	}

	// Gets number of vehicles to insert & reads them from terminal
	int newVehiclesCount;
    scanf("%d", &newVehiclesCount);

	if (newVehiclesCount <= 0) {
		BTreeMetadata_Free(meta);
		return;
	}

    Vehicle_t** newVehicles = Vehicle_Read(newVehiclesCount);
	ByteOffset_t* offsets = (ByteOffset_t*) calloc(newVehiclesCount, sizeof(ByteOffset_t));
	
	// Checks for errors
	if (BinaryWriter_AppendVehicles(newVehicles, newVehiclesCount, meta->registersFile, offsets) != 0) {
		BTreeMetadata_Free(meta);
		printf("Falha no processamento do arquivo.\n");
		return;
	}

	for (int i = 0; i < newVehiclesCount; ++i) {
		// Inserts the vehicle with a certain offset in the tree
		Vehicle_t* vehicle = newVehicles[i];
		RegKey_t vehicleKey = Utils_VehiclePrefixHash(vehicle->prefix);
		BTreeMetadata_Insert(meta, vehicleKey, offsets[i]);

		// Since the vehicle won't be used again, frees it
		Vehicle_Free(vehicle);
		newVehicles[i] = NULL;
	}

	// Prints the hash of the resulting file
    PrintHash(regsFileName);

	// Frees everything
	free(offsets);
	free(newVehicles);
	BTreeMetadata_Free(meta);
}

void Op_PushBusLines() {
	// TODO: Test Op_PushVehicles, then make this
}