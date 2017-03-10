/**
* @file Sim03.c
*
* @brief Driver program for running the simulator
*
* @details Uses parser and structures methods to read and store
* configuration data and meta data
*
* @commit
* C.S. Student (23 February 2017)
* Added support for multiple processes
*
* @commit
* C.S. Student (21 February 2017)
* Finished coding for Project 2
*
* @commit 12b42c2
* C.S. Student (3 February 2017)
* Finished adding all error checking for the parser
*
* @commit fd579a2
* C.S. Student (31 January 2017)
* Added documentation for all methods, implemented errorCheck
*
* @commit 030ed4b
* C.S. Student (30 January 2017)
* Added the reading of Meta Data
*
* @commit 795a4a0
* C.S. Student (27 January 2017)
* Added checking for command line arguements,
* parses in Config file and prints out the stored information
*
* @commit 501b0d6
* C.S. Student (26 January 2017)
* Basic creation of this file and creation of main
*
* @note Requires Structures.h, Structures.c, Parser.h, Parser.c
*       Process.h, Process.c
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Sim03.h"
#include "Structures.h"
#include "Parser.h"
#include "Process.h"
#include "SimpleTimer.h"

Boolean ErrorCheck(int errorNum);

// Main Function Implementation ///////////////////////////////////
int main(int argc, char const *argv[])
{
    char configFileName[40];
    ConfigInfo configData = {-1, "\0", -1, -1, -1, -1, -1, -1, "\0"};
    int errorNum = 0;
    MetaDataNode head = {};
    char* timer = malloc(256 * sizeof(char));
    ProcessListNode *processList = malloc(sizeof(ProcessListNode));
    ProcessListNode *processHead;

    //ensures the proper command line arguements were given
    if(argc != 2)
    {
        printf("Usage: %s configFileName\n\n", argv[0]);
        exit(1);
    }
    else
    {
        strcpy(configFileName, argv[1]);
    }

    printf("Operating System Simulator\n==========================\n\n");

    //load configuration data
    printf("Loading configuration file\n");
    errorNum = ReadConfig(configFileName, &configData);
    if(ErrorCheck(errorNum))
    {
        exit(1);
    }

    //load meta-data
    printf("Loading meta-data file\n");
    errorNum = ReadMetaData(configData.filePath , &head);
    if(ErrorCheck(errorNum))
    {
        exit(1);
    }

    printf("==========================\n\nBegin Simulation\n");

    //start the timer and the system
    strcpy(timer, "0.000000");
    accessTimer(START_TIMER, timer);
    printf("Time: %9s, System Start\n", timer);


    accessTimer(GET_TIME_DIFF, timer);
    printf("Time: %9s, OS: Begin PCB Creation\n", timer);

    //create all needed processes in the New state, stored in processList
    CreateProcesses(processList, &head, &configData);
    accessTimer(GET_TIME_DIFF, timer);
    printf("Time: %9s, OS: All processes initialized in New state\n", timer);

    //loop through the processes, setting each of them to ready
    processHead = processList;
    while(processList != NULL)
    {
        SetReady(processList->process);
        processList = processList->nextProcess;
    }
    //point the list back to the start instead of the end
    processList = processHead;
    accessTimer(GET_TIME_DIFF, timer);
    printf("Time: %9s, OS: All processes now set in Ready state\n", timer);

    //order the processes by cycle time if using SJFN scheduling
    if(configData.cpuSchedulingCode == SJFN)
    {
        shortestJobFirstSort(processList);
    }

    //for each process:
    while(processList != NULL)
    {
        SetRunning(processList->process); //set it to running
        accessTimer(GET_TIME_DIFF, timer);
        printf("Time: %9s, OS: Process %d set in Running state\n",
            timer, processList->process->procNum);

        //execute all commands
        while(processList->process->currentNode != NULL)
        {
            Run(processList->process, &configData, timer);
        }

        SetExit(processList->process); //set it to exit
        accessTimer(GET_TIME_DIFF, timer);
        printf("Time: %9s, OS: Process %d set in Exit state\n",
            timer, processList->process->procNum);

        processList = processList->nextProcess;
    }

    accessTimer(GET_TIME_DIFF, timer);
    printf("Time: %9s, System stop\n", timer);

     return 0;
 }

// Free Function Implementation ///////////////////////////////////

/*
* @brief Checks if there have been errors
*
* @details Upon encountering an error prints out an error code
*
* @param[in] errorNum
*            holds the error code
*
* @return TRUE if there are errors, FALSE if there are not
*
* @note: None
*/
Boolean ErrorCheck(int errorNum)
{
    if(errorNum == 0)
    {
        return FALSE;
    }
    else if(errorNum == CPU_SCHEDULING_CODE_ERROR)
    {
        printf("Invalid value for 'CPU Scheduling Code' "
            "in the configuration file\n");
    }
    else if(errorNum == LOG_TO_ERROR)
    {
        printf("Invalid value for 'Log To' in the configuration file\n");
    }
    else if(errorNum == CONFIG_FILE_ERROR)
    {
        printf("Error opening configuration file\n");
    }
    else if(errorNum == META_DATA_FILE_ERROR)
    {
        printf("Error opening meta data file\n");
    }
    else if(errorNum == META_DATA_FORMAT_ERROR)
    {
        printf("Invalid meta data format\n");
    }
    else if(errorNum == CONFIG_FORMAT_ERROR)
    {
        printf("Missing required configuration information\n");
    }
    else if(errorNum == CONFIG_BOUNDS_ERROR)
    {
        printf("At least one configuration value is out of bounds\n");
    }
    else if(errorNum == WRONG_VERSION)
    {
        printf("Wrong Version Number\n");
    }
    return TRUE;
}