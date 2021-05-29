#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stringTable.h"
#include "dataModel.h"
#include "csvToBinary.h"
#include "binaryHeaders.h"
#include "binaryReader.h"
#include "binaryWriter.h"
#include "selectWhere.h"
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


void Op_CreateTableVehicles() {
    //  lê o nome dos arquivos
    char csvFile[128], binFile[128];
    scanf("%s %s", csvFile, binFile);

    //  lê o arquivo csv
    StringTable* table = StringTable_FromCsv(csvFile, ',');
    if (table == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    //  transforma o arquivo csv em binário e libéra memória
    CsvToBinary_WriteVehicleFile(table, binFile);
    StringTable_Free(table);

    //  confere se o binário está correto
    PrintHash(binFile);
}

void Op_CreateTableBuslines() {
    //  lê o nome dos arquivos
    char csvFile[128], binFile[128];
    scanf("%s %s", csvFile, binFile);

    //  lê o arquivo csv
    StringTable* Table = StringTable_FromCsv(csvFile, ',');
        if (Table == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    //  transforma o arquivo csv em binário e libéra memória
    CsvToBinary_WriteBusLineFile(Table, binFile);
    StringTable_Free(Table);

    //  confere se o binário está correto
    PrintHash(binFile);
}


void Op_SelectVehicles() {
    //  lê o nome do arquivo
    char binFile[128];
    scanf("%s", binFile);

    //  lê o arquivo binário e salva registros não removidos
    VehicleHeader* header = NULL;
    Vehicle** vehicles = binaryReader_Vehicles(&header, binFile);
    if (vehicles == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }
    
    //  imprime todos os registros
    if (header->numReg > 0) {
        for (int i = 0; i < header->numReg; i++) {
            Printer_Vehicle(vehicles[i]);
            Vehicle_Free(vehicles[i]);
        }
    } else {
        printf("Registro inexistente.\n");
    }

    //  libera memória
    BinaryHeaders_FreeVehicleHeader(header);
    free(vehicles);
}

void Op_SelectBusLines() {
    //  lê o nome do arquivo
    char binFile[128] = { '\0' };
    scanf("%s", binFile);

    //  lê o arquivo binário e salva registros não removidos
    BusLineHeader* header = NULL;
    BusLine** buslines = binaryReader_BusLines(&header, binFile);
    if (buslines == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    //  imprime todos os registros
    if (header->numReg > 0) {
        for (int i = 0; i < header->numReg; i++) {
            Printer_BusLine(buslines[i]);
            BusLine_Free(buslines[i]);
        }
    } else {
        printf("Registro inexistente.\n");
    }

    //  libera memória
    BinaryHeaders_FreeBusLineHeader(header);
    free(buslines);
}


void Op_SelectVehiclesWhere() {
    //  lê o nome do arquivo
    char binFile[128] = { '\0' };
    scanf("%s", binFile);

    //  lê o arquivo binário e salva registros não removidos
    VehicleHeader* header = NULL;
    Vehicle** vehicles = binaryReader_Vehicles(&header, binFile);
    if (vehicles == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    //  lê o nome do campo
    char fieldName[64] = { '\0' };
    scanf("%s", fieldName);

    //  seleciona a condição e modelo
    SelectWhereFnType functionPt = SelectWhere_SetCondition(fieldName);
    void* pattern = SelectWhere_SetPattern(fieldName);


    // Seleciona e imprime os registros
    int32_t nSelectedReg;
    Vehicle** selectedVehicles = SelectWhere_SelectVehicles(functionPt, pattern, vehicles, header->numReg, &nSelectedReg);

    if (nSelectedReg > 0) {
        for (int i = 0; i < nSelectedReg; i++) {
            Printer_Vehicle(selectedVehicles[i]);
        }
    } else {
        printf("Registro inexistente.\n");
    }


    // Libera a memória
    for (int i=0; i<header->numReg; i++) {
        Vehicle_Free(vehicles[i]);
    }

    BinaryHeaders_FreeVehicleHeader(header);
    free(selectedVehicles);
    free(vehicles);
    free(pattern);
}

void Op_SelectBuslinesWhere() {
    //  lê o nome do arquivo
    char binFile[128] = { '\0' };
    scanf("%s", binFile);

    //  lê o arquivo binário e salva registros não removidos
    BusLineHeader* header = NULL;
    BusLine** buslines = binaryReader_BusLines(&header, binFile);
    if (buslines == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    //  lê o nome do campo
    char fieldName[64] = { '\0' };
    scanf("%s", fieldName);

    //  seleciona a condição e modelo
    void *functionPt = SelectWhere_SetCondition(fieldName);
    void *pattern = SelectWhere_SetPattern(fieldName);


    // Seleciona e imprime os registros
    int32_t nSelectedReg;
    BusLine** selectedBuslines = SelectWhere_SelectBusLines(functionPt, pattern, buslines, header->numReg, &nSelectedReg);

    if (nSelectedReg > 0) {
        for (int i = 0; i < nSelectedReg; i++) {
            Printer_BusLine(selectedBuslines[i]);
        }
    } else {
        printf("Registro inexistente.\n");
    }


    // Libera a memória
    for (int i=0; i<header->numReg; i++) {
        BusLine_Free(buslines[i]);
    }

    BinaryHeaders_FreeBusLineHeader(header);
    free(selectedBuslines);
    free(buslines);
    free(pattern);
}


void Op_PushVehicles() {
    //  lê o nome do arquivo
    char binFile[128] = { '\0' };
    scanf("%s", binFile);

    //  lê os registros do stdin
    int n;
    scanf("%d", &n);
    Vehicle** vehicles = Vehicle_Read(n);
    if (vehicles == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    //  atualiza o arquivo binário
    if (BinaryWriter_IncrementVehicleFile(vehicles, n, binFile)) {
        printf("Falha no processamento do arquivo.\n");
        free(vehicles);
        return;
    }
    
    //  libera a memória alocada
    free(vehicles);
    PrintHash(binFile);
}

void Op_PushBuslines() {
    //  lê o nome do arquivo
    char binFile[128] = { '\0' };
    scanf("%s", binFile);

    //  lê os registros do stdin
    int n;
    scanf("%d", &n);
    BusLine** buslines = BusLine_Read(n);
    if (buslines == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    //  atualiza o arquivo binário
    if (BinaryWriter_IncrementBusLineFile(buslines, n, binFile)) {
        printf("Falha no processamento do arquivo.\n");
        free(buslines);
        return;
    }
    
    //  libera a memória alocada
    free(buslines);
    PrintHash(binFile);
}
