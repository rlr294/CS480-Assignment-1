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

/* function to swap data of two nodes a and b*/
void swap(ProcessListNode *a, ProcessListNode *b)
{
    PCB* temp = a->process;
    a->process = b->process;
    b->process = temp;
}

/* Bubble sort the given linked list */
void shortestJobFirstSort(ProcessListNode *start)
{
    int swapped;
    ProcessListNode *ptr1 = start;
    ProcessListNode *lptr = NULL;

    do
    {
        swapped = 0;
        ptr1 = start;

        while (ptr1->nextProcess != lptr)
        {
            if (ptr1->process->cycleTime > ptr1->nextProcess->process->cycleTime)
            {
                swap(ptr1, ptr1->nextProcess);
                swapped = 1;
            }
            ptr1 = ptr1->nextProcess;
        }
        lptr = ptr1;
    }
    while (swapped);
}


int calcCycleTime(MetaDataNode *node, ConfigInfo *configData)
{
    //cycle times for I/O commands
    if(node->command == 'I'
        || node->command == 'O')
    {
        return configData->ioCycleTime * node->cycleTime;
    }
    //cycle times for memory commands
    else if(node->command == 'M')
    {
        return 0;
    }
    //cycle times for all other commands
    else
    {
        return configData->pCycleTime * node->cycleTime;
    }
}

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
void CreateProcesses(ProcessListNode *list, MetaDataNode *node, ConfigInfo *configData)
{
    PCB *newProc;
    ProcessListNode *newListNode;
    MetaDataNode *tempNode;
    MetaDataNode *tempNode2;
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
            newProc->cycleTime = 0;
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

            tempNode2 = newProc->currentNode;
            while(tempNode2 != NULL)
            {
                newProc->cycleTime = newProc->cycleTime + calcCycleTime(tempNode2, configData);
                tempNode2 = tempNode2->nextNode;
            }
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

        delay(calcCycleTime(process->currentNode, configData));
        //delays for duration determined by if this is an I/O or other command
        // if(process->currentNode->command == 'I'
        //     || process->currentNode->command == 'O')
        // {
        //     delay(configData->ioCycleTime * process->currentNode->cycleTime);
        // }
        // else if(process->currentNode->command == 'M')
        // {
        //     //don't delay for memory operations
        // }
        // else
        // {
        //     delay(configData->pCycleTime * process->currentNode->cycleTime);
        // }

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
