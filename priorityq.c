#include <stdio.h>
#include <stdlib.h>
#include "structs.h"
#include "priorityq.h"

void sortProcessByPriority(process*** priorityQ, int numProcesses){
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

void populatePriorityQ(process*** priorityQ, process** allProcesses, int numProcesses, int* pqSize){
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