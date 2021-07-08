#ifndef _PRINTER_H_
#define _PRINTER_H_


#include "bTree.h"
#include "bTreeDataModel.h"

// FUNÇÕES RESPONSÁVEIS PELA FORMATAÇÃO DOS PRINTS

//  Printam um registro seguindo as normas
void Printer_Vehicle(Vehicle_t *vehicle);
void Printer_BusLine(BusLine_t *busLine);
void Printer_Node(BTreeMetadata_t* cache, BNode_t* node);

#endif
