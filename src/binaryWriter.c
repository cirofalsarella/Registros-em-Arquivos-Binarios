
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "dataModel.h"
#include "stringTable.h"
#include "binaryHeaders.h"

#include "bTree.h"
#include "bTreeDataModel.h"


/*  TODO:
 *      BinaryWriter_BTreeIndexFile
 *
 */


//  Escrevem Registros Únicos

void WriteVehicle(const Vehicle_t* vehicle, FILE* destFile) {
    // Fixed length fields
    fwrite(&vehicle->removed, sizeof(char), 1, destFile);
    fwrite(&vehicle->regSize, sizeof(int32_t), 1, destFile);
    fwrite(&vehicle->prefix[0], sizeof(char), 5, destFile);
    fwrite(&vehicle->date[0], sizeof(char), 10, destFile);
    fwrite(&vehicle->numSeats, sizeof(int32_t), 1, destFile);
    fwrite(&vehicle->lineCode, sizeof(int32_t), 1, destFile);


    // Variable-length fields
    //  if the field is NULL, length == 0
    fwrite(&vehicle->modelLength, sizeof(int32_t), 1, destFile);
    if (vehicle->model != NULL)
        fwrite(vehicle->model, sizeof(char), vehicle->modelLength, destFile);

    fwrite(&vehicle->categoryLength, sizeof(int32_t), 1, destFile);
    if (vehicle->category != NULL)
        fwrite(vehicle->category, sizeof(char), vehicle->categoryLength, destFile);
}

void WriteBusLine(const BusLine_t* busLine, FILE* destFile) {
    // Fixed length fields
    fwrite(&busLine->removed, sizeof(char), 1, destFile);
    fwrite(&busLine->regSize, sizeof(int32_t), 1, destFile);
    fwrite(&busLine->lineCode, sizeof(int32_t), 1, destFile);
    fwrite(&busLine->acceptsCreditCard, sizeof(char), 1, destFile);

    // Variable-length fields
    //  if the field is NULL, length == 0
    fwrite(&busLine->nameLength, sizeof(int32_t), 1, destFile);
    if (busLine->name != NULL)
        fwrite(busLine->name, sizeof(char), busLine->nameLength, destFile);

    fwrite(&busLine->colorLength, sizeof(int32_t), 1, destFile);
    if (busLine->color != NULL)
        fwrite(busLine->color, sizeof(char), busLine->colorLength, destFile);
}

void WriteBTreeNode(const BNode_t* node, FILE* destFile){
    fwrite(&node->isLeaf, sizeof(char), 1, destFile);
    fwrite(&node->indexedKeysCount, sizeof(int32_t), 1, destFile);
    fwrite(&node->rrn, sizeof(RRN), 1, destFile);

    for (int i=0; i<BTREE_ORDER-1; i++) {
        fwrite(&node->childrenRRNs[i], sizeof(RRN), 1, destFile);
        fwrite(&node->regKeys[i], sizeof(REGKEY), 1, destFile);
        fwrite(&node->regOffsets[i], sizeof(OFFSET), 1, destFile);
    }
    fwrite(&node->childrenRRNs[BTREE_ORDER-1], sizeof(RRN), 1, destFile);
}


//  Escrevem um Header

void WriteVehicleHeader(const VehicleHeader_t* header, FILE *destFile) {
    fwrite(&header->status, sizeof(char), 1, destFile);
    fwrite(&header->nextReg, sizeof(int64_t), 1, destFile);
    fwrite(&header->numReg, sizeof(int32_t), 1, destFile);
    fwrite(&header->numRegRemov, sizeof(int32_t), 1, destFile);

    // Strings with always the same sizes
    fwrite(&header->describePrefix[0], sizeof(char), 18, destFile);
    fwrite(&header->describeDate[0], sizeof(char), 35, destFile);
    fwrite(&header->describePlaces[0], sizeof(char), 42, destFile);
    fwrite(&header->describeLine[0], sizeof(char), 26, destFile);
    fwrite(&header->describeModel[0], sizeof(char), 17, destFile);
    fwrite(&header->describeCategory[0], sizeof(char), 20, destFile);
}

