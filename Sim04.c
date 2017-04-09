/**
* @file Sim04.c
*
* @brief Driver program for running the simulator
*
* @details Uses parser and structures methods to read and store
* configuration data and meta data
*
* @commit
* C.S. Student (10 March 2017)
* Added support for FCFS-N and SJF-N, Added logging to file, added POSIX thread
* handling for I/O operations
*
* @commit
* C.S. Student (23 February 2017)
* Added support for multiple processes
*
* @commit
* C.S. Student (21 February 2017)
* Finished coding for Project 2
*
* @commit 12b42c2
* C.S. Student (3 February 2017)
* Finished adding all error checking for the parser
*
* @commit fd579a2
* C.S. Student (31 January 2017)
* Added documentation for all methods, implemented errorCheck
*
* @commit 030ed4b
* C.S. Student (30 January 2017)
* Added the reading of Meta Data
*
* @commit 795a4a0
* C.S. Student (27 January 2017)
* Added checking for command line arguements,
* parses in Config file and prints out the stored information
*
* @commit 501b0d6
* C.S. Student (26 January 2017)
* Basic creation of this file and creation of main
*
* @note Requires Structures.h, Structures.c, Parser.h, Parser.c
*       Process.h, Process.c
*/
#include "Sim04.h"

Boolean ErrorCheck(int errorNum);
PCB* GetNextProcess(ProcessListNode*, ConfigInfo*, QueueNode*);
QueueNode* GetNextQueue(QueueNode*);
void NonPreemptiveScheduling(ProcessListNode *processList,
    char* timer, char* filePrint, ConfigInfo configData, QueueNode *processQueue);
void PreemptiveScheduling(ProcessListNode *processList,
    char* timer, char* filePrint, ConfigInfo configData, QueueNode *processQueue);
void HandleInterupt(QueueNode *processQueue, QueueNode *interuptQueue);

// Main Function Implementation ///////////////////////////////////
int main(int argc, char const *argv[])
{
    char configFileName[40];
    ConfigInfo configData = {-1, "\0", -1, -1, -1, -1, -1, -1, "\0"};
    int errorNum = 0;
    MetaDataNode head = {};
    char* timer = malloc(256 * sizeof(char));
    ProcessListNode *processList = malloc(sizeof(ProcessListNode));
    ProcessListNode *processHead;
    char* monitorPrint = malloc(100 * sizeof(char));
    char* filePrint = malloc(10000 * sizeof(char));
    FILE *filePointer;
    QueueNode *processQueue = NULL;

    //ensures the proper command line arguements were given
    if(argc != 2)
    {
        printf("Usage: %s configFileName\n\n", argv[0]);
        exit(1);
    }
    else
    {
        strcpy(configFileName, argv[1]);
    }

    snprintf(monitorPrint, 100,
        "Operating System Simulator\n==========================\n\n");
    printf("%s", monitorPrint);
    strcpy(filePrint, monitorPrint);

    //load configuration data
    snprintf(monitorPrint, 100, "Loading configuration file\n");
    printf("%s", monitorPrint);
    strcat(filePrint, monitorPrint);

    errorNum = ReadConfig(configFileName, &configData);
    if(ErrorCheck(errorNum))
    {
        exit(1);
    }

    //load meta-data
    snprintf(monitorPrint, 100,
        "Loading meta-data file\n==========================\n\n");
    printf("%s", monitorPrint);
    strcat(filePrint, monitorPrint);

    errorNum = ReadMetaData(configData.filePath , &head);
    if(ErrorCheck(errorNum))
    {
        exit(1);
    }

    snprintf(monitorPrint, 100, "Begin Simulation\n");
    PrintIfLogToMonitor(monitorPrint, &configData);
    strcat(filePrint, monitorPrint);

    //start the timer and the system
    strcpy(timer, "0.000000");
    accessTimer(START_TIMER, timer);
    snprintf(monitorPrint, 100, "Time: %9s, System Start\n", timer);
    PrintIfLogToMonitor(monitorPrint, &configData);
    strcat(filePrint, monitorPrint);

    accessTimer(GET_TIME_DIFF, timer);
    snprintf(monitorPrint, 100, "Time: %9s, OS: Begin PCB Creation\n", timer);
    PrintIfLogToMonitor(monitorPrint, &configData);
    strcat(filePrint, monitorPrint);

    //create all needed processes in the New state, stored in processList
    CreateProcesses(processList, &head, &configData);
    accessTimer(GET_TIME_DIFF, timer);
    snprintf(monitorPrint, 100,
        "Time: %9s, OS: All processes initialized in New state\n", timer);
    PrintIfLogToMonitor(monitorPrint, &configData);
    strcat(filePrint, monitorPrint);

    //loop through the processes, setting each of them to ready and setting
    //their position in queue
    processHead = processList;
    while(processList != NULL)
    {
        SetReady(processList->process);
        processQueue = EnqueueFCFS(processQueue, processList->process);
        processList = processList->nextProcess;
    }
    //point the list back to the start instead of the end
    processList = processHead;
    accessTimer(GET_TIME_DIFF, timer);
    snprintf(monitorPrint, 100,
        "Time: %9s, OS: All processes now set in Ready state\n", timer);
    PrintIfLogToMonitor(monitorPrint, &configData);
    strcat(filePrint, monitorPrint);

    if(configData.cpuSchedulingCode == FCFSN
        || configData.cpuSchedulingCode == SJFN)
    {
        NonPreemptiveScheduling(processList, timer,
            filePrint, configData, processQueue);
    }
    else
    {
        PreemptiveScheduling(processList, timer,
            filePrint, configData, processQueue);
    }

    accessTimer(GET_TIME_DIFF, timer);
    snprintf(monitorPrint, 100, "Time: %9s, System stop\n", timer);
    PrintIfLogToMonitor(monitorPrint, &configData);
    strcat(filePrint, monitorPrint);

    //Log output to file
    if(configData.logTo == Both
        || configData.logTo == File)
    {
        filePointer = fopen (configData.logFilePath, "w+");
        fprintf(filePointer ,"%s", filePrint);
        fclose(filePointer);
    }


     return 0;
 }

