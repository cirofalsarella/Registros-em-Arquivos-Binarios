#include <stdio.h>
#include <stdlib.h>

int GetBINData(FILE *fp, void *value, long long int size);
char *GetBINString(FILE *fp, int len);
void *WriteBINData(FILE *fp, void *value, long long int size);
void *WriteBINString(FILE *fp, char *string, int len);
