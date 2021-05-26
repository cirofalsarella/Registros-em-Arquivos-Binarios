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

    VehicleHeader_Free(header);
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
    vehicles = SelectWhere_SelectVehicles(functionPt, pattern, &vehicles, &header->numReg);

    // Linearly searches for the right vehicle
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

void Op_SelectBuslinesWhere() {
    // Reads file name
    char binFile[128] = { '\0' };
    scanf("%s", binFile);

    // Reads binary file from disk
    BusLineHeader* header = NULL;
    BusLine **buslines = BinaryReader_BusLines(&header, binFile);
    if (buslines == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // Reads field name
    char fieldName[64] = { '\0' };
    scanf("%s", fieldName);

    // Calls our generic function that selects by an arbitrary field
    void* functionPt = SelectWhere_SetCondition(fieldName);
    void* pattern = SelectWhere_SetPattern(fieldName);
    buslines = SelectWhere_SelectBusLines(functionPt, pattern, &buslines, &header->numReg);

    // Linearly searches for the right bus line
    if (header->numReg > 0) {
        for (int i = 0; i < header->numReg; i++) {
            Printer_BusLine(buslines[i]);
            BusLine_Free(buslines[i]);
        }
    } else {
        printf("Registro inexistente.\n");
    }

    VehicleHeader_Free(header);
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

    // Updates the binary file
    BinaryWriter_IncrementVehicleFile(vehicles, n, binFile);
    
    free(vehicles);
    PrintHash(binFile);
}

void Op_PushBuslines() {
    // Format: codLinha1 aceitaCartao1 nomeLinha1 corLinha1

    // Reads file name
    char binFile[128] = { '\0' };
    scanf("%s", binFile);

    // Reads binary file from disk
    BusLineHeader* header = NULL;
    BusLine** busLines = BinaryReader_BusLines(&header, binFile);

    // Format: codLinha1 aceitaCartao1 nomeLinha1 corLinha1

    // Scans lineCode, acceptsCreditCard (all fixed-length fields)
    char lineCode[4] = { '\0' };
    ScanQuoteString(lineCode);

    char acceptsCreditCard[1] = { '\0' };
    ScanQuoteString(acceptsCreditCard);

    // Scans name and color (not fixed-length)
    char* name = calloc(100, sizeof(char));
    char* color = calloc(100, sizeof(char));

    ScanQuoteString(name);
    ScanQuoteString(color);

    // Creates a new vehicle and pushes it
    BusLine* newBusLine = BusLine_Create('1', Utils_StrToInt(lineCode), acceptsCreditCard[0], name, color);
    header->numReg += 1;
    busLines = (BusLine**) realloc(busLines, sizeof(BusLine*) * header->numReg);
    busLines[header->numReg-1] = newBusLine;

    // Writes to file
    BinaryWriter_CreateBusLineFile(busLines, header->numReg, header, binFile);
    PrintHash(binFile);
}