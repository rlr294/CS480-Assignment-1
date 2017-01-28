/**
* @file parser.h
*
* @brief Header file for the data parser
*
* @details Contains methods for reading configuration and
* meta data files and storing them into structures
*
*
* @version 1.00
* C.S. Student (26 January 2017)
* Initial development of the parser
*
* @note None
*/
#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include "Structures.h"

int ReadConfig(char* configFileName, ConfigInfo *configData);

int ReadMetaData(char* metaDataFileName, MetaDataNode *head);

#endif
