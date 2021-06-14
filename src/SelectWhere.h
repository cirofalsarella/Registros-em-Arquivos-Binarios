#ifndef _SELECT_WHERE_H_
#define _SELECT_WHERE_H_


//  Tipo definido para executarmos as comparações entre os modelos e os registros
typedef int (*SelectWhereFnType)(void*, Vehicle_t*, BusLine_t*);


//  Seleciona a condição do where
SelectWhereFnType SelectWhere_SetCondition(char *fieldName);

//  Seleciona o molde que será usado para as comparações
void *SelectWhere_SetPattern(char *fieldName);


//  Selecionam quais as linhas que segue o molde esperado
BusLine_t **SelectWhere_SelectBusLines(SelectWhereFnType shouldSelect, void *pattern, BusLine_t **busLines, int nReg, int *nSelectedReg);
Vehicle_t **SelectWhere_SelectVehicles(SelectWhereFnType shouldSelect, void *pattern, Vehicle_t **vehicles, int nReg, int *nSelectedReg);

#endif