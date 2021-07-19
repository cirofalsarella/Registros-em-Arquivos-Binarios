#ifndef _PRINTER_H_
#define _PRINTER_H_


#include "../bTree/bTree.h"
#include "../bTree/bTreeDataModel.h"
#include "../dataModel/dataModel.h"

// FUNÇÕES RESPONSÁVEIS PELA FORMATAÇÃO DOS PRINTS

//  Printam um registro seguindo as normas
void Printer_Vehicle(const Vehicle_t* vehicle);
void Printer_BusLine(const BusLine_t* busLine);
void Printer_Merge(const Vehicle_t* vehicle, const BusLine_t* busLine);
void Printer_Node(const BNode_t* node);

#endif
