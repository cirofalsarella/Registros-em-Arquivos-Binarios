/**
 * @file stringTable.c
 * @author Pedro Martelleto
 * @brief String table header
 */

#ifndef _STRING_TABLE_H_
#define _STRING_TABLE_H_

typedef struct {
    int columnCount;
    // Row count includes header!
    int rowCount;
    int allocatedCellCount;
    // The cells matrix does not include the headers.
    char** cells;
} StringTable;

/**
 * @brief Creates a new StringTable. Always call this when initializing one.
 * 
 * @return StringTable* 
 */
StringTable* StringTable_Create();

/**
 * @brief Creates a new StringTable from a csv file.
 * 
 * @param fileName
 * @param sep
 * @return StringTable* 
 */
StringTable* StringTable_FromCsv(const char* fileName, char sep);

/**
 * @brief Returns NULL if column or row are invalid (negative or greater than column count or row count).
 * Otherwise, returns cells[column][row].
 * 
 * @param table 
 * @param column 
 * @param row 
 * @return char* 
 */
char* StringTable_GetCellAt(StringTable* table, int column, int row);

/**
 * @brief Calls StringTable_GetCellAt for the column with the given columnName.
 * 
 * @param table 
 * @param columnName 
 * @param row 
 * @return char* 
 */
char* StringTable_GetCell(StringTable* table, const char* columnName, int row);

/**
 * @brief Sets cells at the column with the given columnName and row.
 * 
 * @param table 
 * @param columnName 
 * @param row 
 * @return char* 
 */
void StringTable_SetCell(StringTable* table, const char* columnName, int row, char* newValue);

/**
 * @brief Frees the given StringTable.
 * 
 * @param table 
 */
void StringTable_Free(StringTable* table);

#endif