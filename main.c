/*
	AUTHOR: SCOTT VANDERWEIDE
	CS 4540
	ASSIGNMENT 1

    TODO 
    processes are not finishing properly for some reason
    clean up commented-out code

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"
#include "readProcesses.h"
#include "cpu.h"
#include "io.h"

void init(FILE** infile, process** allProcesses, int* processesRead, process*** priorityQ, int* priorityqSize, process*** ioCollection, process** cpuCurrentProcess) {
    readProcesses(*infile, allProcesses, processesRead);
    fclose(*infile);
    printf("read %d processes from file\n", *processesRead);         
    populatePriorityQ(priorityQ, allProcesses, *processesRead, priorityqSize);
    *ioCollection = malloc(sizeof(process*) * *processesRead);
    sortProcessByPriority(priorityQ, *priorityqSize);
    printf("queue populated & sorted -- size: %d\n", *priorityqSize);
    loadCpu(cpuCurrentProcess, priorityQ, priorityqSize);
    cpuCurrentProcess[0][0].waitCount = 0;
    dequeue(priorityQ, priorityqSize);
    printf("cpu loaded... new pq size: %d\n", *priorityqSize);
}

int main(int argc, char * argv[]) {
    process* cpuCurrentProcess = NULL;
    // process** ioCollection = malloc(sizeof(process*));
    process** ioCollection;
    process** priorityQ = malloc(sizeof(process*));

    int cpuStatus = 0;
    int ioToMove = -1;
    int ioCollectionSize = 0;
    int priorityqSize = 0;
    int processesRead = 0;
    int processesCompleted = 0;
    int retval = 0;
 
    os osParams;
    osParams.quantum = 70;
    osParams.wait = 20;
    // printf("\nOS PARAMS\nquantum: %d\nwait: %d\n\n", osParams.quantum, osParams.wait);
    process* allProcesses ;
    allocProcess(&allProcesses, &processesRead);

	FILE * infile = NULL;
    // infile = fopen("a2in.txt", "r");
    infile = fopen("in.txt", "r");

	if(infile) {
        init(&infile, &allProcesses, &processesRead, &priorityQ, &priorityqSize, &ioCollection, &cpuCurrentProcess);
        int i = 0;
        // for(i = 0; i < priorityqSize; i++) {
        //     priorityQ[i]->waitCount = priorityQ[i]->waitCount + 1;
        // }
        // for(i = 1; i < 160000; i++) { 
        while(processesCompleted < processesRead && i < 5000001) {
            i++;
            // printf("tick: %d\n", i);
            tickWait(&priorityQ, priorityqSize, osParams.wait);
            sortProcessByPriority(&priorityQ, priorityqSize);
            checkCpu(&cpuCurrentProcess, osParams.quantum, i, &cpuStatus, &processesCompleted);
            // if(cpuCurrentProcess->runtime == 80000) printf("problem process status: %d\n", cpuStatus);
            if(cpuStatus == 1 || cpuStatus == 4){ // cpu empty
                if(priorityqSize > 0) {
                    loadCpu(&cpuCurrentProcess, &priorityQ, &priorityqSize);
                    dequeue(&priorityQ, &priorityqSize);
                    // printf("priorityQ:\n");
                    // for (int k = 0; k < priorityqSize; k++) {
                    //     printf("%u wait: %u total: %u runtime: %u\n", priorityQ[k]->curPrior, priorityQ[k]->wait, priorityQ[k]->waitSum, priorityQ[k]->runtime);
                    // }
                }
            } else if(cpuStatus == 2){ // move to IO
                addToIOCollection(&ioCollection, &cpuCurrentProcess, &ioCollectionSize);
                cpuCurrentProcess = NULL;
                if(priorityqSize > 0) {
                    loadCpu(&cpuCurrentProcess, &priorityQ, &priorityqSize);
                    dequeue(&priorityQ, &priorityqSize);
                    // for (int k = 0; k < priorityqSize; k++) {
                    //     printf("%u wait: %u total: %u runtime: %u\n", priorityQ[k]->curPrior, priorityQ[k]->wait, priorityQ[k]->waitSum, priorityQ[k]->runtime);
                    // }
                }
            } else if (cpuStatus == 3){ // move to priorityQ
                enqueue(&cpuCurrentProcess, &priorityQ, &priorityqSize);
                // for (int k = 0; k < priorityqSize; k++) {
                //     printf("%u wait: %u total: %u runtime: %u\n", priorityQ[k]->curPrior, priorityQ[k]->wait, priorityQ[k]->waitSum, priorityQ[k]->runtime);
                // }
                sortProcessByPriority(&priorityQ, priorityqSize);
                cpuCurrentProcess = NULL;
                if(priorityqSize > 0) {
                    loadCpu(&cpuCurrentProcess, &priorityQ, &priorityqSize);
                    dequeue(&priorityQ, &priorityqSize);
                    // for (int k = 0; k < priorityqSize; k++) {
                    //     printf("%u wait: %u total: %u runtime: %u\n", priorityQ[k]->curPrior, priorityQ[k]->wait, priorityQ[k]->waitSum, priorityQ[k]->runtime);
                    // }
                }
            }
            tickIo(&ioCollection, ioCollectionSize);
            checkIo(&ioCollection, &ioCollectionSize, &ioToMove);
            while(ioToMove >= 0) {
                ioCollection[ioToMove]->ioTotal = ioCollection[ioToMove]->ioTotal + ioCollection[ioToMove]->curIo;
                ioCollection[ioToMove]->curIo = 0;
                enqueue(&ioCollection[ioToMove], &priorityQ, &priorityqSize);
                sortProcessByPriority(&priorityQ, priorityqSize);
                // for (int k = 0; k < priorityqSize; k++) {
                //     printf("%u wait: %u total: %u runtime: %u\n", priorityQ[k]->curPrior, priorityQ[k]->wait, priorityQ[k]->waitSum, priorityQ[k]->runtime);
                // }
                ioCollection[ioToMove] = NULL;
                shiftIoCollection(&ioCollection, &ioCollectionSize, ioToMove);
                checkIo(&ioCollection, &ioCollectionSize, &ioToMove);
            }
        }
        if(i == 5000001) printf("no mores ticks\n");
        printf("processes finished: %d\n", processesCompleted);
        for(int p = 0; p < processesRead; p++) {
            printf("%u / %u | cpu: %u | io: %u | is running: %u\n", allProcesses[p].cpuTotal + allProcesses[p].ioTotal, allProcesses[p].runtime, allProcesses[p].cpuTotal, allProcesses[p].ioTotal, allProcesses[p].isRunning);
        }
    } else {
        retval = 1;
		printf("failed to open file!\n closing program...\n");
    }
    return retval;
}

