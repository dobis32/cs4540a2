/*
	AUTHOR: SCOTT VANDERWEIDE
	CS 4540
	ASSIGNMENT 1
*/
#include <stdio.h>
#include <stdlib.h>
#include "io.h"
#include "structs.h"

void loadIo(){}

void checkIo(process*** ioCollection, int* ioCollectionSize, int* ioToMove){
    *ioToMove = -1;
    int* i = malloc(sizeof(int));
    for(*i = 0; *i < *ioCollectionSize - 1; *i = *i + 1) {
        // printf("io status: [%d] %d / %d\n", *i, ioCollection[0][*i]->curIo, ioCollection[0][*i]->io);
        if (ioCollection[0][*i]->curIo >= ioCollection[0][*i]->io) {
            *ioToMove = *i;
        }
    }
    free(i);
}

void tickIo(process*** ioCollection, int ioCollectionSize) {
    int* i = malloc(sizeof(int));
    for(*i = 0; *i < ioCollectionSize - 1; *i = *i + 1) {
        ioCollection[0][*i]->curIo = ioCollection[0][*i]->curIo + 1;
        ioCollection[0][*i]->ioTotal = ioCollection[0][*i]->ioTotal + 1;
    }
    free(i);
}

void shiftIoCollection(process*** ioCollection, int* ioCollectionSize, int ioToMove) {
    int* i = malloc(sizeof(int));
    for(*i = ioToMove; *i < *ioCollectionSize - 1; *i = *i + 1) {
        ioCollection[0][*i] = ioCollection[0][*i + 1];
    }
    ioCollection[0][*ioCollectionSize - 1] = NULL;
    *ioCollectionSize = *ioCollectionSize - 1;
    free(i);
}

void addToIOCollection(process*** ioCollection, process** incomingProcess, int* ioCollectionSize){
    ioCollection[0][*ioCollectionSize] = incomingProcess[0];
    *ioCollectionSize = *ioCollectionSize + 1;
}