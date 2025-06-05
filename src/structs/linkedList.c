#include "linkedList.h"
#include <stdlib.h>  // malloc, free
#include <string.h>  // memcpy

#define DEFAULT_LIST_VALUE 0

ErrorCode createLinkedList(LinkedList** list) {
    ErrorCode result = list == NULL ? INVALID_DATA_ERROR : OK;
    if (result == OK) {
        *list = (LinkedList*)malloc(sizeof(LinkedList));
        if (*list == NULL) {
            result = MEMORY_ALLOCATION_ERROR;
        }
        else {
            (*list)->head = NULL;
            (*list)->tail = NULL;
            (*list)->size =  DEFAULT_LIST_VALUE;
        }
    }
    return result;
}

ErrorCode addNode(LinkedList* list, const DemographyData* data) {
    ErrorCode result = INVALID_DATA_ERROR;
    Node* newNode = NULL;

    if (list != NULL && data != NULL) {
        newNode = (Node*)malloc(sizeof(Node));
        if (newNode != NULL) {
            newNode->data = *data; // Copy
            newNode->next = NULL;

            if (list->head == NULL) {
                list->head = newNode;
                list->tail = newNode;
            } else {
                list->tail->next = newNode;
                list->tail = newNode;
            }
            list->size++;
            result = OK;
        } else {
            result = MEMORY_ALLOCATION_ERROR;
        }
    }
    if(result != OK && newNode != NULL){
        free(newNode);
        newNode = NULL;
    }

    return result;
}

void clearLinkedList(LinkedList* list) {
    if (list != NULL) {
        Node* current = list->head;
        Node* next;

        while (current != NULL) {
            next = current->next;
            free(current);
            current = next;
        }

        list->head = NULL;
        list->tail = NULL;
        list->size = 0;
    }
}

void freeLinkedList(LinkedList* list) {
    if (list != NULL) {
        clearLinkedList(list);
        free(list);
        list = NULL;
    }
}

ErrorCode createIterator(Iterator** iter, const LinkedList* list) {
    ErrorCode result = (iter == NULL || list == NULL) ? INVALID_DATA_ERROR :  OK;
    if (result == OK) {
        *iter = (Iterator*)malloc(sizeof(Iterator));
        if (*iter == NULL) {
            result = MEMORY_ALLOCATION_ERROR;
        }

        else {
            (*iter)->list = list;
            (*iter)->current = list->head;
        }
    }
    return result;
}

DemographyData* next(Iterator* iterator) {
    DemographyData* data = NULL;
    if (iterator != NULL && iterator->list != NULL && iterator->current != NULL) {
        data = &(iterator->current->data);
        iterator->current = iterator->current->next;
    }
    return data;
}

void resetIterator(Iterator* iterator) {
    if (iterator != NULL && iterator->list != NULL) {
        iterator->current = iterator->list->head;
    }
}

void freeIterator(Iterator* iterator) {
    if(iterator != NULL){
        free(iterator);
    }
}
