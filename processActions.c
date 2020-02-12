#include <stdio.h>
#include <stdlib.h>
#include "processActions.h"
#include "structs.h"


void moveToWait(process* processToMove) {
    printf("curCpu: %u\n", processToMove->curCpu);
}

void moveToIo(process* processToMove) {
    printf("curCpu: %u\n", processToMove->curCpu);
}

void processIsFinished(process* processToMove) {
    printf("curCpu: %u\n", processToMove->curCpu);
}

void loadCpu(process** cpuCurrentProcess, process*** priorityQ, int* priorityQSize){
    printf("loading cpu\n");
    // if(!cpuCurrentProcess[0]) printf("CPU is empty (as expected)\n");
    cpuCurrentProcess[0] = priorityQ[0][0];
    cpuCurrentProcess[0]->curCpu = 0;
    cpuCurrentProcess[0]->waitSum = cpuCurrentProcess[0]->wait;
    if (cpuCurrentProcess[0]->wait < cpuCurrentProcess[0]->waitMin) cpuCurrentProcess[0]->waitMin = cpuCurrentProcess[0]->wait;
    if (cpuCurrentProcess[0]->wait > cpuCurrentProcess[0]->waitMax) cpuCurrentProcess[0]->waitMax = cpuCurrentProcess[0]->wait;
    cpuCurrentProcess[0]->wait = 0;
    priorityQ[0][0] = NULL;
    // printf("%d/%d\n", cpuCurrentProcess[0]->curCpu, cpuCurrentProcess[0]->cpu);
}

void checkCpu(process** cpuCurrentProcess ,int quantum, int ticks, int* retValue, int* processesCompleted) {
    *retValue = 0;
    if(!cpuCurrentProcess[0]) {
        *retValue = 1;
        // printf("cpu empty...\n");
    } else {
        cpuCurrentProcess[0]->curCpu = cpuCurrentProcess[0]->curCpu + 1;
        cpuCurrentProcess[0]->cpuTotal = cpuCurrentProcess[0]->cpuTotal + 1;
        // printf("process status: [tick %d] %d/%d\n", ticks, cpuCurrentProcess[0]->curCpu, cpuCurrentProcess[0]->cpu);
        if(cpuCurrentProcess[0]->cpuTotal + cpuCurrentProcess[0]->ioTotal >= cpuCurrentProcess[0]->runtime){
            printf("process finished...\n");
            cpuCurrentProcess[0]->isRunning = 0;
            cpuCurrentProcess[0] = NULL;
            *retValue = 1;
            *processesCompleted = *processesCompleted + 1;
        } else if(cpuCurrentProcess[0]->curCpu >= cpuCurrentProcess[0]->cpu){
            printf("cpu burst finished!\n");
            *retValue = 2;
        } else if (cpuCurrentProcess[0]->curCpu % quantum == 0) {
            printf("quantum reached... quantum: %d tick: %d\n", quantum, ticks);
            *retValue = 3;
        }
    }    
}
