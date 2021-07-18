#ifndef _DATA_MODEL_H_
#define _DATA_MODEL_H_

#include <stdint.h>

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef BOOL_NULL
#define BOOL_NULL -1
#endif

//  FUNCIONALIDADES RESPONSÁVEIS POR ESTRUTURAR OS REGISTROS

#define REG_SIZE_IGNORED 5 // Register size ignored by register->regSize (1 byte for removed and 4 for regSize)

#define VEHICLE_FIXED_LENGTH 31 // 31 bytes of fixed length (IGNORES 'REMOVED' AND 'REGSIZE')
#define BUSLINE_FIXED_LENGTH 13 // 13 bytes of fixed length (IGNORES 'REMOVED' AND 'REGSIZE')

#define VEHICLE_HEADER_SIZE (1+8+4+4+18+35+42+26+17+20)
#define BUSLINE_HEADER_SIZE (1+8+4+4+15+13+13+24)

//  Header dos arquivos Binários

typedef struct VehicleHeader {
    int64_t nextReg;

    int32_t validRegCount;
    int32_t removedRegCount;
    
    // Labels
    char describePrefix[18];
    char describeDate[35];
    char describePlaces[42];
    char describeLine[26];
    char describeModel[17];
    char describeCategory[20];
} VehicleHeader_t;

typedef struct BusLineHeader {
    int64_t nextReg;

    int32_t validRegCount;
    int32_t removedRegCount;
    
    // Labels
    char describeCode[15];
    char describeCard[13];
    char describeName[13];
    char describeLine[24];
} BusLineHeader_t;



//  Registros

typedef struct Vehicle {
    int64_t offset;

    char removed;       // '0' if this register has been removed, '1' otherwise
    int32_t regSize;    // Size of this register

    // Fixed-length fields
    char prefix[5];
    char date[10];
    int32_t numSeats;
    int32_t lineCode;

    // Variable-length fields
    int32_t modelLength;
    char* model;
    int32_t categoryLength;
    char* category;
} Vehicle_t;

typedef struct BusLine {
    int64_t offset;

    char removed;       // '0' if this register has been removed, '1' otherwise
    int32_t regSize;    // Size of this register

    // Fixed-length fields
    int32_t lineCode;
    char acceptsCreditCard;

    // Variable-length fields
    int32_t nameLength;
    char* name;
    int32_t colorLength;
    char* color;
} BusLine_t;



//  Cria um novo Registro a partir de seus campos

Vehicle_t* Vehicle_Create(char removed, char* prefix, char* date, int32_t numPlaces, int32_t lineCode, char* model, char* category, int64_t offset);

BusLine_t* BusLine_Create(char removed, int32_t lineCode, char acceptsCreditCard, char* name, char* color, int64_t offset);



//  Lê n registros do stdin

Vehicle_t* Vehicle_Read();

BusLine_t* BusLine_Read();



// Libera a memória alocada por um Registro

void Vehicle_Free(Vehicle_t* vehicle);

void BusLine_Free(BusLine_t* busLine);



#endif
