/*
	AUTHOR: SCOTT VANDERWEIDE
	CS 4540
	ASSIGNMENT 1
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"
#include "readProcesses.c"
#include "priorityq.c"
#include "cpu.c"
#include "queue.c"
#include "io.c"

int main(int argc, char * argv[]) {
    process** priorityQ = malloc(sizeof(process*));
    process** ioCollection = malloc(sizeof(process*));
    int processesRead = 0;
    int processesCompleted = 0;
    int retval = 0;
    int priorityqSize = 0;
    int cpuStatus = 0;
    int ioToMove = -1;
    int ioCollectionSize = 0;
    int j = 0;
    process* cpuCurrentProcess = NULL;

    os osParams;
    osParams.quantum = 70;
    osParams.wait = 20;
    printf("\nOS PARAMS\nquantum: %d\nwait: %d\n\n", osParams.quantum, osParams.wait);
    process* allProcesses ;
    allocProcess(&allProcesses, &processesRead);

	FILE * infile = NULL;
    infile = fopen("a2in.txt", "r");

	if(infile) {
        init();
        int i;
        for(i = 0; i < priorityqSize; i++) {
            priorityQ[i]->waitCount = priorityQ[i]->waitCount + 1;
        }
        for(i = 1; i < 140000; i++) { 
            tickWait(&priorityQ, priorityqSize, osParams.wait);
            sortProcessByPriority(&priorityQ, priorityqSize);
            checkCpu(&cpuCurrentProcess, osParams.quantum, i, &cpuStatus, &processesCompleted);
            if(cpuStatus == 1 || cpuStatus == 4){ // cpu empty
                if(priorityqSize > 0) {
                    loadCpu(&cpuCurrentProcess, &priorityQ, &priorityqSize);
                    dequeue(&priorityQ, &priorityqSize);
                } else {
                    printf("priority Q empty.. aborting loop\n");
                    i = 140000;
                }
            } else if(cpuStatus == 2){ // move to IO
                addToIOCollection(&ioCollection, &cpuCurrentProcess, &ioCollectionSize);
                cpuCurrentProcess = NULL;
                if(priorityqSize > 0) {
                    loadCpu(&cpuCurrentProcess, &priorityQ, &priorityqSize);
                    dequeue(&priorityQ, &priorityqSize);
                }
            } else if (cpuStatus == 3){ // move to priorityQ
                enqueue(&cpuCurrentProcess, &priorityQ, &priorityqSize);
                sortProcessByPriority(&priorityQ, priorityqSize);
                cpuCurrentProcess = NULL;
                if(priorityqSize > 0) {
                    loadCpu(&cpuCurrentProcess, &priorityQ, &priorityqSize);
                    dequeue(&priorityQ, &priorityqSize);
                }
            }
            tickIo(&ioCollection, ioCollectionSize);
            checkIo(&ioCollection, &ioCollectionSize, &ioToMove);
            while(ioToMove >= 0) {
                ioCollection[ioToMove]->ioTotal = ioCollection[ioToMove]->curIo;
                ioCollection[ioToMove]->curIo = 0;
                enqueue(&ioCollection[ioToMove], &priorityQ, &priorityqSize);
                sortProcessByPriority(&priorityQ, priorityqSize);
                ioCollection[ioToMove] = NULL;
                shiftIoCollection(&ioCollection, &ioCollectionSize, ioToMove);
                checkIo(&ioCollection, &ioCollectionSize, &ioToMove);
            }
        }
        printf("processes finished: %d\n", processesCompleted);
    } else {
        retval = 1;
		printf("failed to open file!\n closing program...\n");
    }
    return retval;
}

void init() {
    readProcesses(infile, &allProcesses, &processesRead);
    fclose(infile);
    printf("read %d processes from file\n", processesRead);         
    populatePriorityQ(&priorityQ, &allProcesses, processesRead, &priorityqSize);
    ioCollection = malloc(sizeof(process*) * processesRead);
    printf("queue populated size: %d\n", priorityqSize);
    sortProcessByPriority(&priorityQ, priorityqSize);
    loadCpu(&cpuCurrentProcess, &priorityQ, &priorityqSize);
    cpuCurrentProcess[0].waitCount = 0;
    dequeue(&priorityQ, &priorityqSize);
    printf("cpu loaded... new pq size: %d\n", priorityqSize);
}