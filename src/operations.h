#ifndef _OPERATIONS_H_
#define _OPERATIONS_H_


//  FUNÇÕES RESPONSÁVEIS POR ORGANIZAR AS OPERAÇÕES


/*  Operações 1 e 2
 *      Criam um arquivo binário a partir de um arquivo csv
 */
void Op_CreateTableVehicles();
void Op_CreateTableBuslines();


/*  Operações 3 e 4
 *      Leem um arquivo binário e printam os seus registros
 */
void Op_SelectVehicles();
void Op_SelectBusLines();


/*  Operações 5 e 6
 *      Leem um arquivo binário e printam os seus registros que assumem uma condição
 */
void Op_SelectVehiclesWhere();
void Op_SelectBuslinesWhere();


/*  Operações 7 e 8
 *      Incrementam um arquivo binário pré existente com novos registros
 */
void Op_PushVehicles();
void Op_PushBuslines();


#endif