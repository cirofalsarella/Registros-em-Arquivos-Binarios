#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdint.h>

/**
 * @brief Helper function used to split strings.
 * 
 * @param stringp 
 * @param delim 
 * @return char* 
 */
char* Utils_StrSplit(char **stringp, const char *delim);

/**
 * @brief Helper function used to copy strings.
 * 
 * @param str 
 * @return char* 
 */
char* Utils_StrCopy(const char* str);

/**
 * @brief Returns min(x, y).
 * 
 * @param x 
 * @param y 
 * @return int 
 */
int Utils_MinNumber(int x, int y);

/**
 * @brief Safely copies a string with varying length to a string with fixed length (ensures that we don't copy unalloacted memory).
 * 
 * @param from 
 * @param to 
 * @param length 
 */
void Utils_StrCopyToFixedLen(char* from, char* to, int length);

/**
 * @brief Returns true if strA == strB.
 * 
 * @param strA 
 * @param strB 
 * @param length 
 */
int Utils_StrEqualsFixedLen(char* strA, char* strB, int length);

/**
 * @brief Prints str in terminal.
 * 
 * @param str 
 * @param maxLength 
 */
void Utils_StrPrintWithFixedLen(const char* str, int maxLength);

void Utils_ScanQuoteString(char *str);

/**
 * @brief Returns the given str converted to an integer. If str is "NULO", the integer returned is -1.
 * 
 * @param str 
 * @return int32_t 
 */
int32_t Utils_StrToInt(const char* str);

/*  
    Função convertePrefixo.
    Converte o prefixo do veículo para int

    OBS1:   retorna -1 se o primeiro caracter é '*'

    OBS2:   retorna LIXO se a string contém algo diferente de números e letras 
            maiúsculas (com excessão do caso da OBS1)

    COMO FUNCIONA:

        Para converter os prefixos para int e garantir que prefixos diferentes 
        deem números diferentes interpretamos eles como números em base 36

        Um número N com 5 digitos dI em base 36 é escrito como:

            N = d0 * 36^0 + d1 * 36^1 + d2 * 36^2 + d3 * 36^3 + d4 * 36^4

        Nota-se que estamos escrevendo do digito menos significativo para o 
        mais significativo

        Como o nosso prefixo têm 5 bytes e eles podem assumir 36 valores
        cada, podemos interpretar cada char dele como um digito em base 36, 
        prefixos diferentes formaram números diferentes em base 36 e portanto 
        números diferentes quando convertemos para um int normal
*/
int32_t Utils_VehiclePrefixHash(char* str);

#endif