// Free Function Implementation ///////////////////////////////////

/*
* @brief Checks if there have been errors
*
* @details Upon encountering an error prints out an error code
*
* @param[in] errorNum
*            holds the error code
*
* @return TRUE if there are errors, FALSE if there are not
*
* @note: None
*/
Boolean ErrorCheck(int errorNum)
{
    if(errorNum == 0)
    {
        return FALSE;
    }
    else if(errorNum == CPU_SCHEDULING_CODE_ERROR)
    {
        printf("Invalid value for 'CPU Scheduling Code' "
            "in the configuration file\n");
    }
    else if(errorNum == LOG_TO_ERROR)
    {
        printf("Invalid value for 'Log To' in the configuration file\n");
    }
    else if(errorNum == CONFIG_FILE_ERROR)
    {
        printf("Error opening configuration file\n");
    }
    else if(errorNum == META_DATA_FILE_ERROR)
    {
        printf("Error opening meta data file\n");
    }
    else if(errorNum == META_DATA_FORMAT_ERROR)
    {
        printf("Invalid meta data format\n");
    }
    else if(errorNum == CONFIG_FORMAT_ERROR)
    {
        printf("Missing required configuration information\n");
    }
    else if(errorNum == CONFIG_BOUNDS_ERROR)
    {
        printf("At least one configuration value is out of bounds\n");
    }
    else if(errorNum == WRONG_VERSION)
    {
        printf("Wrong Version Number\n");
    }
    return TRUE;
}

/*
* @brief Prints output to monitor if the the config file set logTo to either
*        Both or Monitor
*
* @param[in] string
*            the string that will be printed
*
* @param[in] configData
*             points to the data from the config file
*
* @return None
*
* @note: None
*/
void PrintIfLogToMonitor(char* string, ConfigInfo *configData)
{
    if(configData->logTo == -1 || configData->logTo == Both
        || configData->logTo == Monitor)
    {
        printf("%s", string);
    }
}

