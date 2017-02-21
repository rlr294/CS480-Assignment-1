/**
* @file structures.h
*
* @brief Header file for OS simulation structures
*
* @details contains the data structures for containing the
* config data and meta data
*
* @commit 795a4a0
* C.S. Student (27 January 2017)
* Changed some of the method arguements to agree with Structures.c
*
* @commit 501b0d6
* C.S. Student (26 January 2017)
* Structs developed and functions declared
*
* @note None
*/
#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct ConfigInfo
{
    int versionPhase;
    char filePath[40];
    enum cpuSchedulingCode {NONE, FCFSN, SJFN, SRTFP, FCFSP, RRP}
        cpuSchedulingCode;
    int quantumTime;
    int memoryAvailable;
    int pCycleTime;
    int ioCycleTime;
    enum logTo {Monitor, File, Both} logTo;
    char logFilePath[40];
} ConfigInfo;

typedef struct MetaDataNode
{
    char command;
    char operation[10];
    int cycleTime;
    struct MetaDataNode *nextNode;
} MetaDataNode;

void AddToList(MetaDataNode *head, char newCommand, char newOpperation[10],
        int newCycleTime);

void PrintList(MetaDataNode *head);

void PrintConfig(ConfigInfo *configData);

char* NodeToString(MetaDataNode *node);

#endif //STRUCTURES_H
