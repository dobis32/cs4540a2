/*
	AUTHOR: SCOTT VANDERWEIDE
	CS 4540
	ASSIGNMENT 1
*/
#ifndef IO_H
#define IO_H
#include "structs.h"
extern void checkIo(process*** ioCollection, int* ioCollectionSize, int* ioToMove);
extern void shiftIoCollection(process*** ioCollection, int* ioCollectionSize, int ioToMove);
extern void tickIo(process*** ioCollection, int ioCollectionSize);
extern void addToIOCollection(process*** ioCollection, process** incomingProcess, int* ioCollectionSize);
#endif