/*
* @brief Gets the next process to be run based on the CPU Scheduling code
*
* @param[in] processList
*            points to the list of procseses currently in memory
*
* @param[in] configData
*             points to the data from the config file
*
* @return The next process to be run
*
* @note: None
*/
PCB* GetNextProcess(ProcessListNode *processList,
    ConfigInfo *configData, QueueNode *queue)
{
    ProcessListNode *tempList = processList;
    PCB *tempProcess = malloc(sizeof(PCB));
    int minCycleTime = 0;
    Boolean noneReady = TRUE;

    //if any processes are ready set noneReady to false
    while(tempList != NULL)
    {
        if(tempList->process->state == Ready)
        {
            noneReady = FALSE;
        }
        tempList = tempList->nextProcess;
    }

    if(noneReady)
    {
        return NULL;
    }

    tempList = processList;

    //get process using FCFS-N selection
    if(configData->cpuSchedulingCode == FCFSN)
    {
        while(tempList->process->state != Ready)
        {
            tempList = tempList->nextProcess;
        }
        return tempList->process;
    }
    //get processes using SJF-N selection
    else if(configData->cpuSchedulingCode == SJFN)
    {
        //loop until you find a ready process
        while(tempList->process->state != Ready && tempList != NULL)
        {
            tempList = tempList->nextProcess;
        }

        tempProcess = tempList->process;
        minCycleTime = tempList->process->cycleTime;

        //loop through the remaining processes
        while(tempList != NULL)
        {
            //if the process is shorter set it as the one to be returned
            if(tempList->process->state == Ready
                && tempList->process->cycleTime < minCycleTime)
            {
                tempProcess = tempList->process;
                minCycleTime = tempList->process->cycleTime;
            }
            tempList = tempList->nextProcess;
        }
        return tempProcess;
    }
    return NULL;
}

QueueNode* GetNextQueue(QueueNode *queue)
{
    queue = Dequeue(queue);
    return queue;
}

void NonPreemptiveScheduling(ProcessListNode *processList, char* timer,
     char* filePrint, ConfigInfo configData, QueueNode *processQueue)
{
    char* monitorPrint = malloc(100 * sizeof(char));
    PCB *selectedProcess;
    //Select the first process to run
    selectedProcess = GetNextProcess(processList, &configData, processQueue);
    accessTimer(GET_TIME_DIFF, timer);
    snprintf(monitorPrint, 100,
        "Time: %9s, OS: %s Strategy selects Process %d with time: %d mSec \n",
        timer, convertSchedulingCode(configData.cpuSchedulingCode),
        selectedProcess->procNum, selectedProcess->cycleTime);
    PrintIfLogToMonitor(monitorPrint, &configData);
    strcat(filePrint, monitorPrint);

    // for each process:
    while(selectedProcess != NULL)
    {
        //set it to running
        SetRunning(selectedProcess);
        accessTimer(GET_TIME_DIFF, timer);
        snprintf(monitorPrint, 100,
            "Time: %9s, OS: Process %d set in Running state\n",
            timer, selectedProcess->procNum);
        PrintIfLogToMonitor(monitorPrint, &configData);
        strcat(filePrint, monitorPrint);

        //execute all commands
        while(selectedProcess->currentNode != NULL)
        {
            Run(selectedProcess, &configData, timer, filePrint);
        }

        //set it to exit
        SetExit(selectedProcess);
        accessTimer(GET_TIME_DIFF, timer);
        snprintf(monitorPrint, 100,
            "Time: %9s, OS: Process %d set in Exit state\n",
            timer, selectedProcess->procNum);
        PrintIfLogToMonitor(monitorPrint, &configData);
        strcat(filePrint, monitorPrint);

        //Select the next process to run
        selectedProcess = GetNextProcess(processList, &configData, processQueue);
        if(selectedProcess != NULL)
        {
            accessTimer(GET_TIME_DIFF, timer);
            snprintf(monitorPrint, 100,
                "Time: %9s, OS: %s Strategy selects Process %d with time: %d mSec \n",
                timer, convertSchedulingCode(configData.cpuSchedulingCode),
                selectedProcess->procNum, selectedProcess->cycleTime);
            PrintIfLogToMonitor(monitorPrint, &configData);
            strcat(filePrint, monitorPrint);
        }
    }
}

