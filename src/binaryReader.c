#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "binaryReader.h"
#include "dataModel.h"
#include "bTreeDataModel.h"
#include "bTree.h"

Vehicle_t* BinaryReader_Vehicle(FILE *srcFile) {
    Vehicle_t* vehicle = calloc(1, sizeof(Vehicle_t));

    vehicle->offset = ftell(srcFile);

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

    busLine->offset = ftell(srcFile);

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

BNode_t* BinaryReader_BTreeNode(BTreeMetadata_t* meta, RRN_t nodeRRN) {
    if (meta == NULL || meta->bTreeIndexFile == NULL) {
        fprintf(stderr, "WARN: Tried to read a B-Tree node but passed a NULL metadata or a metadata without a B-Tree file opened.\n");
        return NULL;
    }

    if (nodeRRN < 0 || nodeRRN >= meta->header->rrnNextNode) {
        printf("WARN: NULL node passed to BinaryReader_BTreeNode.\n");
        return NULL;
    }

    fseek(meta->bTreeIndexFile, RRNToOffset(nodeRRN), SEEK_SET);
    
    // Creates a NULL node
    BNode_t* node = BNode_CreateNull();

    // Reads fields
    fread(&(node->isLeaf), sizeof(char), 1, meta->bTreeIndexFile);
    node->isLeaf -= '0';

    fread(&node->indexedKeysCount, sizeof(int32_t), 1, meta->bTreeIndexFile);
    fread(&node->rrn, sizeof(RRN_t), 1, meta->bTreeIndexFile);

    for (int i = 0; i < BTREE_ORDER-1; i++) {
        fread(&node->childrenRRNs[i], sizeof(RRN_t), 1, meta->bTreeIndexFile);
        fread(&node->keys[i], sizeof(RegKey_t), 1, meta->bTreeIndexFile);
        fread(&node->offsets[i], sizeof(ByteOffset_t), 1, meta->bTreeIndexFile);
    }
    fread(&node->childrenRRNs[BTREE_ORDER-1], sizeof(RRN_t), 1, meta->bTreeIndexFile);

    return node;
}

/**
 * @brief Function that reads the header and root of an bTree.
 * 
 * @param meta The cache of the bTree
 * @return the file status 
 */
int BinaryReader_BTreeHeaderAndRoot(BTreeMetadata_t* meta) {
    meta->header = BHeader_Create(0, -1, -1);

    fread(&(meta->header->status), sizeof(char), 1, meta->bTreeIndexFile);
    fread(&(meta->header->rootRRN), sizeof(RRN_t), 1, meta->bTreeIndexFile);
    fread(&(meta->header->rrnNextNode), sizeof(RRN_t), 1, meta->bTreeIndexFile);
    fread(&(meta->header->unused[0]), sizeof(char), 68, meta->bTreeIndexFile);

    meta->root = BinaryReader_BTreeNode(meta, meta->header->rootRRN);
    return meta->header->status;
}
