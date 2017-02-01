/**
* @file sim01.h
*
* @brief Header file for the simulation program
*
* @details Executes all the commands needed to make the program run
*
*
* @commit 501b0d6
* C.S. Student (26 January 2017)
* Definition of Boolean
*
* @note None
*/
#ifndef SIM01_H
#define SIM01_H

#define CPU_SCHEDULING_CODE_ERROR -1
#define LOG_TO_ERROR -2
#define CONFIG_FILE_ERROR -3
#define META_DATA_FILE_ERROR -4

typedef enum {FALSE, TRUE} Boolean;

void errorCheck(int errorNum);

#endif //SIM01_H
