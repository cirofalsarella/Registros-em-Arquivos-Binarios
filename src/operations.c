#include "operations.h"
#include "binaryWriter.h"
#include "binaryHeaders.h"
#include "utils.h"

/**
 * @brief Prints a hash of the given binary file (aka binarioNaTela).
 * 
 * @param nomeArquivoBinario 
 */
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
    // Reads csv and binary
    char csvFile[128], binFile[128];
    scanf("%s %s", csvFile, binFile);

    // Converts csv to binary and saves to disk
    StringTable* Table = StringTable_FromCsv(csvFile, ',');
    if (Table == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    CsvToBinary_WriteVehicleFile(Table, binFile);

    StringTable_Free(Table);
    PrintHash(binFile);
}

void Op_CreateTableBuslines() {
    // Reads csv and binary
    char csvFile[128], binFile[128];
    scanf("%s %s", csvFile, binFile);

    // Converts csv to binary and saves to disk
    StringTable* Table = StringTable_FromCsv(csvFile, ',');
        if (Table == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    CsvToBinary_WriteBusLineFile(Table, binFile);

    StringTable_Free(Table);
    PrintHash(binFile);
}

void Op_SelectVehicles() {
    // Reads file name
    char binFile[128];
    scanf("%s", binFile);

    // Reads binary file from disk
    VehicleHeader* header = NULL;
    Vehicle** vehicles = BinaryReader_Vehicles(&header, binFile);
    if (vehicles == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }
    
    // Prints all vehicles
    if (header->numReg > 0) {
        for (int i = 0; i < header->numReg; i++) {
            Printer_Vehicle(vehicles[i]);
            Vehicle_Free(vehicles[i]);
        }
    } else {
        printf("Registro inexistente.\n");
    }

    VehicleHeader_Free(header);
    free(vehicles);
}

void Op_SelectBusLines() {
    // Reads file name
    char binFile[128] = { '\0' };
    scanf("%s", binFile);

    // Reads binary file from disk
    BusLineHeader* header = NULL;
    BusLine** buslines = BinaryReader_BusLines(&header, binFile);
    if (buslines == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // Prints all bus lines
    if (header->numReg > 0) {
        for (int i = 0; i < header->numReg; i++) {
            Printer_BusLine(buslines[i]);
            BusLine_Free(buslines[i]);
        }
    } else {
        printf("Registro inexistente.\n");
    }

    BusLineHeader_Free(header);
    free(buslines);
}

void Op_SelectVehiclesWhere() {
    // Reads file name
    char binFile[128] = { '\0' };
    scanf("%s", binFile);

    // Reads binary file from disk
    VehicleHeader* header = NULL;
    Vehicle** vehicles = BinaryReader_Vehicles(&header, binFile);
    if (vehicles == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // Reads field name
    char fieldName[64] = { '\0' };
    scanf("%s", fieldName);

    // Calls our generic function that selects by an arbitrary field
    void *functionPt = SelectWhere_SetCondition(fieldName);
    void *pattern = SelectWhere_SetPattern(fieldName);

    // Select the vehicles
    int32_t nSelectedReg;
    Vehicle** selectedVehicles = SelectWhere_SelectVehicles(functionPt, pattern, vehicles, header->numReg, &nSelectedReg);

    // prints the vehicles
    if (nSelectedReg > 0) {
        for (int i = 0; i < nSelectedReg; i++) {
            Printer_Vehicle(selectedVehicles[i]);
        }
    } else {
        printf("Registro inexistente.\n");
    }

    // Free the vehicles
    for (int i=0; i<header->numReg; i++) {
        Vehicle_Free(vehicles[i]);
    }

    VehicleHeader_Free(header);
    free(selectedVehicles);
    free(vehicles);
}

void Op_SelectBuslinesWhere() {
    // Reads file name
    char binFile[128] = { '\0' };
    scanf("%s", binFile);

    // Reads binary file from disk
    BusLineHeader* header = NULL;
    BusLine** buslines = BinaryReader_BusLines(&header, binFile);
    if (buslines == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // Reads field name
    char fieldName[64] = { '\0' };
    scanf("%s", fieldName);

    // Calls our generic function that selects by an arbitrary field
    void *functionPt = SelectWhere_SetCondition(fieldName);
    void *pattern = SelectWhere_SetPattern(fieldName);
    printf("pattern = %s\n", (char*) pattern);

    // Select the buslines
    int32_t nSelectedReg;
    BusLine** selectedBuslines = SelectWhere_SelectBusLines(functionPt, pattern, buslines, header->numReg, &nSelectedReg);

    // prints the buslines
    if (nSelectedReg > 0) {
        for (int i = 0; i < nSelectedReg; i++) {
            Printer_BusLine(selectedBuslines[i]);
        }
    } else {
        printf("Registro inexistente.\n");
    }

    // Free the vehicles
    for (int i=0; i<header->numReg; i++) {
        BusLine_Free(buslines[i]);
    }

    BusLineHeader_Free(header);
    free(selectedBuslines);
    free(buslines);
}

void Op_PushVehicles() {
    // Reads file name
    char binFile[128] = { '\0' };
    scanf("%s", binFile);

    // Reads vehicles from stdin
    int n;
    scanf("%d", &n);
    Vehicle** vehicles = Vehicle_Read(n);
    if (vehicles == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // Updates the binary file
    if (BinaryWriter_IncrementVehicleFile(vehicles, n, binFile)) {
        printf("Falha no processamento do arquivo.\n");
        free(vehicles);
        return;
    }
    
    free(vehicles);
    PrintHash(binFile);
}

void Op_PushBuslines() {
    // Reads file name
    char binFile[128] = { '\0' };
    scanf("%s", binFile);

    // Reads buslines from stdin
    int n;
    scanf("%d", &n);
    BusLine** buslines = BusLine_Read(n);
    if (buslines == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // Updates the binary file
    if (BinaryWriter_IncrementBusLineFile(buslines, n, binFile)) {
        printf("Falha no processamento do arquivo.\n");
        free(buslines);
        return;
    }
    
    free(buslines);
    PrintHash(binFile);
}
