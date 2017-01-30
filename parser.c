/**
* @file parser.c
*
* @brief Implementation file for parser class
*
* @details Implements all member methods of the paser class
*
* @version 1.00
* C.S. Student (26 January 2017)
* Initial development and testing of parser class
*
* @note Requires parser.h
*/
#include <string.h>
#include <stdlib.h>
#include "Parser.h"

static int convertSchedulingCode(char* codeString)
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

static int convertLogTo(char* logString)
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

int ReadConfig(char* configFileName, ConfigInfo *configData)
{
    char schedulingCode[6];
    char logTo[7];
    FILE *configFile;
    char line[256];
    configFile = fopen(configFileName, "r");

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

    configData->cpuSchedulingCode = convertSchedulingCode(schedulingCode);
    if(configData->cpuSchedulingCode < 0)
    {
        return -1;
    }

    configData->logTo = convertLogTo(logTo);
    if(configData->logTo < 0)
    {
        return -1;
    }

    fclose(configFile);
    return 0;
}

int ReadMetaData(char* metaDataFileName, MetaDataNode *head)
{
    char line[256];
    char* instruction;
    char* openParenthesis;
    char* closeParenthesis;
    char command;
    char* opperation;
    char* stringToLongPtr;
    int cycleTime;
    FILE *metaDataFile;

    metaDataFile = fopen(metaDataFileName, "r");

    fgets(line, sizeof(line), metaDataFile); //handles the Start Program line of the file
    while(fgets(line, sizeof(line), metaDataFile))
    {
        if(strcmp(line, "End Program Meta-Data Code.\n") != 0)
        {
            instruction = strtok(line, ";");
            while(instruction != NULL && instruction[0] != '\n')
            {
                printf("%s\n", instruction);

                command = instruction[0];

                openParenthesis = strchr(instruction, '(');
                closeParenthesis = strchr(instruction, ')');

                opperation = malloc(sizeof(closeParenthesis - openParenthesis - 1));
                strncpy(opperation, openParenthesis + 1, closeParenthesis - openParenthesis - 1);

                cycleTime = strtol(closeParenthesis + 1, &stringToLongPtr, 10);

                if(!head->command)
                {
                    head->command = command;
                    strcpy(head->opperation, opperation);
                }
                else
                {
                    AddToList(head, command, opperation, cycleTime);
                }

                free(opperation);

                instruction = strtok(NULL, ";.");
                if(instruction != NULL)
                {
                    if(instruction[0] == ' ')
                    {
                        instruction++;
                    }
                }
            }
        }
    }

    fclose(metaDataFile);

    return 0;
}
