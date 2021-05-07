#include "binario.h"

int GetBINData(FILE *fp, void *value, long long int size) {
    if (fread(&value, size, 1, fp)){
        return 1;
    }
    return 0;
}

char *GetBINString(FILE *fp, int len) {
    char *string = (char*) malloc (len * (sizeof(char)));
    if (fread(string, 50, sizeof(char), fp)) {
        return string;
    }

    free(string);
    return NULL;
}

void *WriteBINData(FILE *fp, void *value, long long int size) {
    fwrite(&value, size, 1, fp);
}

void *WriteBINString(FILE *fp, char *string, int len) {
    fwrite(string, sizeof(char), len, fp);
}