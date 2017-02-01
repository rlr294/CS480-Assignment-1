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

int ReadConfig(char* configFileName, ConfigInfo *configData);

int ReadMetaData(char* metaDataFileName, MetaDataNode *head);

#endif //PARSER_H
