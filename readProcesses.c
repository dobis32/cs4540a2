/*
	AUTHOR: SCOTT VANDERWEIDE
	CS 4540
	ASSIGNMENT 1
*/
#include <stdio.h>
#include <stdlib.h>
#include "readProcesses.h"
#include "structs.h" 

void allocProcess(process** allProcesses, int* processesRead) {
    if(*processesRead < 1){
        *allProcesses = malloc(sizeof(process) * (*processesRead + 1));
    } else {
        *allProcesses = realloc(*allProcesses, sizeof(process) * (*processesRead + 1));
    }
}

void tickWait(process*** priorityQ, int priorityqSize, int waitTime) {
    int* i = malloc(sizeof(int));
    for(*i = 0; *i < priorityqSize - 1; *i = *i + 1) {
        priorityQ[0][*i]->wait = priorityQ[0][*i]->wait + 1;
        if(priorityQ[0][*i]->wait % waitTime == 0) priorityQ[0][*i]->curPrior = priorityQ[0][*i]->curPrior + 1;
        if(priorityQ[0][*i]->curPrior > 15) priorityQ[0][*i]->curPrior = 15;
    }
    free(i);
}

void initProcess(process* process) {
    process->curCpu = 0;
    process->curIo = 0;
    process->wait = 0;
    process->curPrior = 0;
    process->cpuTotal = 0;
    process->ioTotal = 0;
    process->waitSum = 0;
    process->waitCount = 0;
    process->waitMin = 0;
    process->waitMax = 0;
    process->isRunning = 1;
}

void readProcesses(FILE* infile, process** allProcesses, int* processesRead) {
    while(fscanf(infile, "%u %u %u %u", &allProcesses[0][*processesRead].priority, &allProcesses[0][*processesRead].cpu, &allProcesses[0][*processesRead].io, &allProcesses[0][*processesRead].runtime) == 4){
        allProcesses[0][*processesRead].curPrior = allProcesses[0][*processesRead].priority;
        initProcess(&allProcesses[0][*processesRead]); 
        *processesRead = *processesRead + 1;
        allocProcess(allProcesses, processesRead);
    };
}