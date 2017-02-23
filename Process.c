/**
* @file Process.c
*
* @brief Implementation file for Process class
*
* @details Implements all member methods of the Process class
*
* @commit
* C.S. Student (23 February 2017)
* Added support for multiple processes
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
* @brief Creates all processses outlined in the meta-data file and
*        stores them in the ProcessList passed as a parameter
*
* @param[in] list
*            points to the list of PCB's
*
* @param[in] node
*            points to the head node for the process
*
* @return None
*
* @note: None
*/
void CreateProcesses(ProcessListNode *list, MetaDataNode *node)
{
    PCB *newProc;
    ProcessListNode *newListNode;
    MetaDataNode *tempNode;
    int procNum = 0;

    while(node->nextNode != NULL)
    {
        tempNode = node->nextNode;
        if(node->command == 'A' && strcmp(node->operation, "start") == 0)
        {
            newProc = malloc(sizeof(PCB));
            newProc->state = New;
            newProc->currentNode = node;
            newProc->procNum = procNum;
            procNum++;

            if(list->process == NULL)
            {
                list->process = newProc;
            }
            else
            {
                newListNode = malloc(sizeof(ProcessListNode));
                list->nextProcess = newListNode;
                list = list->nextProcess;
                list->process = newProc;
                list->nextProcess = NULL;
            }
        }
        else if(node->command == 'A' && strcmp(node->operation, "end") == 0)
        {
            node->nextNode = NULL;
        }
        node = tempNode;
    }
}

/*
* @brief Runs the current command that a processes is looking at
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
        else if(process->currentNode->command == 'M')
        {
            //don't delay for memory operations
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
