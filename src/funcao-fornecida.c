#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void binarioNaTela(char *nomeArquivoBinario) {
	unsigned long i, cs;
	unsigned char *mb;
	size_t fl;
	FILE *fs;
	if(nomeArquivoBinario == NULL || !(fs = fopen(nomeArquivoBinario, "rb"))) {
		fprintf(stderr, "ERRO AO ESCREVER O BINARIO NA TELA (função binarioNaTela): não foi possível abrir o arquivo que me passou para leitura. Ele existe e você tá passando o nome certo? Você lembrou de fechar ele com fclose depois de usar?\n");
		return;
	}
	fseek(fs, 0, SEEK_END);
	fl = ftell(fs);
	fseek(fs, 0, SEEK_SET);
	mb = (unsigned char *) malloc(fl);
	fread(mb, 1, fl, fs);

	cs = 0;
	for(i = 0; i < fl; i++) {
		cs += (unsigned long) mb[i];
	}
	printf("%lf\n", (cs / (double) 100));
	free(mb);
	fclose(fs);
}

/*
void scan_quote_string(char *str) {

	char R;

	while((R = getchar()) != EOF && isspace(R));

	if(R == 'N' || R == 'n') {
		getchar(); getchar(); getchar();
		strcpy(str, "");
	} else if(R == '\"') {
		if(scanf("%[^\"]", str) != 1) {
			strcpy(str, "");
		}
		getchar();
	} else if(R != EOF){
		str[0] = R;
		scanf("%s", &str[1]);
	} else {
		strcpy(str, "");
	}
}
*/