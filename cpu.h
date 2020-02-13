/*
	AUTHOR: SCOTT VANDERWEIDE
	CS 4540
	ASSIGNMENT 1
*/
#ifndef CPU_H
#define CPU_H
#include "structs.h"
extern void sortProcessByPriority(process*** priorityQ, int numProcesses);
extern void populatePriorityQ(process*** priorityQ, process** allProcesses, int numProcesses, int* pqSize);
extern void loadCpu(process** cpuCurrentProcess, process*** priorityQ, int* priorityQSize);
extern void checkCpu(process** cpuCurrentProcess, int quantum, int ticks, int* retValue, int* processesCompleted);
extern void tickWait(process*** priorityQ, int priorityqSize, int waitTime);
extern void dequeue(process*** queue, int* queueSize);
extern void enqueue(process** incomingProcess, process*** queue, int* queueSize);
#endif