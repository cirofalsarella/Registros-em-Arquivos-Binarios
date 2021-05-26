#include "stringTable.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

StringTable* StringTable_Create() {
    // Inits everything to either 0 or NULL.
    StringTable* newTable = malloc(sizeof(StringTable));
    newTable->cells = NULL;
    newTable->columnCount = 0;
    newTable->rowCount = 0;
    newTable->allocatedCellCount = 0;
    return newTable;
}

/**
 * @brief Our strings have length N * REALLOC_BUFFER, where N is a natural number greater or equal to 1.
 */
#define REALLOC_BUFFER 128

/**
 * @brief Helper function called when reading from a csv. Reads a line from a file.
 *        Note that this function allocates N * REALLOC_BUFFER bytes in the heap.
 *        Always free the string returned.
 * 
 * @param stream 
 * @return string 
 */
char* ReadLine(FILE* stream) {
    char* chars = NULL;
    int len = 0;

    // While there are chars, resize line buffer
    do {
        if (len % REALLOC_BUFFER == 0) {
            const int q = len / REALLOC_BUFFER + 1;
            chars = (char*) realloc(chars, q * REALLOC_BUFFER);
        }

        chars[len] = (char) fgetc(stream);
    } while (chars[len++] != '\n' && !feof(stream));

    chars[len - 1] = '\0';

    // Deals with \r\n line ends
    if (len >= 2 && chars[len - 2] == '\r') {
        len -= 1;
        chars[len - 1] = '\0';
    }

    return chars;
}

StringTable* StringTable_FromCsv(const char* fileName, char sep) {
    FILE* stream = fopen(fileName, "r");
    if (stream == NULL){
        return NULL;
    }

    StringTable* table = StringTable_Create();
    char* sepStr = malloc(sizeof(char));
    sepStr[0] = sep;

    while (!feof(stream)) {
        char* line = ReadLine(stream);

        // If the line is empty, just ignore it
        if (line == NULL || strchr(line, sep) == NULL || line[0] == '\0') {
            continue;
        }

        // If we still have not initialized column count...
        if (table->columnCount <= 0) {
            table->columnCount = 1;
            // Count the number of separators in the first line
            int i = 0;
            while (line[i] != '\0') {
                if (line[i] == sep) table->columnCount += 1;
                i += 1;
            }
        }

        table->rowCount += 1;

        // Resizes our cells matrix if necessary
        int requiredAllocCount = table->columnCount * table->rowCount;

        if (requiredAllocCount > table->allocatedCellCount) {
            table->allocatedCellCount = requiredAllocCount * 2;
            table->cells = realloc(table->cells, table->allocatedCellCount * sizeof(char*));
        }

        char* lineStart = line;
        char* token = NULL;
        int col = 0;

        // For each string delimited by "sep"...
        while ((token = Utils_StrSplit(&line, sepStr)) != NULL) {
            if (col >= table->columnCount) break;

            char* cellContent = Utils_StrCopy(token);
            if (token[0] != '\0') {
                int len = strlen(cellContent);
                // Trims right
                while (cellContent[len-1] == '\n' || cellContent[len-1] == '\r') {
                    cellContent[len-1] = '\0';
                    len -= 1;
                }
            }

            // Inserts string into the table
            table->cells[col + (table->rowCount-1) * table->columnCount] = cellContent;
            col += 1;
        }
        
        // Fill the rest with NULL
        for (; col < table->columnCount; ++col) {
            table->cells[col + (table->rowCount-1) * table->columnCount] = NULL;
        }

        free(lineStart);
    }
    
    free(sepStr);
    fclose(stream);
    return table;
}

char* StringTable_GetCellAt(StringTable* table, int column, int row) {
    if (column >= table->columnCount || row+1 >= table->rowCount || column < 0 || row < 0) return NULL;
    return table->cells[column + (row+1) * table->columnCount];
}


char* StringTable_GetCell(StringTable* table, const char* columnName, int row) {
    if (row < 0 || row >= table->rowCount) return NULL;

    // Finds the column with the given name
    int column = -1;

    for (int col = 0; col < table->columnCount; ++col) {
        if (table->cells[col] != NULL && strcmp(table->cells[col], columnName) == 0) {
            column = col;
            break;
        }
    }

    if (column < 0) return NULL;

    return StringTable_GetCellAt(table, column, row);
}

char* StringTable_GetLabel(StringTable* table, int col) {
    return table->cells[col];
}

void StringTable_SetCell(StringTable* table, const char* columnName, int row, char* newValue) {
    if (row < 0 || row >= table->rowCount) return;

    // Finds the column with the given name
    int column = -1;

    for (int col = 0; col < table->columnCount; ++col) {
        if (table->cells[col] != NULL && strcmp(table->cells[col], columnName) == 0) {
            column = col;
            break;
        }
    }

    if (column < 0) return;

    free(table->cells[column + row * table->columnCount]);
    table->cells[column + row * table->columnCount] = newValue;
}

void StringTable_Free(StringTable* table) {
    // Frees our matrices and our strings
    for (int col = 0; col < table->columnCount; ++col) {
        for (int row = 0; row < table->rowCount; ++row) {
            char* cell = table->cells[col + row * table->columnCount];
            if (cell != NULL) {
                free(cell);
            }
        }
    }

    free(table->cells);
    free(table);
}