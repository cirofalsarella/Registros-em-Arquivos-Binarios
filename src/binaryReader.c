#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "dataModel.h"
#include "bTreeDataModel.h"
#include "bTree.h"

Vehicle_t* BinaryReader_Vehicle(FILE *srcFile) {
    Vehicle_t* vehicle = calloc(1, sizeof(Vehicle_t));

    //  Fixed length fields
    fread(&vehicle->removed, sizeof(char), 1, srcFile);
    
    fread(&vehicle->regSize, sizeof(int32_t), 1, srcFile);  // 4
    fread(&vehicle->prefix[0], sizeof(char), 5, srcFile);   // 5
    fread(&vehicle->date[0], sizeof(char), 10, srcFile);    // 10
    fread(&vehicle->numSeats, sizeof(int32_t), 1, srcFile); // 4
    fread(&vehicle->lineCode, sizeof(int32_t), 1, srcFile); // 4


    // Variable length fields
    fread(&vehicle->modelLength, sizeof(int32_t), 1, srcFile);
    vehicle->model = calloc(vehicle->modelLength+1, sizeof(char));
    fread(vehicle->model, sizeof(char), vehicle->modelLength, srcFile);

    fread(&vehicle->categoryLength, sizeof(int32_t), 1, srcFile);
    vehicle->category = calloc(vehicle->categoryLength+1, sizeof(char));
    fread(vehicle->category, sizeof(char), vehicle->categoryLength, srcFile);

    return vehicle;
}

BusLine_t* BinaryReader_BusLine(FILE *srcFile) {
    BusLine_t* busLine = calloc(1, sizeof(BusLine_t));

    //  Fixed length fields
    fread(&busLine->removed, sizeof(char), 1, srcFile);
    fread(&busLine->regSize, sizeof(int32_t), 1, srcFile);

    fread(&busLine->lineCode, sizeof(int32_t), 1, srcFile);
    fread(&busLine->acceptsCreditCard, sizeof(char), 1, srcFile);

    // Variable length fields
    fread(&busLine->nameLength, sizeof(int32_t), 1, srcFile);
    busLine->name = calloc(busLine->nameLength+1, sizeof(char));
    fread(busLine->name, sizeof(char), busLine->nameLength, srcFile);

    fread(&busLine->colorLength, sizeof(int32_t), 1, srcFile);
    busLine->color = calloc(busLine->colorLength+1, sizeof(char));
    fread(busLine->color, sizeof(char), busLine->colorLength, srcFile);

    return busLine;
}

VehicleHeader_t* BinaryReader_VehicleHeader(FILE *srcFile) {
    VehicleHeader_t* header = calloc(1, sizeof(VehicleHeader_t));

    fread(&header->status, sizeof(char), 1, srcFile);
    fread(&header->nextReg, sizeof(int64_t), 1, srcFile);
    fread(&header->numReg, sizeof(int32_t), 1, srcFile);
    fread(&header->numRegRemov, sizeof(int32_t), 1, srcFile);

    fread(&header->describePrefix, sizeof(char), 18, srcFile);
    fread(&header->describeDate, sizeof(char), 35, srcFile);
    fread(&header->describePlaces, sizeof(char), 42, srcFile);
    fread(&header->describeLine, sizeof(char), 26, srcFile);
    fread(&header->describeModel, sizeof(char), 17, srcFile);
    fread(&header->describeCategory, sizeof(char), 20, srcFile);

    return header;
}

BusLineHeader_t* BinaryReader_BusLineHeader(FILE *srcFile) {
    BusLineHeader_t* header = calloc(1, sizeof(VehicleHeader_t));

    fread(&header->status, sizeof(char), 1, srcFile);
    fread(&header->nextReg, sizeof(int64_t), 1, srcFile);
    fread(&header->numReg, sizeof(int32_t), 1, srcFile);
    fread(&header->numRegRemov, sizeof(int32_t), 1, srcFile);

    fread(&header->describeCode, sizeof(char), 15, srcFile);
    fread(&header->describeCard, sizeof(char), 13, srcFile);
    fread(&header->describeName, sizeof(char), 13, srcFile);
    fread(&header->describeLine, sizeof(char), 24, srcFile);

    return header;
}

