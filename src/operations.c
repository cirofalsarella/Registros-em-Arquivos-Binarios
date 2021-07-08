#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dataModel.h"
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

void Op_CreateBTreeVehicles() {
    // Gets file names from terminal
    char regsFileName[128] = { '\0' };
    scanf("%s", &regsFileName[0]);

    char bTreeFileName[128] = { '\0' };
    scanf("%s", &bTreeFileName[0]);

	BTreeMetadata_t* meta = BTreeMetadata_Create(bTreeFileName, "rb+", regsFileName, "rb");
	BinaryWriter_BTreeIndexFileVehicles(meta);

    // Prints hash of resulting file
    PrintHash(bTreeFileName);
	BTreeMetadata_Free(meta);
}

void Op_CreateBTreeBusLines() {
    // Gets file names from terminal
    char regsFileName[128] = { '\0' };
    scanf("%s", &regsFileName[0]);

    char bTreeFileName[128] = { '\0' };
    scanf("%s", &bTreeFileName[0]);

	BTreeMetadata_t* meta = BTreeMetadata_Create(bTreeFileName, "rb+", regsFileName, "rb");
	BinaryWriter_BTreeIndexFileBusLines(meta);

    // Prints hash of resulting file
    PrintHash(bTreeFileName);
	BTreeMetadata_Free(meta);
}