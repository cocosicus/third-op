#include "context.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_CONTEXT_VALUE 0

// allocate memory for Context structure
ErrorCode allocateContextMemory(Context** context) {
    ErrorCode result = OK;
    if (context == NULL) {
        result = INVALID_DATA_ERROR;
    } else {
        *context = (Context*)malloc(sizeof(Context));
        if (*context == NULL) {
            result = MEMORY_ALLOCATION_ERROR;
        }
    }
    return result;
}

ErrorCode checkContext(Context* context) {
    return context == NULL ? INVALID_DATA_ERROR : OK;
}

void clearContext(Context* context) {
    if (context != NULL) {
        if (context->dataList != NULL) {
            freeLinkedList(context->dataList);
        }
        if (context->calcValues != NULL) {
            freeCalcValues(context->calcValues);
        }
        if (context->stats != NULL) {
            freeStats(context->stats);
        }
        free(context);
        context = NULL;
    }
}

ErrorCode initializeContextMembers(Context* context) {
    ErrorCode result = checkContext(context);
    if (result == OK) {
        result = result == OK ? createLinkedList(&context->dataList) : result;
        result = result == OK ? createCalcValues(&context->calcValues) : result;
        result = result == OK ? createStats(&context->stats) : result;
        if (result == OK) {
            context->errorCode = OK;
            context->errorMessage[0] = '\0';
            context->totalRows = DEFAULT_CONTEXT_VALUE;
            context->errorRows = DEFAULT_CONTEXT_VALUE;
            context->validRows = DEFAULT_CONTEXT_VALUE;
        }
    }
    if (result != OK) {
        freeLinkedList(context->dataList);
        freeCalcValues(context->calcValues);
        freeStats(context->stats);
    }
    return result;
}


void createContext(Context** context) {
    ErrorCode result = context == NULL ? INVALID_DATA_ERROR : OK;
    if (result == OK) {
        result = allocateContextMemory(context);
        if (result == OK) {
            result = initializeContextMembers(*context);
            if (result != OK) {
                clearContext(*context);
            }
        }
    }
    if (result != OK && *context != NULL) {
        *context = NULL;
    }
}

