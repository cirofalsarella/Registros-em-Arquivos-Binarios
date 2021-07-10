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
	if (meta->bTreeIndexFile == NULL || meta->registersFile == NULL || meta->header->rootRRN < 0){
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
	if (meta->bTreeIndexFile == NULL || meta->registersFile == NULL || meta->header->rootRRN < 0){
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
	BTreeMetadata_t* meta = BTreeMetadata_Create(bTreeFileName, "rb+", regsFileName, "rb+");
	if (meta->bTreeIndexFile == NULL || meta->registersFile == NULL || meta->header->rootRRN < 0){
		printf("Falha no processamento do arquivo.\n");
		return;
	}

	// getting the header intel
	fseek(meta->registersFile, 1, SEEK_SET);
    int64_t proxReg;
    fread(&proxReg, sizeof(int64_t), 1, meta->registersFile);
    int32_t numReg, numRegRem;
    fread(&numReg, sizeof(int32_t), 1, meta->registersFile);
    fread(&numRegRem, sizeof(int32_t), 1, meta->registersFile);


	// Gets number of registers to insert & reads them from terminal
	int n;
    scanf("%d", &n);

	fseek(meta->registersFile, proxReg, SEEK_SET);
    // Insert the registers
    for (int i = 0; i < n; i++) {
        Vehicle_t* reg = Vehicle_Read();
		if (reg == NULL) {
			printf("Falha no processamento do arquivo.\n");
			BTreeMetadata_Free(meta);
			return;
		}

        if (reg->removed == '0') {
			numRegRem++;
            Vehicle_Free(reg);
            continue;
        }
		numReg++;

		reg->offset = BinaryWriter_Vehicle(reg, meta->registersFile);
        BTreeMetadata_Insert(meta, Utils_VehiclePrefixHash(reg->prefix), reg->offset);
        Vehicle_Free(reg);
    }
	fseek(meta->registersFile, 0, SEEK_END);
	proxReg = 0; //ftell(meta->registersFile);
	// [FLAG]	Porque 0 e não o ftell???

	// Adjust the headers of both files
	fseek(meta->registersFile, 1, SEEK_SET);
	fwrite(&proxReg, sizeof(int64_t), 1, meta->registersFile);
	fwrite(&numReg, sizeof(int32_t), 1, meta->registersFile);
	fwrite(&numRegRem, sizeof(int32_t), 1, meta->registersFile);

	BTreeMetadata_Free(meta);
	PrintHash(bTreeFileName);
}

void Op_PushBusLines() {
	// Gets file names from terminal
    char regsFileName[128] = { '\0' };
    scanf("%s", &regsFileName[0]);

    char bTreeFileName[128] = { '\0' };
    scanf("%s", &bTreeFileName[0]);

	// Creates metadata
	BTreeMetadata_t* meta = BTreeMetadata_Create(bTreeFileName, "rb+", regsFileName, "rb+");
	if (meta->bTreeIndexFile == NULL || meta->registersFile == NULL || meta->header->rootRRN < 0){
		printf("Falha no processamento do arquivo.\n");
		return;
	}

	// getting the header intel
	fseek(meta->registersFile, 1, SEEK_SET);
    int64_t proxReg;
    fread(&proxReg, sizeof(int64_t), 1, meta->registersFile);
    int32_t numReg, numRegRem;
    fread(&numReg, sizeof(int32_t), 1, meta->registersFile);
    fread(&numRegRem, sizeof(int32_t), 1, meta->registersFile);


	// Gets number of registers to insert & reads them from terminal
	int n;
    scanf("%d", &n);

	fseek(meta->registersFile, proxReg, SEEK_SET);
    // Insert the registers
    for (int i = 0; i < n; i++) {
        BusLine_t* reg = BusLine_Read();
		if (reg == NULL) {
			printf("Falha no processamento do arquivo.\n");
			BTreeMetadata_Free(meta);
			return;
		}

        if (reg->removed == '0') {
			numRegRem++;
            BusLine_Free(reg);
            continue;
        }
		numReg++;

		reg->offset = BinaryWriter_BusLine(reg, meta->registersFile);
        BTreeMetadata_Insert(meta, reg->lineCode, reg->offset);
        BusLine_Free(reg);
    }
	fseek(meta->registersFile, 0, SEEK_END);
	proxReg = 0; //ftell(meta->registersFile);
	

	// Adjust the headers of both files
	fseek(meta->registersFile, 1, SEEK_SET);
	fwrite(&proxReg, sizeof(int64_t), 1, meta->registersFile);
	fwrite(&numReg, sizeof(int32_t), 1, meta->registersFile);
	fwrite(&numRegRem, sizeof(int32_t), 1, meta->registersFile);

	BTreeMetadata_Free(meta);
	PrintHash(bTreeFileName);
}

