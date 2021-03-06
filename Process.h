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
#include "Sim04.h"
#include <pthread.h>

#define PROC_READY 0
#define PROC_BLOCK 1
#define PROC_EXIT 2

typedef struct
{
    enum state {New, Ready, Running, Blocked, Exit} state;
    MetaDataNode *currentNode;
    int procNum;
    int cycleTime;
} PCB;

typedef struct ProcessListNode
{
    PCB *process;
    struct ProcessListNode *nextProcess;
} ProcessListNode;

typedef struct QueueNode {
	PCB *process;
	struct QueueNode* next;
} QueueNode;

typedef struct IOdata
{
    int delay;
    char* timer;
    PCB *process;
    QueueNode *processQueue;
    ConfigInfo *configData;
} IOdata;

void shortestJobFirstSort(ProcessListNode *start);
void CreateProcesses(ProcessListNode*, MetaDataNode*, ConfigInfo*);
int Run(PCB*, ConfigInfo*, char*, char*);
int PreemptiveRun(PCB*, ConfigInfo*, char*, char*, QueueNode*, QueueNode*);
void SetReady(PCB*);
void SetRunning(PCB*);
void SetBlocked(PCB*);
void SetExit(PCB*);

QueueNode* EnqueueFCFS(QueueNode *head, PCB *process);

QueueNode* Dequeue(QueueNode *head);

#endif //STRUCTURES_H
