#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "structs.h"

void dequeue(process*** queue, int* queueSize) {
    static int i = 0;
    for(i = 0; i < *queueSize - 1; i = i + 1){
        queue[0][i] = queue[0][i + 1];
    }
    queue[0][*queueSize - 1] = NULL;
    *queueSize = *queueSize - 1;
    // printf("current priority Q:\n");
    for(i = 0; i < *queueSize; i++) {
        // printf("cpu: %d io: %d\n", queue[0][i]->cpu, queue[0][i]->io);
    }
    // printf("queue size: %d\n", *queueSize);
}

void enqueue(process** incomingProcess, process*** queue, int* queueSize) {
    if(!queue[0][*queueSize]) {
        // printf("ding %d\n", *queueSize);
        queue[0][*queueSize] = incomingProcess[0];
        queue[0][*queueSize]->waitCount = queue[0][*queueSize]->waitCount + 1;
        queue[0][*queueSize]->curPrior = queue[0][*queueSize]->priority + 1;
        *queueSize = *queueSize + 1;
    } else {
        printf("[you shouldn't see this] something here... %d %d\n", queue[0][*queueSize]->cpu, queue[0][*queueSize]->io);
    }
}

// void reallocQueue(process*** priorityQ, int* pqSize) {
//     int size = 1;
//     process** newPriorityQ = malloc(1);
//     while(priorityQ[0][size] && size < *pqSize){
//         // printf("hello %d\n", size);
//         //printf("priority: %u cpu: %u io: %u\n", priorityQ[0][size]->priority, priorityQ[0][size]->cpu, priorityQ[0][size]->io);
//         newPriorityQ = realloc(newPriorityQ, sizeof(process*) * size);
//         newPriorityQ[size-1] = priorityQ[0][size];
//         size++;
//     }
//     *pqSize = size - 1;
// }