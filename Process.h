/**
* @file Process.h
*
* @brief Header file for Processes
*
* @details Contains methods for creating new processes
* and managing processes
*
* @commit 501b0d6
* C.S. Student (17 February 2017)
* Basic creation of this file
*
* @note requires Structures.h, SimpleTimer.h, SimpleTimer.c
*/
#ifndef PROCESS_H
#define PROCESS_H

#include "Structures.h"
#include "SimpleTimer.h"

typedef struct
{
    enum state {New, Ready, Running, Exit} state;
    MetaDataNode *currentNode;
    int procNum;
} PCB;



typedef struct ProcessListNode
{
    PCB *process;
    struct ProcessListNode *nextProcess;
} ProcessListNode;

void CreateProcesses(ProcessListNode*, MetaDataNode*);
int Run(PCB*, ConfigInfo*, char*);
void SetReady(PCB*);
void SetRunning(PCB*);
void SetExit(PCB*);

#endif //STRUCTURES_H
