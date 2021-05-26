#ifndef _DATA_MODEL_H_
#define _DATA_MODEL_H_

#include <stdint.h>

typedef struct {
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

typedef struct {
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

#define VEHICLE_FIXED_LENGTH 31 // 31 bytes of fixed length (IGNORES 'REMOVED' AND 'REGSIZE')

typedef struct {
    // '0' if this register has been removed, '1' otherwise
    char removed;
    // Size of this register
    int32_t regSize;

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

#define BUSLINE_FIXED_LENGTH 13 // 13 bytes of fixed length (IGNORES 'REMOVED' AND 'REGSIZE')

typedef struct {
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

/**
 * @brief Creates a new Vehicle. Automatically calculates regSize.
 * 
 * @param removed 
 * @param prefix 
 * @param date 
 * @param numPlaces 
 * @param lineCode 
 * @param model 
 * @param category 
 * @return Vehicle* 
 */
Vehicle* Vehicle_Create(char removed, char* prefix, char* date, int32_t numPlaces, int32_t lineCode, char* model, char* category);

/**
 * @brief Frees the memory allocated for the given vehicle.
 * 
 * @param vehicle 
 */
void Vehicle_Free(Vehicle* vehicle);

/**
 * @brief Creates a new bus line. Automatically calculates regSize.
 * 
 * @param removed 
 * @param lineCode 
 * @param acceptsCreditCard 
 * @param name 
 * @param color 
 * @return BusLine* 
 */
BusLine* BusLine_Create(char removed, int32_t lineCode, char acceptsCreditCard, char* name, char* color);

/**
 * @brief Frees the memory allocated for the given vehicle.
 * 
 * @param busLine 
 */
void BusLine_Free(BusLine* busLine);

#endif
