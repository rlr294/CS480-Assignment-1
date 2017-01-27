/**
* @file structures.h
*
* @brief Header file for OS simulation structures
*
* @details contains the data structures for containing the
* config data and meta data
*
* @version 1.00
* C.S. Student (26 January 2017)
* Initial development of the structures
*
* @note None
*/
#ifndef STRUCTURES_H
#define STRUCTURES_H

typedef struct ConfigInfo
{
    int versionPhase;
    char filePath[40];
    enum cpuSchedulingCode {NONE, FCFSN, SJFN, SRTFP, FCFSP, RRP} schedule;
    int quantumTime;
    int memoryAvailable;
    int processorCycleTime;
    int ioCycleTime;
    enum logTo {Monitor, File, Both} log;
    char logFilePath[40];
} ConfigInfo;

typedef struct MetaDataNode
{
    char command;
    char opperation[10];
    int cycleTime;
    struct MetaDataNode *nextNode;
} MetaDataNode;

void AddToList(MetaDataNode *head, MetaDataNode *newNode);

void printList(MetaDataNode *head);

void printConfig(ConfigInfo *configuration);

#endif
