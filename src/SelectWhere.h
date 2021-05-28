#ifndef _SELECT_WHERE_H_
#define _SELECT_WHERE_H_


//  Tipo definido para executarmos as comparações entre os modelos e os registros
typedef int (*SelectWhereFnType)(void*, Vehicle*, BusLine*);


//  Seleciona a condição do where
SelectWhereFnType SelectWhere_SetCondition(char *fieldName);

//  Seleciona o molde que será usado para as comparações
void *SelectWhere_SetPattern(char *fieldName);


//  Selecionam quais as linhas que segue o molde esperado
BusLine **SelectWhere_SelectBusLines(SelectWhereFnType shouldSelect, void *pattern, BusLine **busLines, int nReg, int *nSelectedReg);
Vehicle **SelectWhere_SelectVehicles(SelectWhereFnType shouldSelect, void *pattern, Vehicle **vehicles, int nReg, int *nSelectedReg);

#endif