void WriteBusLineHeader(const BusLineHeader_t* header, FILE *destFile) {
    fwrite(&header->status, sizeof(char), 1, destFile);
    fwrite(&header->nextReg, sizeof(int64_t), 1, destFile);
    fwrite(&header->numReg, sizeof(int32_t), 1, destFile);
    fwrite(&header->numRegRemov, sizeof(int32_t), 1, destFile);

    // Strings with always the same sizes
    fwrite(&header->describeCode[0], sizeof(char), 15, destFile);
    fwrite(&header->describeCard[0], sizeof(char), 13, destFile);
    fwrite(&header->describeName[0], sizeof(char), 13, destFile);
    fwrite(&header->describeLine[0], sizeof(char), 24, destFile);
}

void BinaryWriter_BTreeHeader(BTreeCache_t* cache) {
    FILE* destFile = fopen(cache->bTreeIndexFileName, "rb+");
    // Já foi conferido que o arquivo existe e que seu status é válido
    
    // Set as editing mode
    char status = '0';
    fwrite(&status, sizeof(char), 1, destFile);
    
    // Writing header
    fread(cache->header->rootRRN, sizeof(RRN), 1, destFile);
    fread(cache->header->rrnNextNode, sizeof(RRN), 1, destFile);
    fread(cache->header->unused[0], sizeof(char), 68, destFile);

    // Set as default mode
    fseek(destFile, 0, SEEK_SET);
    status = '1';
    fwrite(&status, sizeof(char), 1, destFile);
    
    // Exiting
    fclose(destFile);
}

//  Escrevem um arquivo inteiro

void BinaryWriter_CreateVehicleFile(Vehicle_t** vehicles, int vehiclesCount, VehicleHeader_t* header, char* fileName) {
    header->status = '0';
    // These are adjusted throughout this function
    header->numReg = vehiclesCount;
    header->numRegRemov = 0;
    header->nextReg = 0;

    // First, calculates number of removed registers
    for (int i = 0; i < vehiclesCount; i++) {
        if (vehicles[i]->removed == '0') {
            header->numRegRemov++;
            header->numReg--;
        }
    }

    // Opens the file and writes the header
    FILE* destFile = fopen(fileName, "wb");
    WriteVehicleHeader(header, destFile);

    for (int i = 0; i < vehiclesCount; i++){
        WriteVehicle(vehicles[i], destFile);
        Vehicle_Free(vehicles[i]);
    }

    // Calculates and writes nextReg
    fseek(destFile, 0, SEEK_END);
    header->nextReg = ftell(destFile);
    fseek(destFile, 1, SEEK_SET);
    fwrite(&header->nextReg, sizeof(int64_t), 1, destFile);

    // Writes status
    fseek(destFile, 0, SEEK_SET);
    header->status = '1';
    fwrite(&header->status, 1, 1, destFile);

    // Frees and closes everything
    fclose(destFile);
    free(vehicles);
    BinaryHeaders_FreeVehicleHeader(header);
}

void BinaryWriter_CreateBusLineFile(BusLine_t** busLines, int busLinesCount, BusLineHeader_t* header, char* fileName) {
    header->status = '0';
    // These are adjusted throughout this function
    header->numReg = busLinesCount;
    header->numRegRemov = 0;
    header->nextReg = 0;

    // First, calculates number of removed registers
    for (int i = 0; i < busLinesCount; i++) {
        if (busLines[i]->removed == '0') {
            header->numRegRemov++;
            header->numReg--;
        }
    }

    // Opens the file and writes the header
    FILE* destFile = fopen(fileName, "wb");
    WriteBusLineHeader(header, destFile);

    for (int i = 0; i < busLinesCount; i++){
        WriteBusLine(busLines[i], destFile);
        BusLine_Free(busLines[i]);
    }

    // Calculates and writes nextReg
    fseek(destFile, 0, SEEK_END);
    header->nextReg = ftell(destFile);
    fseek(destFile, 1, SEEK_SET);
    fwrite(&header->nextReg, sizeof(int64_t), 1, destFile);

    // Writes status
    fseek(destFile, 0, SEEK_SET);
    header->status = '1';
    fwrite(&header->status, 1, 1, destFile);

    // Frees and closes everything
    fclose(destFile);
    free(busLines);
    BinaryHeaders_FreeBusLineHeader(header);
}

void BinaryWriter_BTreeIndexFile(BTreeCache_t* cache){
    // TODO
}


