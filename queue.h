#ifndef QUEUE_H
#define QUEUE_H
#include "structs.h"
extern void reallocQueue(process*** priorityQ, int* pqSize);
extern void enqueue(process** incomingProcess, process*** queue, int* queueSize);
extern void dequeue(process*** queue, int* queueCount);
#endif