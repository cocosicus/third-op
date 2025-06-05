#ifndef CONTEXT_H
#define CONTEXT_H

#include "linkedList.h"
#include "calcValues.h"
#include "stats.h"

// context inf
typedef struct {
    LinkedList *dataList;
    ErrorCode errorCode;
    char errorMessage[256];
    int totalRows;
    int errorRows;
    int validRows;
    CalcValues* calcValues;
    Stats* stats;
} Context;

// context
void createContext(Context** context);
void clearContext(Context* context);

#endif // CONTEXT_H