Vehicle_t** BinaryReader_Vehicles(VehicleHeader_t** header, char* fileName) {
    FILE* srcFile = fopen(fileName, "rb");
    if (srcFile == NULL) {
        return NULL;
    } else {
        char status;
        fread(&status, sizeof(char), 1, srcFile);
        if (status == '0'){
            fclose(srcFile);
            return NULL;
        }
        fseek(srcFile, 0, SEEK_SET);
    }

    // Reads the header
    *header = BinaryReader_VehicleHeader(srcFile);

    // Allocates space for the vehicles
    Vehicle_t** vehicles = calloc((*header)->numReg, sizeof(Vehicle_t*));
    int n_registers = (*header)->numReg + (*header)->numRegRemov;

    // Gets the vehicles that aren't removed from the file
    int j = 0;
    for (int i=0; i < n_registers; i++) {
        Vehicle_t* aux = BinaryReader_Vehicle(srcFile);
        if (aux != NULL) {
            if (aux->removed == '0') {
                Vehicle_Free(aux);
            } else {
                vehicles[j] = aux;
                j++;
            }
        }
    }

    fclose(srcFile);
    return vehicles;
}

BusLine_t** BinaryReader_BusLines(BusLineHeader_t** header, char* fileName) {
    FILE* srcFile = fopen(fileName, "rb");
    if (srcFile == NULL) {
        return NULL;
    } else {
        char status;
        fread(&status, sizeof(char), 1, srcFile);
        if (status == '0'){
            fclose(srcFile);
            return NULL;
        }
        fseek(srcFile, 0, SEEK_SET);
    }
    
    // Reads the header
    *header = BinaryReader_BusLineHeader(srcFile);

    // Allocates space for the bus lines
    BusLine_t** busLines = calloc((*header)->numReg, sizeof(BusLine_t*));
    int n_registers = (*header)->numReg + (*header)->numRegRemov;

    // Gets the bus lines that arent removed from the file
    int j = 0;
    for (int i=0; i < n_registers; i++) {
        BusLine_t* aux = BinaryReader_BusLine(srcFile);
        if (aux != NULL) {
            if (aux->removed == '0') {
                BusLine_Free(aux);
            } else {
                busLines[j] = aux;
                j++;
            }
        }
    }

    fclose(srcFile);
    return busLines;
}




BNode_t* BinaryReader_BTreeNode(BTreeCache_t* cache, RRN_t nodeRRN) {
    if (cache == NULL || cache->index == NULL)   return NULL;
    
    if (cache->nodes[nodeRRN-1] != NULL) {
        return cache->nodes[nodeRRN-1];
    }

    fseek(cache->index, RRNToOffset(nodeRRN), SEEK_SET);
    
    // Creates a NULL node
    BNode_t* node = BNode_CreateNoChildren(-1, -1);

    // Reads fields
    fread(&node->isLeaf, sizeof(char), 1, cache->index);
    fread(&node->indexedKeysCount, sizeof(int32_t), 1, cache->index);
    fread(&node->rrn, sizeof(RRN_t), 1, cache->index);

    for (int i=0; i<BTREE_ORDER-2; i++) {
        fread(&node->childrenRRNs[i], sizeof(RRN_t), 1, cache->index);
        fread(&node->regKeys[i], sizeof(RegKey_t), 1, cache->index);
        fread(&node->regOffsets[i], sizeof(ByteOffset_t), 1, cache->index);
    }
    fread(&node->childrenRRNs[BTREE_ORDER-1], sizeof(RRN_t), 1, cache->index);

    return node;
}

/**
 * @brief Function that reads the header and root of an bTree.
 * 
 * @param cache The cache of the bTree
 * @return the file status 
 */
int BinaryReader_BTreeHeaderAndRoot(BTreeCache_t* cache) {
    cache->header = BHeader_Create(0, -1, -1);

    fread(&(cache->header->status), sizeof(char), 1, cache->index);
    fread(&(cache->header->rootRRN), sizeof(RRN_t), 1, cache->index);
    fread(&(cache->header->rrnNextNode), sizeof(RRN_t), 1, cache->index);
    fread(&(cache->header->unused[0]), sizeof(char), 68, cache->index);

    cache->root = BinaryReader_BTreeNode(cache, cache->header->rootRRN);
    cache->nodes[cache->header->rootRRN-1] = cache->root;

    return cache->header->status;
}
