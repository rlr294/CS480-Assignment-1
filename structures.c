/**
* @file structures.c
*
* @brief Implementation file for structures class
*
* @details Implements all member methods of the structures class
*
* @version 1.00
* C.S. Student (26 January 2017)
* Initial development and testing of structures class
*
* @note Requires structures.h
*/
#include <stdio.h>
#include "Structures.h"

void AddToList(MetaDataNode *head, MetaDataNode *newNode)
{
    MetaDataNode *currentNode = head;
    while(currentNode->nextNode != NULL)
    {
        currentNode = currentNode->nextNode;
    }
    currentNode->nextNode = newNode;
}

void printList(MetaDataNode *head)
{
    MetaDataNode *currentNode = head;
    while(currentNode != NULL)
    {
        printf();
        currentNode = currentNode->nextNode;
    }
}

void printConfig(ConfigInfo *configuration)
{

}
