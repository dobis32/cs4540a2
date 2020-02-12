#ifndef IO_H
#define IO_H
#include "structs.h"
extern void loadIo();
extern void checkIo(process*** ioCollection, int* ioCollectionSize, int* ioToMove);
extern void shiftIoCollection(process*** ioCollection, int* ioCollectionSize, int ioToMove);
#endif