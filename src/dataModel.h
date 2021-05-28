#ifndef _DATA_MODEL_H_
#define _DATA_MODEL_H_


//  FUNCIONALIDADES RESPONSÁVEIS POR ESTRUTURAR OS REGISTROS


#define VEHICLE_FIXED_LENGTH 31 // 31 bytes of fixed length (IGNORES 'REMOVED' AND 'REGSIZE')
#define BUSLINE_FIXED_LENGTH 13 // 13 bytes of fixed length (IGNORES 'REMOVED' AND 'REGSIZE')



//  Header dos arquivos Binários

typedef struct VehicleHeader {
    char status;
    int64_t nextReg;

    int32_t numReg;
    int32_t numRegRemov;
    
    // Labels
    char describePrefix[18];
    char describeDate[35];
    char describePlaces[42];
    char describeLine[26];
    char describeModel[17];
    char describeCategory[20];
} VehicleHeader;

typedef struct BusLineHeader {
    char status;
    int64_t nextReg;

    int32_t numReg;
    int32_t numRegRemov;
    
    // Labels
    char describeCode[15];
    char describeCard[13];
    char describeName[13];
    char describeLine[24];
} BusLineHeader;



//  Registros

typedef struct Vehicle {
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
} Vehicle;

typedef struct BusLine {
    // '0' if this register has been removed, '1' otherwise
    char removed;
    // Size of this register
    int32_t regSize;

    // Fixed-length fields
    int32_t lineCode;
    char acceptsCreditCard;

    // Variable-length fields
    int32_t nameLength;
    char* name;
    int32_t colorLength;
    char* color;
} BusLine;



//  Cria um novo Registro a partir de seus campos

Vehicle* Vehicle_Create(char removed, char* prefix, char* date, int32_t numPlaces, int32_t lineCode, char* model, char* category);

BusLine* BusLine_Create(char removed, int32_t lineCode, char acceptsCreditCard, char* name, char* color);



//  Lê n registros do stdin

Vehicle** Vehicle_Read(int n);

BusLine** BusLine_Read(int n);



// Libera a memória alocada por um Registro

void Vehicle_Free(Vehicle* vehicle);

void BusLine_Free(BusLine* busLine);



#endif
