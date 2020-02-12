#ifndef CPU_H
#define CPU_H
#include "structs.h"
extern void moveToWait(process* processToMove);
extern void moveToIo(process* processToMove);
extern void processIsFinished(process* processToMove);
extern void loadCpu(process** cpuCurrentProcess, process*** priorityQ, int* priorityQSize);
extern void checkCpu(process** cpuCurrentProcess, int quantum, int ticks, int* retValue, int* processesCompleted);
#endif