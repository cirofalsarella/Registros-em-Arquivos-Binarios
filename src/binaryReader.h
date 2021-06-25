#ifndef _READ_BINARY_H_
#define _READ_BINARY_H_

// FUNÇÕES RESPONSÁVEIS PELA LEITURA DE ARQUIVOS BINÁRIOS

/**
 * @brief Reads the entire binary file containing vehicle information.
 * 
 * @param header Pointer to the variable that will store the header.
 * @param fileName Name of the file.
 * @return Vehicle_t** 
 */
Vehicle_t** BinaryReader_Vehicles(VehicleHeader_t** header, char *fileName);

/**
 * @brief Reads the entire binary file containing bus line information.
 * 
 * @param header Pointer to the variable that will store the header.
 * @param fileName Name of the file.
 * @return BusLine_t** 
 */
BusLine_t** BinaryReader_BusLines(BusLineHeader_t** header, char *fileName);

/**
 * @brief Reads one vehicle from src file.
 * 
 * @param srcFile The file to read the vehicle from.
 * @return Vehicle_t* 
 */
Vehicle_t* BinaryReader_ReadVehicle(FILE* srcFile);

/**
 * @brief Reads one vehicle from src file.
 * 
 * @param srcFile The file to read the vehicle from.
 * @return BusLine_t* 
 */
BusLine_t* BinaryReader_ReadBusLine(FILE* srcFile);

/**
 * @brief Reads vehicle header from src file. Assumes file pointer is at the correct place!
 * 
 * @param srcFile The file to read the vehicle from.
 * @return VehicleHeader_t* 
 */
VehicleHeader_t* BinaryReader_ReadVehicleHeader(FILE *srcFile);

/**
 * @brief Reads bus line header from src file. Assumes file pointer is at the correct place!
 * 
 * @param srcFile The file to read the bus line from.
 * @return BusLine_t* 
 */
BusLine_t* BinaryReader_ReadBusLineHeader(FILE *srcFile);

#endif