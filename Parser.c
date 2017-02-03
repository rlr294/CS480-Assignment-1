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
* Added a null terminating character after parsing in operation
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
#include "Sim01.h"

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
        return NONE;    
    }
    else if(strcmp(codeString, "FCFS-N") == 0)
    {
        return FCFS_N;
    }
    else if(strcmp(codeString, "SJF-N") == 0)
    {
        return SJF_N;
    }
    else if(strcmp(codeString, "SRTF-P") == 0 )
    {
        return SRTF_P;
    }
    else if(strcmp(codeString, "FCFS-P") == 0)
    {
        return FCFS_P;
    }
    else if(strcmp(codeString, "RR-P") == 0)
    {
        return RR_P;
    }
    else
    {
        return CPU_SCHEDULING_CODE_ERROR;
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
        return LOG_TO_MONITOR;
    }
    else if(strcmp(logString, "File") == 0)
    {
        return LOG_TO_FILE;
    }
    else if(strcmp(logString, "Both") == 0)
    {
        return LOG_TO_BOTH;
    }
    else
    {
        return LOG_TO_ERROR;
    }
}

void RemoveSpaces(char* source)
{
  char* i = source;
  char* j = source;
  while(*j != 0)
  {
    *i = *j++;
    if(*i != ' ' && *i != '\n')
      i++;
  }
  *i = 0;
}

Boolean IsValidCommand(char command)
{
    if(command == 'S' || command == 'A' || command == 'P'
        || command == 'M' || command == 'I' || command == 'O')
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }

}

Boolean IsValidOperation(char* operation)
{
    if(strcmp(operation, "access") == 0|| strcmp(operation, "allocate") == 0
        || strcmp(operation, "end") == 0 || strcmp(operation, "harddrive") == 0
        || strcmp(operation, "keyboard") == 0
        || strcmp(operation, "printer") == 0||strcmp(operation, "monitor") == 0
        || strcmp(operation, "run") == 0 || strcmp(operation, "start") == 0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
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
* @return error codes: 0 represents no error, negative numbers represent errors
*
* @note: None
*/
static int HandleInstruction(char* instruction, MetaDataNode *head)
{
    char* openParenthesis;
    char* closeParenthesis;
    int range;
    char command;
    char* operation;
    char* stringToLongPtr;
    int cycleTime;

    command = instruction[0];
    if(!IsValidCommand(command) || instruction[1] != '(')
    {
        return META_DATA_FORMAT_ERROR;
    }

    //gets the location of the parenthesis and how many characters are
    //between them
    openParenthesis = strchr(instruction, '(');
    closeParenthesis = strchr(instruction, ')');
    if(openParenthesis == NULL || closeParenthesis == NULL)
    {
        return META_DATA_FORMAT_ERROR;
    }
    range = closeParenthesis - openParenthesis - 1;

    //stores the string between the parenthesis into operation
    operation = malloc(sizeof(range));
    strncpy(operation, openParenthesis + 1, range);
    operation[range] = '\0';
    if(!IsValidOperation(operation))
    {
        return META_DATA_FORMAT_ERROR;
    }

    cycleTime = strtol(closeParenthesis + 1, &stringToLongPtr, 10);
    if(stringToLongPtr == closeParenthesis + 1 || stringToLongPtr[0] != '\0')
    {
        return META_DATA_FORMAT_ERROR;
    }

    //if head hasn't been used yet store the data in head,
    //otherwise use AddToList to create a new node to hold the data
    if(!head->command)
    {
        head->command = command;
        strcpy(head->operation, operation);
        head->cycleTime = cycleTime;
    }
    else
    {
        AddToList(head, command, operation, cycleTime);
    }

    free(operation);
    return 0;
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
        return CONFIG_FILE_ERROR;
    }

    while(fgets(line, sizeof(line), configFile))
    {
        RemoveSpaces(line);
        sscanf(line, "Version/Phase:%d", &configData->versionPhase);
        sscanf(line, "FilePath:%s", configData->filePath);
        sscanf(line, "CPUSchedulingCode:%s", schedulingCode);
        sscanf(line, "QuantumTime(cycles):%d",
            &configData->quantumTime);

        sscanf(line, "MemoryAvailable(KB):%d",
            &configData->memoryAvailable);

        sscanf(line, "ProcessorCycleTime(msec):%d",
            &configData->processorCycleTime);

        sscanf(line, "I/OCycleTime(msec):%d",
            &configData->ioCycleTime);

        sscanf(line, "LogTo:%s", logTo);
        sscanf(line, "LogFilePath:%s", configData->logFilePath);
    }

    configData->cpuSchedulingCode = ConvertSchedulingCode(schedulingCode);
    if(configData->cpuSchedulingCode < 0)
    {
        return CPU_SCHEDULING_CODE_ERROR;
    }

    configData->logTo = ConvertLogTo(logTo);
    if(configData->logTo < 0)
    {
        return LOG_TO_ERROR;
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
    int errorNum = 0;

    metaDataFile = fopen(metaDataFileName, "r");

    if(metaDataFile == NULL)
    {
        return META_DATA_FILE_ERROR;
    }

    //fgets(line, sizeof(line), metaDataFile); //handles the Start Program line of the file

    while(fgets(line, sizeof(line), metaDataFile))
    {
        RemoveSpaces(line);
        if(strcmp(line, "EndProgramMeta-DataCode.") != 0
            && strcmp(line, "StartProgramMeta-DataCode:") != 0)
        {
            instruction = strtok(line, ";");
            while(instruction != NULL && instruction[0] != '\n')
            {
                errorNum = HandleInstruction(instruction, head);
                if(errorNum < 0)
                {
                    return errorNum;
                }

                instruction = strtok(NULL, ";.");
            }
        }
    }

    fclose(metaDataFile);

    return 0;
}
