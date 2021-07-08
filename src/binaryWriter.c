
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include "dataModel.h"
#include "binaryWriter.h"
#include "binaryReader.h"
#include "binaryHeaders.h"
#include "utils.h"
#include "bTree.h"
#include "bTreeDataModel.h"

// TODO: Manipulação do status / ler feedback do T1


// void FileSeek(FILE* file, ByteOffset_t offset) {
//     int seekStatus = fseek(file, offset, SEEK_SET);
//     if (seekStatus != 0) {
//         if (seekStatus == EBADF) {
//             fprintf(stderr, "The file descriptor underlying the stream file is not open for writing or the stream's buffer needed to be flushed and the file is not open. Offset: %d\n", offset);
//         } else if (seekStatus == EINVAL) {
//             fprintf(stderr, "The whence argument is invalid. The resulting file-position indicator would be set to a negative value. Offset: %I64d\n", offset);
//         } else if (seekStatus == EFBIG) {
//             fprintf(stderr, "An attempt was made to write a file that exceeds the maximum file size or the process's file size limit, or the file is a regular file and an attempt was made to write at or beyond the offset maximum associated with the corresponding stream. Offset: %I64d\n", offset);
//         } else {
//             if (file == NULL) {
//                 fprintf(stderr, "Tentou-se fazer fseek em um arquivo NULO. Offset: %I64d\n", offset);
//             } else {
//                 fprintf(stderr, "Um erro desconhecido ocorreu durante o fseek. Offset: %I64d\n", offset);
//             }
//         }
//     }
// }


//  MARK: Functions that write single registers

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
    char folha = (node->isLeaf) ? '1' : '0';
    fwrite(&folha, sizeof(char), 1, destFile);
    fwrite(&node->indexedKeysCount, sizeof(int32_t), 1, destFile);
    fwrite(&node->rrn, sizeof(RRN_t), 1, destFile);

    for (int i = 0; i < BTREE_ORDER-2; i++) {
        fwrite(&node->childrenRRNs[i], sizeof(RRN_t), 1, destFile);
        fwrite(&node->regKeys[i], sizeof(RegKey_t), 1, destFile);
        fwrite(&node->regOffsets[i], sizeof(ByteOffset_t), 1, destFile);
    }
    fwrite(&node->childrenRRNs[BTREE_ORDER-2], sizeof(RRN_t), 1, destFile);
}


// MARK: Header-writing functions

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

void BinaryWriter_BTreeHeader(BTreeMetadata_t* meta) {    
    fseek(meta->bTreeIndexFile, 0, SEEK_SET);

    fwrite(&(meta->header->status), sizeof(char), 1, meta->bTreeIndexFile);

    fwrite(&(meta->header->rootRRN), sizeof(RRN_t), 1, meta->bTreeIndexFile);
    fwrite(&(meta->header->rrnNextNode), sizeof(RRN_t), 1, meta->bTreeIndexFile);
    
    fwrite(&(meta->header->unused[0]), sizeof(char), 68, meta->bTreeIndexFile);
}

// MARK: Append files

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

int BinaryWriter_SeekAndWriteNode(BNode_t* node, BTreeMetadata_t* meta) {
    if (meta == NULL || meta->bTreeIndexFile == NULL || node == NULL || node->rrn < 0) {
        fprintf(stderr, "Invalid call to BinaryWriter_SeekAndWriteNode. Either the node is invalid or the metadata is invalid.\n");
        return 1;
    }

    fseek(meta->bTreeIndexFile, RRNToOffset(node->rrn), SEEK_SET);
    WriteBTreeNode(node, meta->bTreeIndexFile);

    return 0;
}

void BinaryWriter_BTreeIndexFileVehicles(BTreeMetadata_t* meta) {
    // Checks if the file does not exist
    if (meta->registersFile == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    VehicleHeader_t* regsHeader = BinaryReader_VehicleHeader(meta->registersFile);
    
    // Checks if the file does not exist
    if (regsHeader == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // Inserts the vehicles
    for (int i = 0; i < regsHeader->numReg; i++) {
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
}

void BinaryWriter_BTreeIndexFileBusLines(BTreeMetadata_t* meta) {
    // Checks if the file does not exist
    if (meta->registersFile == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    BusLineHeader_t* regsHeader = BinaryReader_BusLineHeader(meta->registersFile);
    
    // Checks if the file does not exist
    if (regsHeader == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // Inserts the bus lines
    for (int i = 0; i < regsHeader->numReg; i++) {
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
}