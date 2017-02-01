/**
* @file structures.c
*
* @brief Driver program for running the simulator
*
* @details Uses parser and structures methods to read and store
* configuration data and meta data
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
#include "Sim01.h"
#include "Structures.h"
#include "Parser.h"

// Main Function Implementation ///////////////////////////////////
int main(int argc, char const *argv[])
{
    char configFileName[40];
    ConfigInfo configData = {};
    int errorNum = 0;
    MetaDataNode head = {};

    if(argc != 2)
    {
        printf("Usage: %s configFileName\n\n", argv[0]);
    }
    else
    {
        strcpy(configFileName, argv[1]);
    }

    errorNum = ReadConfig(configFileName, &configData);
    errorCheck(errorNum);

    PrintConfig(&configData);

    errorNum = ReadMetaData(configData.filePath , &head);
    errorCheck(errorNum);

    PrintList(&head);

    return 0;
}

// Free Function Implementation ///////////////////////////////////

/*
* @brief Checks if there have been errors
*
* @details Upon encountering an error prints out an error code and exits the
*          program
*
* @param[in] errorNum
*            holds the error code
*
* @return None
*
* @note: None
*/
void errorCheck(int errorNum)
{
    if(errorNum == 0)
    {
        return;
    }
    else if(errorNum == -1)
    {
        printf("Invalid value for 'CPU Scheduling Code' in the configuration file\n");
    }
    else if(errorNum == -2)
    {
        printf("Invalid value for 'Log To' in the configuration file\n");
    }
    else if(errorNum == -3)
    {
        printf("Error opening configuration file\n");
    }
    else if(errorNum == -4)
    {
        printf("Error opening meta data file\n");
    }
    exit(1);
}