void PreemptiveScheduling(ProcessListNode *processList, char* timer,
    char* filePrint, ConfigInfo configData, QueueNode *processQueue)
{
    char* monitorPrint = malloc(100 * sizeof(char));
    QueueNode *interuptQueue = NULL;
    Boolean finished = FALSE;
    Boolean idle = FALSE;
    int procState = PROC_READY;
    ProcessListNode *tempProcList = processList;
    PCB *selectedProcess;

    selectedProcess = processQueue->process;
    processQueue = Dequeue(processQueue);

    while(!finished)
    {

        //handle if no processes are ready
        if(selectedProcess == NULL)
        {
            idle = TRUE;

            accessTimer(GET_TIME_DIFF, timer);
            snprintf(monitorPrint, 100,
                "Time: %9s, Processor/System Idle start\n", timer);
            PrintIfLogToMonitor(monitorPrint, &configData);
            strcat(filePrint, monitorPrint);

            while(idle){
                HandleInterupt(processQueue, interuptQueue);
                if(processQueue != NULL)
                {
                    selectedProcess = processQueue->process;
                    if(selectedProcess != NULL)
                    {

                        processQueue = GetNextQueue(processQueue);
                        idle = FALSE;
                    }
                }
            }

            accessTimer(GET_TIME_DIFF, timer);
            snprintf(monitorPrint, 100,
                "Time: %9s, Processor/System Idle end\n", timer);
            PrintIfLogToMonitor(monitorPrint, &configData);
            strcat(filePrint, monitorPrint);

            accessTimer(GET_TIME_DIFF, timer);
            snprintf(monitorPrint, 100,
                "Time: %9s, OS: %s Strategy selects Process %d with time: %d mSec \n",
                timer, convertSchedulingCode(configData.cpuSchedulingCode),
                selectedProcess->procNum, selectedProcess->cycleTime);
            PrintIfLogToMonitor(monitorPrint, &configData);
            strcat(filePrint, monitorPrint);
        }

        //Run the processes current operation
        SetRunning(selectedProcess);
        accessTimer(GET_TIME_DIFF, timer);
        snprintf(monitorPrint, 100,
            "Time: %9s, OS: Process %d set in Running state\n",
            timer, selectedProcess->procNum);
        PrintIfLogToMonitor(monitorPrint, &configData);
        strcat(filePrint, monitorPrint);

        procState = PreemptiveRun(selectedProcess, &configData, timer,
            filePrint, interuptQueue, processQueue);

        if(procState == PROC_BLOCK)
        {
            SetBlocked(selectedProcess);
            accessTimer(GET_TIME_DIFF, timer);
            snprintf(monitorPrint, 100,
                "Time: %9s, OS: Process %d set in Blocked state\n",
                timer, selectedProcess->procNum);
            PrintIfLogToMonitor(monitorPrint, &configData);
            strcat(filePrint, monitorPrint);
        }
        else if(procState == PROC_EXIT)
        {
            SetExit(selectedProcess);
            accessTimer(GET_TIME_DIFF, timer);
            snprintf(monitorPrint, 100,
                "Time: %9s, OS: Process %d set in Exit state\n",
                timer, selectedProcess->procNum);
            PrintIfLogToMonitor(monitorPrint, &configData);
            strcat(filePrint, monitorPrint);
        }
        else
        {
            SetReady(selectedProcess);
            accessTimer(GET_TIME_DIFF, timer);
            snprintf(monitorPrint, 100,
                "Time: %9s, OS: Process %d set in Ready state\n",
                timer, selectedProcess->procNum);
            PrintIfLogToMonitor(monitorPrint, &configData);
            strcat(filePrint, monitorPrint);
        }

        HandleInterupt(processQueue, interuptQueue);

        if(processQueue != NULL)
        {
            selectedProcess = processQueue->process;
            processQueue = GetNextQueue(processQueue);
            accessTimer(GET_TIME_DIFF, timer);
            snprintf(monitorPrint, 100,
                "Time: %9s, OS: %s Strategy selects Process %d with time:"
                " %d mSec \n", timer, 
                convertSchedulingCode(configData.cpuSchedulingCode),
                selectedProcess->procNum, selectedProcess->cycleTime);
            PrintIfLogToMonitor(monitorPrint, &configData);
            strcat(filePrint, monitorPrint);
        }
        else
        {
            finished = TRUE;

            //Loops through all the processes
            while(tempProcList != NULL && tempProcList->process != NULL)
            {
                //If any are not in exit state, set finished to false
                if(tempProcList->process->state != Exit)
                {
                    finished = FALSE;
                    selectedProcess = NULL;
                }
                tempProcList = tempProcList->nextProcess;
            }
            tempProcList = processList;
        }
    }
}

void HandleInterupt(QueueNode *processQueue, QueueNode *interuptQueue)
{
    while(interuptQueue != NULL)
    {
        processQueue = EnqueueFCFS(processQueue, interuptQueue->process);

        interuptQueue = interuptQueue->next;
    }
}
