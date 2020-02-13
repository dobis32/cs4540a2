/*
	AUTHOR: SCOTT VANDERWEIDE
	CS 4540
	ASSIGNMENT 1
*/
#include <stdio.h>
#include <stdlib.h>
#include "processActions.h"
#include "structs.h"

void sortProcessByPriority(process*** priorityQ, int numProcesses) {
    int* i = malloc(sizeof(int));
    int* j = malloc(sizeof(int));
    process* key;
    for (*i = 1; *i < numProcesses; *i = *i + 1) { 
        key = priorityQ[0][*i]; 
        *j = *i - 1; 
        while (*j >= 0 && priorityQ[0][*j]->curPrior < key->curPrior) { 
            priorityQ[0][*j + 1] = priorityQ[0][*j]; 
            *j = *j - 1; 
        } 
        priorityQ[0][*j + 1] = key; 
    } 
    free(i);
    free(j);
}

void populatePriorityQ(process*** priorityQ, process** allProcesses, int numProcesses, int* pqSize) {
    int* i = malloc(sizeof(int));
    *pqSize = 0;
    for(*i = 0; *i < numProcesses; *i = *i + 1) {
        if(allProcesses[0][*i].isRunning > 0){
            *pqSize = *pqSize + 1;
            priorityQ[0] = realloc(priorityQ[0], sizeof(process*) * *pqSize);          
            priorityQ[0][*pqSize-1] = &allProcesses[0][*i];
        }
    }
    free(i);
}

void loadCpu(process** cpuCurrentProcess, process*** priorityQ, int* priorityQSize) {
    cpuCurrentProcess[0] = priorityQ[0][0];
    cpuCurrentProcess[0]->curCpu = 0;
    if (cpuCurrentProcess[0]->wait < cpuCurrentProcess[0]->waitMin) cpuCurrentProcess[0]->waitMin = cpuCurrentProcess[0]->wait;
    if (cpuCurrentProcess[0]->wait > cpuCurrentProcess[0]->waitMax) cpuCurrentProcess[0]->waitMax = cpuCurrentProcess[0]->wait;
    cpuCurrentProcess[0]->wait = 0;
    cpuCurrentProcess[0]->curPrior = cpuCurrentProcess[0]->priority;
    priorityQ[0][0] = NULL;
}


void checkCpu(process** cpuCurrentProcess ,int quantum, int ticks, int* retValue, int* processesCompleted) {
    *retValue = 0;
    if(!cpuCurrentProcess[0]) {
        *retValue = 1;
    } else {
        cpuCurrentProcess[0]->curCpu = cpuCurrentProcess[0]->curCpu + 1;
        cpuCurrentProcess[0]->cpuTotal = cpuCurrentProcess[0]->cpuTotal + 1;
        if(cpuCurrentProcess[0]->cpuTotal + cpuCurrentProcess[0]->ioTotal >= cpuCurrentProcess[0]->runtime){
            cpuCurrentProcess[0]->isRunning = 0;
            cpuCurrentProcess[0] = NULL;
            *retValue = 4;
            *processesCompleted = *processesCompleted + 1;
        } else if(cpuCurrentProcess[0]->curCpu >= cpuCurrentProcess[0]->cpu){
            *retValue = 2;
        } else if (cpuCurrentProcess[0]->curCpu % quantum == 0) {
            *retValue = 3;
        }
    }        
}

void tickWait(process*** priorityQ, int priorityqSize, int waitTime) {
    int* i = malloc(sizeof(int));
    for(*i = 0; *i < priorityqSize - 1; *i = *i + 1) {
        priorityQ[0][*i]->wait = priorityQ[0][*i]->wait + 1;
        priorityQ[0]->waitSum = cpuCurrentProcess[0]->waitSum + 1;
        if(priorityQ[0][*i]->wait % waitTime == 0 && priorityQ[0][*i]->wait > 0) priorityQ[0][*i]->curPrior = priorityQ[0][*i]->curPrior + 1;
        if(priorityQ[0][*i]->curPrior > 15) priorityQ[0][*i]->curPrior = 15;
    }
    free(i);
}

void dequeue(process*** queue, int* queueSize) {
    static int i = 0;
    for(i = 0; i < *queueSize - 1; i = i + 1){
        queue[0][i] = queue[0][i + 1];
    }
    queue[0][*queueSize - 1] = NULL;
    *queueSize = *queueSize - 1;
}

void enqueue(process** incomingProcess, process*** queue, int* queueSize) {
    if(!queue[0][*queueSize]) {
        queue[0][*queueSize] = incomingProcess[0];
        queue[0][*queueSize]->waitCount = queue[0][*queueSize]->waitCount + 1;
        queue[0][*queueSize]->curPrior = queue[0][*queueSize]->priority + 1;
        *queueSize = *queueSize + 1;
    } else {
        printf("[you shouldn't see this] something here... %d %d\n", queue[0][*queueSize]->cpu, queue[0][*queueSize]->io);
    }
}