#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../dataModel/dataModel.h"
#include "../dataModel/binaryHeaders.h"
#include "../io/binaryReader.h"
#include "../io/binaryWriter.h"
#include "../core/printer.h"
#include "../core/utils.h"

//  Printa uma Hash do arquivo (binário na tela)
void PrintHash(const char* nomeArquivoBinario) {
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

void Op_15(const char* vehicleFile, const char* buslineFile) {

}

void Op_16(const char* vehicleFile, const char* buslineFile) {

}

void Op_17(const char* unorderedFile, const char* orderedFile) {
	char field[64] = { "\0" };
	scanf("%s", field);
	
	// Read unorderedFile to ram
	int n_vehicles;
	Vehicle_t** vehicles = BinaryReader_Vehicles(unorderedFile, &n_vehicles);

	// Order file acording to the especified field


	// Write file in orderedFile
	BinaryWriter_VehicleFile(vehicles, n_vehicles, orderedFile);

	for (int i=0; i<n_vehicles; i++) {
		Vehicle_Free(vehicles[i]);
	}
	free(vehicles);
}



void Op_18(const char* unorderedFile, const char* orderedFile) {

}

void Op_19(const char* vehicleFile, const char* buslineFile) {

}