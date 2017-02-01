/**
* @file parser.c
*
* @brief Implementation file for parser class
*
* @details Implements all member methods of the paser class
*
* @commit fd579a2
* C.S. Student (31 January 2017)
* Added documentation for all methods, added error codes for file checks
*
* @commit 44c0cab
* C.S. Student (31 January 2017)
* Added a null terminating character after parsing in opperation
*
* @commit 030ed4b
* C.S. Student (30 January 2017)
* Development of ReadMetaData
*
* @commit 795a4a0
* C.S. Student (27 January 2017)
* Initial development of convertSchedulingCode,  convertLogTo, and ReadConfig
*
* @commit 501b0d6
* C.S. Student (26 January 2017)
* Basic creation of this file
*
* @note Requires parser.h
*/
#include <string.h>
#include <stdlib.h>
#include "Parser.h"

// Free Function Implementation ///////////////////////////////////

/*
* @brief Converts the 'CPU scheduling code' string to an enum representing it
*
* @param[in] codeString
*            holds the string for the 'CPU scheduling code'
*
* @return A number used for an enum to represent the config file's
*        'CPU scheduling code', negative nubers represent error codes
*
* @note: None
*/
static int ConvertSchedulingCode(char* codeString)
{
    if(strcmp(codeString, "NONE") == 0)
    {
        return 0;
    }
    else if(strcmp(codeString, "FCFS-N") == 0)
    {
        return 1;
    }
    else if(strcmp(codeString, "SJF-N") == 0)
    {
        return 2;
    }
    else if(strcmp(codeString, "SRTF-P") == 0 )
    {
        return 3;
    }
    else if(strcmp(codeString, "FCFS-P") == 0)
    {
        return 4;
    }
    else if(strcmp(codeString, "RR-P") == 0)
    {
        return 5;
    }
    else
    {
        return -1;
    }
}

/*
* @brief Converts the 'Log To' string to an enum representing it
*
* @param[in] codeString
*            holds the string for the 'Log To'
*
* @return A number used for an enum to represent the config file's
*        'Log To', negative nubers represent error codes
*
* @note: None
*/
static int ConvertLogTo(char* logString)
{
    if(strcmp(logString, "Monitor") == 0)
    {
        return 0;
    }
    else if(strcmp(logString, "File") == 0)
    {
        return 1;
    }
    else if(strcmp(logString, "Both") == 0)
    {
        return 2;
    }
    else
    {
        return -1;
    }
}

/*
* @brief Subroutine of Read Meta data that takes an instruction and stores it
*        in a node
*
* @details If this is the first instruction being read the data is stored in
*          head, otherwise it is stored in a new node that is put at the
*          end of the linked list that head points to
*
* @param[in] instruction
*            holds the string containing the meta data instruction
*
* @param[in] head
*            pointer to the head of the linked list where the meta data
*            will be stored
*
* @return None
*
* @note: None
*/
static void HandleInstruction(char* instruction, MetaDataNode *head)
{
    char* openParenthesis;
    char* closeParenthesis;
    int range;
    char command;
    char* opperation;
    char* stringToLongPtr;
    int cycleTime;

    command = instruction[0];

    openParenthesis = strchr(instruction, '(');
    closeParenthesis = strchr(instruction, ')');
    range = closeParenthesis - openParenthesis - 1;

    opperation = malloc(sizeof(range));
    strncpy(opperation, openParenthesis + 1, range);
    opperation[range] = '\0';

    cycleTime = strtol(closeParenthesis + 1, &stringToLongPtr, 10);

    if(!head->command)
    {
        head->command = command;
        strcpy(head->opperation, opperation);
        head->cycleTime = cycleTime;
    }
    else
    {
        AddToList(head, command, opperation, cycleTime);
    }

    free(opperation);
}

/*
* @brief Reads configuration information from a config file and stores it
*        in a ConfigInfo struct
*
* @param[in] configFileName
*            holds the string for the configuration file name
*
* @param[in] configData
*            pointer to the struct where the configuration data will be stored
*
* @return error codes: 0 represents no error, negative numbers represent errors
*
* @note: None
*/
int ReadConfig(char* configFileName, ConfigInfo *configData)
{
    char schedulingCode[6];
    char logTo[7];
    FILE *configFile;
    char line[256];
    configFile = fopen(configFileName, "r");

    if(configFile == NULL)
    {
        return -3;
    }

    while(fgets(line, sizeof(line), configFile))
    {
        sscanf(line, "Version/Phase: %d", &configData->versionPhase);
        sscanf(line, "File Path: %s", configData->filePath);
        sscanf(line, "CPU Scheduling Code: %s", schedulingCode);
        sscanf(line, "Quantum Time (cycles): %d",
            &configData->quantumTime);

        sscanf(line, "Memory Available (KB): %d",
            &configData->memoryAvailable);

        sscanf(line, "Processor Cycle Time (msec): %d",
            &configData->processorCycleTime);

        sscanf(line, "I/O Cycle Time (msec): %d",
            &configData->ioCycleTime);

        sscanf(line, "Log To: %s", logTo);
        sscanf(line, "Log File Path: %s", configData->logFilePath);
    }

    configData->cpuSchedulingCode = ConvertSchedulingCode(schedulingCode);
    if(configData->cpuSchedulingCode < 0)
    {
        return -1;
    }

    configData->logTo = ConvertLogTo(logTo);
    if(configData->logTo < 0)
    {
        return -2;
    }

    fclose(configFile);
    return 0;
}

/*
* @brief Reads meta data information from a meta data file and stores it
*        in a linked list
*
* @param[in] metaDataFileName
*            holds the string for the meta data name
*
* @param[in] head
*            pointer to the head of the linked list where the meta data
*            will be stored
*
* @return error codes: 0 represents no error, negative numbers represent errors
*
* @note: None
*/
int ReadMetaData(char* metaDataFileName, MetaDataNode *head)
{
    char line[256];
    char* instruction;
    FILE *metaDataFile;

    metaDataFile = fopen(metaDataFileName, "r");

    if(metaDataFile == NULL)
    {
        return -4;
    }

    fgets(line, sizeof(line), metaDataFile); //handles the Start Program line of the file

    while(fgets(line, sizeof(line), metaDataFile))
    {
        if(strcmp(line, "End Program Meta-Data Code.\n") != 0)
        {
            instruction = strtok(line, ";");
            while(instruction != NULL && instruction[0] != '\n')
            {
                HandleInstruction(instruction, head);

                instruction = strtok(NULL, ";.");
                if(instruction != NULL && instruction[0] == ' ')
                {
                    instruction++;  //cuts off the leading spaces
                }
            }
        }
    }

    fclose(metaDataFile);

    return 0;
}
