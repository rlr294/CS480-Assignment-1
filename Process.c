/**
* @file Process.c
*
* @brief Implementation file for Process class
*
* @details Implements all member methods of the Process class
*
* @commit
* C.S. Student (21 February 2017)
* Implements the methods
*
* @commit
* C.S. Student (14 February 2017)
* Basic creation of this file
*
* @note Requires Process.h
*/

#include "Process.h"

// Free Function Implementation ///////////////////////////////////

/*
* @brief Initializes a process with its new values
*
* @param[in] process
*            points to the PCB of a process
*
* @param[in] node
*            points to the head node for the process
*
* @param[in] procNum
*            the number associated with this process
*
* @return None
*
* @note: None
*/
void NewProcess(PCB *process, MetaDataNode *node, int procNum)
{
    process->state = New;
    process->currentNode = node;
    process->procNum = procNum;
}

/*
* @brief Initializes a process with its new values
*
* @param[in] process
*            points to the PCB of a process
*
* @param[in] configData
*            points to the data from the config file
*
* @param[in] timer
*            the current timer value
*
* @return 0 if no errors, negative for errors
*
* @note: None
*/
int Run(PCB *process, ConfigInfo *configData, char* timer)
{
    if(process->currentNode->cycleTime != 0)
    {
        //prints the start time
        accessTimer(GET_TIME_DIFF, timer);
        printf("Time: %9s, Process %d, %s start\n",
            timer, process->procNum, NodeToString(process->currentNode));

        //delays for duration determined by if this is an I/O or other command
        if(process->currentNode->command == 'I'
            || process->currentNode->command == 'O')
        {
            delay(configData->ioCycleTime * process->currentNode->cycleTime);
        }
        else
        {
            delay(configData->pCycleTime * process->currentNode->cycleTime);
        }

        //prints the end time
        accessTimer(GET_TIME_DIFF, timer);
        printf("Time: %9s, Process %d, %s end\n",
            timer, process->procNum, NodeToString(process->currentNode));
    }

    process->currentNode = process->currentNode->nextNode;
    return 0;
}

/*
* @brief Sets a process to the ready state
*
* @param[in] process
*            points to the PCB of a process
*
* @return None
*
* @note: None
*/
void SetReady(PCB *process)
{
    process->state = Ready;
}

/*
* @brief Sets a process to the running state
*
* @param[in] process
*            points to the PCB of a process
*
* @return None
*
* @note: None
*/
void SetRunning(PCB *process)
{
    process->state = Running;
}

/*
* @brief Sets a process to the exit state
*
* @param[in] process
*            points to the PCB of a process
*
* @return None
*
* @note: None
*/
void SetExit(PCB *process)
{
    process->state = Exit;
}
