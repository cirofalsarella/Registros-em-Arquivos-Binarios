#ifndef _BINARY_WRITER_H_
#define _BINARY_WRITER_H_


//  FUNÇÕES RESPONSÁVEIS POR ESCREVER ARQUIVOS BINÁRIOS

/**
 * @brief Creates a vehicle binary file with the given vehicles.
 * 
 * @param vehicles 
 * @param vehiclesCount 
 * @param header 
 * @param fileName 
 */
void BinaryWriter_CreateVehicleFile(Vehicle_t** vehicles, int vehiclesCount, VehicleHeader_t* header, char* fileName);

/**
 * @brief Creates a bus line binary file with the given bus lines.
 * 
 * @param busLines 
 * @param busLinesCount 
 * @param header 
 * @param fileName 
 */
void BinaryWriter_CreateBusLineFile(BusLine_t** busLines, int busLinesCount, BusLineHeader_t* header, char* fileName);


/**
 * @brief Writes a vector of vehicles to the end of dest file.
 * 
 * @param vehicles 
 * @param vehiclesCount 
 * @param fileName 
 * @return int 
 */
int BinaryWriter_IncrementVehicleFile(Vehicle_t** vehicles, int vehiclesCount, char* fileName);

/**
 * @brief Writes a vector of bus lines to the end of dest file.
 * 
 * @param busLines 
 * @param buslinesCount 
 * @param fileName 
 * @return int 
 */
int BinaryWriter_IncrementBusLineFile(BusLine_t** busLines, int buslinesCount, char* fileName);

#endif
