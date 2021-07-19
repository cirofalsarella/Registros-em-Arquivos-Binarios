#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "../fileIO/binaryReader.h"
#include "../dataModel/dataModel.h"
#include "../dataModel/binaryHeaders.h"
#include "../fileIO/binaryWriter.h"
#include "../bTree/bTreeDataModel.h"
#include "../bTree/bTree.h"
#include "../core/printer.h"


// ANCHOR: Read registers & reg file headers

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

char BinaryReader_ValidateStatus(FILE* file) {
    if (file == NULL) return FALSE;

    char status;    
    fseek(file, 0, SEEK_SET);
    fread(&status, sizeof(char), 1, file);
    if (status == '1') return TRUE; // Everything is OK
    else {
        fclose(file); // File is corrupted
        return FALSE;
    }
}

VehicleHeader_t* BinaryReader_VehicleHeader(FILE *srcFile) {
    VehicleHeader_t* header = calloc(1, sizeof(VehicleHeader_t));

    fread(&header->nextReg, sizeof(int64_t), 1, srcFile);
    fread(&header->validRegCount, sizeof(int32_t), 1, srcFile);
    fread(&header->removedRegCount, sizeof(int32_t), 1, srcFile);

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

    fread(&header->nextReg, sizeof(int64_t), 1, srcFile);
    fread(&header->validRegCount, sizeof(int32_t), 1, srcFile);
    fread(&header->removedRegCount, sizeof(int32_t), 1, srcFile);

    fread(&header->describeCode, sizeof(char), 15, srcFile);
    fread(&header->describeCard, sizeof(char), 13, srcFile);
    fread(&header->describeName, sizeof(char), 13, srcFile);
    fread(&header->describeLine, sizeof(char), 24, srcFile);

    return header;
}

// ANCHOR

Vehicle_t** BinaryReader_Vehicles(const char* fileName, int* n_vehicles) {
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

        status = '0';
        fwrite(&status, sizeof(char), 1, srcFile);
    }

    // Reads the header
    fseek(srcFile, 1, SEEK_SET);
    VehicleHeader_t* header = BinaryReader_VehicleHeader(srcFile);

    if (!BinaryHeaders_IsVehicleHeaderValid(header)) {
        BinaryHeaders_FreeVehicleHeader(header);
        char status = '1';

        fseek(srcFile, 0, SEEK_SET);
        fwrite(&status, sizeof(char), 1, srcFile);
        fclose(srcFile);
        return NULL;
    }

    // Allocates space for the vehicles
    Vehicle_t** vehicles = calloc((header)->validRegCount, sizeof(Vehicle_t*));
    int n_registers = (header)->validRegCount + (header)->removedRegCount;
    *n_vehicles = header->validRegCount;

    // Gets the vehicles that arent removed from the file
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

    fseek(srcFile, 0, SEEK_SET);
    char status = '1';
    fwrite(&status, sizeof(char), 1, srcFile);
    
    fclose(srcFile);
    BinaryHeaders_FreeVehicleHeader(header);
    return vehicles;
}

BusLine_t** BinaryReader_BusLines(const char* fileName, int* n_buslines) {
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

        status = '0';
        fwrite(&status, sizeof(char), 1, srcFile);
    }

    // Reads the header
    fseek(srcFile, 1, SEEK_SET);
    BusLineHeader_t* header = BinaryReader_BusLineHeader(srcFile);

    if (!BinaryHeaders_IsBusLineHeaderValid(header)) {
        BinaryHeaders_FreeBusLineHeader(header);
        fclose(srcFile);
        char status = '1';

        fseek(srcFile, 0, SEEK_SET);
        fwrite(&status, sizeof(char), 1, srcFile);
        fclose(srcFile);
        return NULL;
    }

    // Allocates space for the buslines
    BusLine_t** buslines = calloc((header)->validRegCount, sizeof(BusLine_t*));
    int n_registers = (header)->validRegCount + (header)->removedRegCount;
    *n_buslines = header->validRegCount;

    // Gets the buslines that arent removed from the file
    int j = 0;
    for (int i=0; i < n_registers; i++) {
        BusLine_t* aux = BinaryReader_BusLine(srcFile);
        if (aux != NULL) {
            if (aux->removed == '0') {
                BusLine_Free(aux);
            } else {
                buslines[j] = aux;
                j++;
            }
        }
    }

    fseek(srcFile, 0, SEEK_SET);
    char status = '1';
    fwrite(&status, sizeof(char), 1, srcFile);
    
    fclose(srcFile);
    BinaryHeaders_FreeBusLineHeader(header);
    return buslines;
}


// ANCHOR: B-Tree header & nodes

BNode_t* BinaryReader_BTreeNode(BTreeMetadata_t* meta, RRN_t nodeRRN) {
    if (meta == NULL || meta->bTreeIndexFile == NULL) {
        fprintf(stderr, "WARN: Tried to read a B-Tree node but passed a NULL metadata or a metadata without a B-Tree file opened.\n");
        return NULL;
    }

    if (nodeRRN < 0 || nodeRRN >= meta->header->rrnNextNode) {
        fprintf(stderr, "WARN: NULL node passed to BinaryReader_BTreeNode.\n");
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

BHeader_t* BinaryReader_BTreeHeader(FILE* pt) {
    fseek(pt, 0, SEEK_SET);

    BHeader_t* header = BHeader_Create(0, -1, -1);

    fread(&(header->status), sizeof(char), 1, pt);
    fread(&(header->rootRRN), sizeof(RRN_t), 1, pt);
    fread(&(header->rrnNextNode), sizeof(RRN_t), 1, pt);
    fread(&(header->unused[0]), sizeof(char), 68, pt);

    return header;
}
