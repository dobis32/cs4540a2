/*
	AUTHOR: SCOTT VANDERWEIDE
	CS 4540
	ASSIGNMENT 1
*/
#ifndef READFILE_H
#define READFILE_H
#include "structs.h"

extern void allocProcess(process** allProcesses, int* processesRead);
extern void initProcess(process* process);
extern void readProcesses(FILE* infile, process** allProcesses, int* processesRead);
#endif
