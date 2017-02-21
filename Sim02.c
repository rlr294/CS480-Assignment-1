/**
* @file Sim02.c
*
* @brief Driver program for running the simulator
*
* @details Uses parser and structures methods to read and store
* configuration data and meta data
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
#include "Sim02.h"
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
    int numberOfProcesses = 0;
    PCB proc0 = {};//malloc(sizeof(PCB));

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

    printf("Loading configuration file\n");
    errorNum = ReadConfig(configFileName, &configData);
    if(ErrorCheck(errorNum))
    {
        exit(1);
    }

    printf("Loading meta-data file\n");
    errorNum = ReadMetaData(configData.filePath , &head);
    if(ErrorCheck(errorNum))
    {
        exit(1);
    }

    printf("==========================\n\nBegin Simulation\n");

    strcpy(timer, "0.000000");
    accessTimer(START_TIMER, timer);
    printf("Time: %9s, System Start\n", timer);

    accessTimer(GET_TIME_DIFF, timer);
    printf("Time: %9s, OS: Begin PCB Creation\n", timer);

    NewProcess(&proc0, &head, numberOfProcesses);
    accessTimer(GET_TIME_DIFF, timer);
    printf("Time: %9s, OS: All processes initialized in New state\n", timer);

    SetReady(&proc0);
    accessTimer(GET_TIME_DIFF, timer);
    printf("Time: %9s, OS: All processes initialized in New state\n", timer);

    SetRunning(&proc0);
    accessTimer(GET_TIME_DIFF, timer);
    printf("Time: %9s, OS: Process %d set in Running state\n",
        timer, proc0.procNum);

    while(proc0.currentNode != NULL)
    {
        Run(&proc0, &configData, timer);
    }

    SetExit(&proc0);
    accessTimer(GET_TIME_DIFF, timer);
    printf("Time: %9s, OS: Process %d set in Exit state\n",
        timer, proc0.procNum);

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
        printf("Invalid value for 'CPU Scheduling Code' in the configuration file\n");
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
    return TRUE;
}
