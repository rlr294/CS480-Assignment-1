/**
* @file parser.h
*
* @brief Header file for the data parser
*
* @details Contains methods for reading configuration and
* meta data files and storing them into structures
*
* @commit 795a4a0
* C.S. Student (27 January 2017)
* Added declarations for ReadConfig and ReadMetaData
*
* @commit 501b0d6
* C.S. Student (26 January 2017)
* Basic creation of this file
*
* @note None
*/
#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include "Structures.h"

#define NONE 0
#define FCFS_N 1
#define SJF_N 2
#define SRTF_P 3
#define FCFS_P 4
#define RR_P 5

#define LOG_TO_MONITOR 0
#define LOG_TO_FILE 1
#define LOG_TO_BOTH 2

int ReadConfig(char* configFileName, ConfigInfo *configData);

int ReadMetaData(char* metaDataFileName, MetaDataNode *head);

#endif //PARSER_H
