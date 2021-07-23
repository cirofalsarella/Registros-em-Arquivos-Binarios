/* * * * * * * * * * * * * * * * * * * * 
 *  11795593   Ciro Grossi Falsarella  *
 *  11795641   Pedro Martelleto        *
 * * * * * * * * * * * * * * * * * * * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "core/utils.h"
#include "core/operations.h"

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


int main() {
    // Gets op number
    int op;
    scanf("%d", &op);

    // Gets file names from terminal
    char firstFileName[128] = { '\0' };
    scanf("%s", firstFileName);

    char secondFileName[128] = { '\0' };
    scanf("%s", secondFileName);

    // Chooses the right operation
    switch (op) {
    case 15:
        Op_NestedLoopJoin(firstFileName, secondFileName);
        break;
    case 16:
        Op_SingleLoopJoin(firstFileName, secondFileName);
        break;
    case 17:
        if (!Op_SortVehiclesByLineCode(firstFileName, secondFileName)) PrintHash(secondFileName);
        break;
    case 18:
        if (!Op_SortBusLinesByLineCode(firstFileName, secondFileName)) PrintHash(secondFileName);
        break;
    case 19:
        Op_SortMergeJoin(firstFileName, secondFileName);
        break;
    }

    return 0;
}