/**
* @file structures.c
*
* @brief Driver program for running the simulator
*
* @details Uses parser and structures methods to read and store
* configuration data and meta data
*
* @version 1.00
* C.S. Student (26 January 2017)
* Initial development and testing of the sim01 assignment
*
* @note Requires structures.h, structures.c, parser.h, parser.c
*/
#include <stdio.h>
#include <string.h>
#include "Sim01.h"
#include "Structures.h"
#include "Parser.h"

int main(int argc, char const *argv[])
{
    char configFileName[40];
    ConfigInfo configData = {};
    int errorCheck = 0;

    if(argc != 2)
    {
        printf("Usage: %s configFileName\n\n", argv[0]);
    }
    else
    {
        strcpy(configFileName, argv[1]);
    }

    errorCheck = ReadConfig(configFileName, &configData);
    if(errorCheck < 0)
    {
        printf("You broke it\n\n");
        return -1;
    }

    PrintConfig(&configData);


    return 0;
}