// Incrementam um arquivo com novos registros

int BinaryWriter_IncrementVehicleFile(Vehicle_t** vehicles, int vehiclesCount, char* fileName) {
    FILE *destFile = fopen(fileName, "rb+");
    if (destFile == NULL) {
        return 1;
    }  else {
        char status;
        fread(&status, sizeof(char), 1, destFile);
        if (status == '0'){
            fclose(destFile);
            return 1;
        }
        fseek(destFile, 0, SEEK_SET);
    }

    // Set as editing mode
    char status = '0';
    fwrite(&status, sizeof(char), 1, destFile);

    // getting the header intel
    int64_t proxReg;
    fread(&proxReg, sizeof(int64_t), 1, destFile);
    int32_t numReg, numRegRem;
    fread(&numReg, sizeof(int32_t), 1, destFile);
    fread(&numRegRem, sizeof(int32_t), 1, destFile);


    // Writing the registers
    fseek(destFile, proxReg, SEEK_SET);
    for (int i = 0; i < vehiclesCount; i++){
        WriteVehicle(vehicles[i], destFile);
        if (vehicles[i]->removed == '0') {
            numRegRem++;
        } else {
            numReg++;
        }
        Vehicle_Free(vehicles[i]);
    }
    proxReg = ftell(destFile);


    // Adjusting the header fields
    fseek(destFile, 1, SEEK_SET);
    fwrite(&proxReg, sizeof(int64_t), 1, destFile);
    fwrite(&numReg, sizeof(int32_t), 1, destFile);
    fwrite(&numRegRem, sizeof(int32_t), 1, destFile);

    // Set as default mode
    status = '1';
    fseek(destFile, 0, SEEK_SET);
    fwrite(&status, sizeof(char), 1, destFile);

    fclose(destFile);
    return 0;
}

int BinaryWriter_IncrementBusLineFile(BusLine_t** buslines, int buslinesCount, char* fileName) {
    FILE *destFile = fopen(fileName, "rb+");
    if (destFile == NULL) {
        return 1;
    } else {
        char status;
        fread(&status, sizeof(char), 1, destFile);
        if (status == '0'){
            fclose(destFile);
            return 1;
        }
        fseek(destFile, 0, SEEK_SET);
    }


    // Set as editing mode
    char status = '0';
    fwrite(&status, sizeof(char), 1, destFile);


    // getting the header intel
    int64_t proxReg;
    fread(&proxReg, sizeof(int64_t), 1, destFile);
    int32_t numReg, numRegRem;
    fread(&numReg, sizeof(int32_t), 1, destFile);
    fread(&numRegRem, sizeof(int32_t), 1, destFile);


    // Writing the registers
    fseek(destFile, proxReg, SEEK_SET);
    for (int i = 0; i < buslinesCount; i++){
        WriteBusLine(buslines[i], destFile);
        if (buslines[i]->removed == '0') {
            numRegRem++;
        } else {
            numReg++;
        }
        BusLine_Free(buslines[i]);
    }
    proxReg = ftell(destFile);


    // Adjusting the header fields
    fseek(destFile, 1, SEEK_SET);
    fwrite(&proxReg, sizeof(int64_t), 1, destFile);
    fwrite(&numReg, sizeof(int32_t), 1, destFile);
    fwrite(&numRegRem, sizeof(int32_t), 1, destFile);

    // Set as default mode
    status = '1';
    fseek(destFile, 0, SEEK_SET);
    fwrite(&status, sizeof(char), 1, destFile);

    fclose(destFile);
    return 0;
}

int BinaryWriter_IncrementBtree(BNode_t* node, BTreeCache_t* cache){
    FILE* destFile = fopen(cache->bTreeIndexFileName, "rb+");
    // Já foi conferido que o arquivo existe e que seu status é válido
    
    // Set as editing mode
    char status = '0';
    fwrite(&status, sizeof(char), 1, destFile);
    
    // Writing node
    fseek(destFile, RRNToOffset(node->rrn), SEEK_SET);
    WriteBTreeNode(node, destFile);

    // Set as default mode
    fseek(destFile, 0, SEEK_SET);
    status = '1';
    fwrite(&status, sizeof(char), 1, destFile);
    
    // Exiting
    fclose(destFile);
    return 0;
}

