/**
* @file structures.c
*
* @brief Implementation file for structures class
*
* @details Implements all member methods of the structures class
*
* @version 1.00
* C.S. Student (26 January 2017)
* Initial development and testing of structures class
*
* @note Requires structures.h
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Structures.h"

static char* convertSchedulingCode(enum cpuSchedulingCode code)
{
    static char * strings[6] = {"FCFS-N", "FCFS-N", "SJF-N", "SRTF-P",
        "FCFS-P", "RR-P"};

    return strings[code];
}

static char* convertLogTo(enum logTo log)
{
    static char * strings[3] = {"Monitor", "File", "Both"};

    return strings[log];
}

static MetaDataNode *makeNode(MetaDataNode *newNode, char newCommand, char newOpperation[10], int newCycleTime)
{
    newNode->command = newCommand;
    strcpy(newNode->opperation, newOpperation);
    newNode->cycleTime = newCycleTime;
    newNode->nextNode = NULL;

    return newNode;
}

void AddToList(MetaDataNode *head, char newCommand, char newOpperation[10], int newCycleTime)
{
    MetaDataNode *currentNode = head;

    MetaDataNode *newNode = malloc(sizeof(MetaDataNode));
    newNode = makeNode(newNode, newCommand, newOpperation, newCycleTime);

    while(currentNode->nextNode != NULL)
    {
        currentNode = currentNode->nextNode;
    }
    currentNode->nextNode = newNode;
}

void PrintList(MetaDataNode *head)
{
    MetaDataNode *currentNode = head;
    printf("========META DATA========\n\n");
    while(currentNode != NULL)
    {
        printf("Command: %c\n", currentNode->command);
        printf("Opperation: %s\n", currentNode->opperation);
        printf("Cycle Time: %i\n\n", currentNode->cycleTime);
        currentNode = currentNode->nextNode;
    }
}

void PrintConfig(ConfigInfo *configData)
{
    printf("Start Simulator Configuration File\n");
    printf("Version/Phase: %d\n", configData->versionPhase);
    printf("File Path: %s\n", configData->filePath);
    printf("CPU Scheduling Code: %s\n",
        convertSchedulingCode(configData->cpuSchedulingCode));
    printf("Quantum Time (cycles): %d\n", configData->quantumTime);
    printf("Memory Available (KB): %d\n", configData->memoryAvailable);
    printf("Processor Cycle Time (msec): %d\n", configData->processorCycleTime);
    printf("I/O Cycle Time (msec): %d\n", configData->ioCycleTime);
    printf("Log to: %s\n", convertLogTo(configData->logTo));
    printf("Log File Path: %s\n", configData->logFilePath);
    printf("End Simulator Configuration File.\n\n");
}
