#ifndef ENTRY_POINT_H
#define ENTRY_POINT_H

#include "context.h"
#include "demographyData.h"
#include "linkedList.h"

#define MAX_FILE_NAME 256

typedef enum Operation {
    CreateContext,
    ClearContext,
    LoadData,
    CalcStats,
} Operation;

typedef struct {
    union {
        // Параметры для LoadData
        struct {
            char filename[MAX_FILE_NAME];
        } loadDataParams;

        // Параметры для CalculateStats
        struct {
            char region[MAX_REGION_LENGTH];
            int column;
        } calculateStatsParams;

    } params;
} Params;

void doOperation(Operation operation, Context** context, Params* param);

#endif
