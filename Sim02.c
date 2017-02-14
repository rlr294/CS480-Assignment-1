/**
* @file Sim02.c
*
* @brief Driver program for running the simulator
*
* @details Uses parser and structures methods to read and store
* configuration data and meta data
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
* @note Requires structures.h, structures.c, parser.h, parser.c
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Sim02.h"
#include "Structures.h"
#include "Parser.h"

Boolean errorCheck(int errorNum);

// Main Function Implementation ///////////////////////////////////
int main(int argc, char const *argv[])
{
    char configFileName[40];
    ConfigInfo configData = {-1, "\0", -1, -1, -1, -1, -1, -1, "\0"};
    int errorNum = 0;
    MetaDataNode head = {};

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
    if(errorCheck(errorNum))
    {
        exit(1);
    }

    printf("Loading meta-data file\n");
    errorNum = ReadMetaData(configData.filePath , &head);
    if(errorCheck(errorNum))
    {
        exit(1);
    }

    printf("==========================\n\nBegin Simulation\n");

    //Print System start
    //Start Timer

    //Print PCB creation

    //Make PCBs in new state
    //Print initialized

    //Set PCBs to ready state
    //Print ready

    //Set PCB running
    //Print running

    //Start run while loop, exit condition on PCB.State == Exit
        //while loop calls run on PCB0
        //if run returns PCB complete
            //set PCB.state = Exit
            //Print Exit state

    //Print System Stop

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
Boolean errorCheck(int errorNum)
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
        printf("Invalid meta data format");
    }
    else if(errorNum == CONFIG_FORMAT_ERROR)
    {
        printf("Missing required configuration information");
    }
    return TRUE;
}
