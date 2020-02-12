#ifndef READFILE_H
#define READFILE_H
extern void sortProcessByPriority(process*** priorityQ, int numProcesses);
extern void populatePriorityQ(process*** priorityQ, process** allProcesses, int numProcesses, int* pqSize);
#endif