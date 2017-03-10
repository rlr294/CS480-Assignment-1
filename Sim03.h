/**
* @file sim03.h
*
* @brief Header file for the simulation program
*
* @details Executes all the commands needed to make the program run
*
* @commit 501b0d6
* C.S. Student (26 January 2017)
* Definition of Boolean
*
* @note None
*/
#ifndef SIM03_H
#define SIM03_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Parser.h"
#include "Process.h"
#include "SimpleTimer.h"
#include "Structures.h"

typedef enum {FALSE, TRUE} Boolean;

void printIfLogToMonitor(char*, ConfigInfo*);

#endif //SIM03_H
