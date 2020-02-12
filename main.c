#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"
#include "readProcesses.c"
#include "priorityq.c"
#include "processActions.c"
#include "queue.c"
#include "io.c"


// void sortPriorityQueue(Arrays* arrs, int n) 
// { 
//     int* i = malloc(sizeof(int));
//     int* j = malloc(sizeof(int));
//     int* key = malloc(sizeof(int));
//     arrs->states[arrs->statesLength] = n;
//     arrs->statesLength++;
//     // base insertion sort algorithm source: https://www.geeksforgeeks.org/insertion-sort/
//     for (*i = 1; *i < arrs->statesLength; *i = *i + 1) { 
//         *key = arrs->states[*i];
//         *j = *i - 1; 
//         while (*j >= 0 && arrs->lines[arrs->states[*j]].stateCode > arrs->lines[*key].stateCode) { 
//             arrs->states[*j + 1] = arrs->states[*j]; 
//             *j = *j - 1; 
//         } 
//         arrs->states[*j + 1] = *key; 
//     } 
//     free(i);
//     free(j);
//     free(key);
// } 

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
    // int retValue = 0;
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
		readProcesses(infile, &allProcesses, &processesRead);
        fclose(infile);
        printf("read %d processes from file\n", processesRead);         
        populatePriorityQ(&priorityQ, &allProcesses, processesRead, &priorityqSize);
        ioCollection = malloc(sizeof(process*) * processesRead);
        printf("queue populated size: %d\n", priorityqSize);
        sortProcessByPriority(&priorityQ, priorityqSize);
        loadCpu(&cpuCurrentProcess, &priorityQ, &priorityqSize);
        dequeue(&priorityQ, &priorityqSize);
        printf("cpu loaded... new pq size: %d\n", priorityqSize);
        // printf("priority Q reallocated, new size: %d\n", priorityqSize);
        int i;
        for(i = 0; i < priorityqSize; i++) {
            priorityQ[i]->waitCount = priorityQ[i]->waitCount + 1;
        }
        for(i = 1; i < 42000; i++) { 
            tickWait(&priorityQ, priorityqSize, osParams.wait);
            sortProcessByPriority(&priorityQ, priorityqSize);

        // while(processesCompleted < processesRead) {
        //     i++;
        // for(i = 0; i < priorityqSize; i++) {
            // printf("%d %d %d %d\n", i, priorityQ[i]->priority, priorityQ[i]->cpu, priorityQ[i]->io);
            // printf("priority: %u cpu: %u io: %u runtime: %u isRunning: %u\n", priorityQ[i]->priority, priorityQ[i]->cpu, priorityQ[i]->io, priorityQ[i]->runtime, priorityQ[i]->isRunning);
            checkCpu(&cpuCurrentProcess, osParams.quantum, i, &cpuStatus, &processesCompleted);
            if(cpuCurrentProcess) printf("[tick %d] checked cpu... %d / %d status: %d\n", i, cpuCurrentProcess[0].curCpu, cpuCurrentProcess[0].cpu, cpuStatus);
            if(cpuStatus == 1){
                // cpu empty
                if(priorityqSize > 0) {
                    loadCpu(&cpuCurrentProcess, &priorityQ, &priorityqSize);
                    dequeue(&priorityQ, &priorityqSize);
                } else {
                    printf("priority Q empty.. aborting loop\n");
                    i = 2600;
                }
            } else if(cpuStatus == 2){
                // move to IO
                addToIOCollection(&ioCollection, &cpuCurrentProcess, &ioCollectionSize);
                for(j = 0; j < ioCollectionSize; j++) {
                    printf("[IN IO] cpu: %d: io %d\n", ioCollection[j]->cpu, ioCollection[j]->io);
                }
                printf("IO collection size: %d\n", ioCollectionSize);
                cpuCurrentProcess = NULL;
                if(priorityqSize > 0) {
                    loadCpu(&cpuCurrentProcess, &priorityQ, &priorityqSize);
                    dequeue(&priorityQ, &priorityqSize);
                }
            } else if (cpuStatus == 3){
                // move to priorityQ
                enqueue(&cpuCurrentProcess, &priorityQ, &priorityqSize);
                sortProcessByPriority(&priorityQ, priorityqSize);
                cpuCurrentProcess = NULL;
                if(priorityqSize > 0) {
                    loadCpu(&cpuCurrentProcess, &priorityQ, &priorityqSize);
                    dequeue(&priorityQ, &priorityqSize);
                }
            }
            // check io collection
            // checkIo();
            // move each when they complete
            tickIo(&ioCollection, ioCollectionSize);
            // ioToMove = -1;
            checkIo(&ioCollection, &ioCollectionSize, &ioToMove);
            // if(ioToMove >= 0) printf("io finished: [%d] %d / %d\n", ioToMove, ioCollection[ioToMove]->curIo, ioCollection[ioToMove]->io);
            while(ioToMove >= 0) {
                printf("io finished: [%d] %d / %d\n", ioToMove, ioCollection[ioToMove]->curIo, ioCollection[ioToMove]->io);
                ioCollection[ioToMove]->ioTotal = ioCollection[ioToMove]->curIo;
                ioCollection[ioToMove]->curIo = 0;
                printf("enqueueing from io...\n");

                enqueue(&ioCollection[ioToMove], &priorityQ, &priorityqSize);
                // for(j = 0; j < priorityqSize; j++) {
                //     printf("[%d] %d / %d\n", j, priorityQ[j]->cpu, priorityQ[j]->io);
                // }
                ioCollection[ioToMove] = NULL;
                shiftIoCollection(&ioCollection, &ioCollectionSize, ioToMove);
                for(j = 0; j < ioCollectionSize; j++) {
                    printf("[%d] %d / %d\n", j, ioCollection[j]->curIo, ioCollection[j]->io);
                }
                printf("io collection size: %d\n", ioCollectionSize);
                checkIo(&ioCollection, &ioCollectionSize, &ioToMove);
                // ioToMove = -1;
            }
            // adjust priority
        }
        for(i = 0; i < processesRead; i++) {
            printf("[%d] %d / %d\n", i, allProcesses[i].cpuTotal + allProcesses[i].ioTotal, allProcesses[i].runtime);
        }
        printf("processes finished: %d\n", processesCompleted);
    } else {
        retval = 1;
		printf("failed to open file!\n closing program...\n");
    }
    return retval;
}