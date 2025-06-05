#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stddef.h>  // size_t
#include <stdbool.h> // bool

#include "ErrorCode.h"
#include "demographyData.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Node {
    DemographyData data;
    struct Node *next;
} Node;

typedef struct {
    Node *head;
    Node *tail;
    size_t size;
} LinkedList;

typedef struct {
    LinkedList *list;
    Node *current; // for iteration
} Iterator;

ErrorCode createLinkedList(LinkedList** list);
ErrorCode addNode(LinkedList* list, const DemographyData* data);
void clearLinkedList(LinkedList* list);
void freeLinkedList(LinkedList* list);
ErrorCode createIterator(Iterator** iter, const LinkedList* list);
DemographyData* next(Iterator* iterator);
void resetIterator(Iterator* iterator);
void freeIterator(Iterator* iterator);

#ifdef __cplusplus
}
#endif

#endif
