
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include "binaryWriter.h"
#include "binaryReader.h"
#include "../dataModel/dataModel.h"
#include "../dataModel/binaryHeaders.h"
#include "../core/utils.h"
#include "../bTree/bTree.h"
#include "../core/operations.h"
#include "../bTree/bTreeDataModel.h"

// ANCHOR: Register writing functions

void BinaryWriter_Vehicle(const Vehicle_t* vehicle, FILE* destFile) {
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

void BinaryWriter_BusLine(const BusLine_t* busLine, FILE* destFile) {
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
    char folha = node->isLeaf + '0';

    fwrite(&folha, sizeof(char), 1, destFile);
    fwrite(&node->indexedKeysCount, sizeof(int32_t), 1, destFile);
    fwrite(&node->rrn, sizeof(RRN_t), 1, destFile);

    for (int i = 0; i < BTREE_ORDER-1; i++) {
        fwrite(&node->childrenRRNs[i], sizeof(RRN_t), 1, destFile);
        fwrite(&node->keys[i], sizeof(RegKey_t), 1, destFile);
        fwrite(&node->offsets[i], sizeof(ByteOffset_t), 1, destFile);
    }
    fwrite(&node->childrenRRNs[BTREE_ORDER-1], sizeof(RRN_t), 1, destFile);
}


// MARK: Header-writing functions

void WriteVehicleHeader(const VehicleHeader_t* header, FILE *destFile) {
    fwrite(&header->nextReg, sizeof(int64_t), 1, destFile);
    fwrite(&header->validRegCount, sizeof(int32_t), 1, destFile);
    fwrite(&header->removedRegCount, sizeof(int32_t), 1, destFile);

    // Strings with always the same sizes
    fwrite(&header->describePrefix[0], sizeof(char), 18, destFile);
    fwrite(&header->describeDate[0], sizeof(char), 35, destFile);
    fwrite(&header->describePlaces[0], sizeof(char), 42, destFile);
    fwrite(&header->describeLine[0], sizeof(char), 26, destFile);
    fwrite(&header->describeModel[0], sizeof(char), 17, destFile);
    fwrite(&header->describeCategory[0], sizeof(char), 20, destFile);
}

void WriteBusLineHeader(const BusLineHeader_t* header, FILE *destFile) {
    fwrite(&header->nextReg, sizeof(int64_t), 1, destFile);
    fwrite(&header->validRegCount, sizeof(int32_t), 1, destFile);
    fwrite(&header->removedRegCount, sizeof(int32_t), 1, destFile);

    // Strings with always the same sizes
    fwrite(&header->describeCode[0], sizeof(char), 15, destFile);
    fwrite(&header->describeCard[0], sizeof(char), 13, destFile);
    fwrite(&header->describeName[0], sizeof(char), 13, destFile);
    fwrite(&header->describeLine[0], sizeof(char), 24, destFile);
}

void BinaryWriter_BTreeHeader(BTreeMetadata_t* meta) {    
    fseek(meta->bTreeIndexFile, 0, SEEK_SET);

    fwrite(&(meta->header->status), sizeof(char), 1, meta->bTreeIndexFile);

    fwrite(&(meta->header->rootRRN), sizeof(RRN_t), 1, meta->bTreeIndexFile);
    fwrite(&(meta->header->rrnNextNode), sizeof(RRN_t), 1, meta->bTreeIndexFile);
    
    fwrite(&(meta->header->unused[0]), sizeof(char), 68, meta->bTreeIndexFile);
}


// ANCHOR: File Writing functions

void BinaryWriter_VehicleFile(Vehicle_t** vehicles, int n_vehicles, const char* fileName) {
    VehicleHeader_t* header = BinaryHeaders_CreateVehicleHeader(0, 0, 0);
    
    // Opens the file and writes the header
    FILE* destFile = fopen(fileName, "wb");
    char status = '0';
    fwrite(&status, sizeof(char), 1, destFile);
    fseek(destFile, VEHICLE_HEADER_SIZE-1, SEEK_CUR);

    for (int i = 0; i < n_vehicles; i++){
        BinaryWriter_Vehicle(vehicles[i], destFile);
        if (vehicles[i]->removed == '1')    header->removedRegCount++;
        else                                header->validRegCount++;
    }

    // Writes nextReg
    header->nextReg = ftell(destFile);
    fseek(destFile, 1, SEEK_SET);
    fwrite(&header->nextReg, sizeof(int64_t), 1, destFile);

    // Writes status
    fseek(destFile, 0, SEEK_SET);
    status = '1';
    fwrite(&status, 1, 1, destFile);
    WriteVehicleHeader(header, destFile);

    // Frees and closes everything
    fclose(destFile);
    BinaryHeaders_FreeVehicleHeader(header);
}

void BinaryWriter_BusLineFile(BusLine_t** buslines, int n_buslines, const char* fileName) {
    BusLineHeader_t* header = BinaryHeaders_CreateBusLineHeader(0, n_buslines, 0);
    
    // Opens the file and writes the header
    FILE* destFile = fopen(fileName, "wb");
    char status = '0';
    fwrite(&status, sizeof(char), 1, destFile);
    fseek(destFile, BUSLINE_HEADER_SIZE-1, SEEK_CUR);

    for (int i = 0; i < n_buslines; i++){
        BinaryWriter_BusLine(buslines[i], destFile);
        if (buslines[i]->removed == '0')    header->validRegCount++;
    }

    // Writes nextReg
    header->nextReg = ftell(destFile);
    fseek(destFile, 1, SEEK_SET);
    fwrite(&header->nextReg, sizeof(int64_t), 1, destFile);

    // Writes status
    fseek(destFile, 0, SEEK_SET);
    status = '1';
    fwrite(&status, 1, 1, destFile);
    WriteBusLineHeader(header, destFile);

    // Frees and closes everything
    fclose(destFile);
    BinaryHeaders_FreeBusLineHeader(header);
}


// ANCHOR: Register appending functions

int BinaryWriter_AppendVehicles(Vehicle_t** vehicles, int vehiclesCount, FILE* destFile, ByteOffset_t* destOffsets) {
    if (destFile == NULL) {
        return 1;
    }  else {
        char status;
        fread(&status, sizeof(char), 1, destFile);
        if (status == '0') {
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
    for (int i = 0; i < vehiclesCount; i++) {
        destOffsets[i] = ftell(destFile);

        BinaryWriter_Vehicle(vehicles[i], destFile);
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

int BinaryWriter_AppendBusLines(BusLine_t** buslines, int buslinesCount, FILE* destFile, ByteOffset_t* destOffsets) {
    if (destFile == NULL) {
        return 1;
    } else {
        char status;
        fread(&status, sizeof(char), 1, destFile);
        if (status == '0') {
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
        destOffsets[i] = ftell(destFile);

        BinaryWriter_BusLine(buslines[i], destFile);
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


// ANCHOR: B-Tree related writing

int BinaryWriter_SeekAndWriteNode(BNode_t* node, BTreeMetadata_t* meta) {
    if (meta == NULL || meta->bTreeIndexFile == NULL || node == NULL || node->rrn < 0) {
        fprintf(stderr, "Invalid call to BinaryWriter_SeekAndWriteNode. Either the node is invalid or the metadata is invalid.\n");
        return 1;
    }

    fseek(meta->bTreeIndexFile, RRNToOffset(node->rrn), SEEK_SET);
    WriteBTreeNode(node, meta->bTreeIndexFile);

    return 0;
}

int BinaryWriter_BTreeIndexFileVehicles(BTreeMetadata_t* meta) {
    // Checks if the file does not exist
    if (meta->registersFile == NULL) {
        return 1;
    }

    VehicleHeader_t* regsHeader = BinaryReader_VehicleHeader(meta->registersFile);
    if (regsHeader == NULL) {
        return 1;
    }


    // Inserts the vehicles
    for (int i = 0; i < regsHeader->validRegCount + regsHeader->removedRegCount; i++) {
        Vehicle_t* reg = BinaryReader_Vehicle(meta->registersFile);
        if (reg->removed == '0') {
            Vehicle_Free(reg);
            continue;
        }

        const RegKey_t key = Utils_VehiclePrefixHash(reg->prefix);
        BTreeMetadata_Insert(meta, key, reg->offset);
        Vehicle_Free(reg);
    }


    BinaryWriter_BTreeHeader(meta);
    BinaryHeaders_FreeVehicleHeader(regsHeader);
    return 0;
}

int BinaryWriter_BTreeIndexFileBusLines(BTreeMetadata_t* meta) {
    // Checks if the file does not exist
    if (meta->registersFile == NULL) {
        return 1;
    }

    BusLineHeader_t* regsHeader = BinaryReader_BusLineHeader(meta->registersFile);
    if (regsHeader == NULL) {
        return 1;
    }

    // Inserts the bus lines
    for (int i = 0; i < regsHeader->validRegCount + regsHeader->removedRegCount; i++) {
        BusLine_t* reg = BinaryReader_BusLine(meta->registersFile);
        if (reg->removed == '0') {
            BusLine_Free(reg);
            continue;
        }

        const RegKey_t key = reg->lineCode;
        BTreeMetadata_Insert(meta, key, reg->offset);
        BusLine_Free(reg);
    }

    BinaryWriter_BTreeHeader(meta);
    BinaryHeaders_FreeBusLineHeader(regsHeader);
    
    return 0;
}
