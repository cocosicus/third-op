#include "calcStructs.h"
#include <stdlib.h>

#define DEFAULT_STATS_VALUE 0

ErrorCode allocateCalcValuesMemory(CalcValues** calcValues) {
    result = calcValues == NULL ? INVALID_DATA_ERROR : OK;
    if (result == OK) {
        *calcValues = (CalcValues*)malloc(sizeof(CalcValues));
        if (*calcValues == NULL) {
            result = MEMORY_ALLOCATION_ERROR;
        }
    }
    return result;
}

ErrorCode allocateStatsMemory(Stats** stats) {
    result = stats == NULL ? INVALID_DATA_ERROR : OK;
    if (result == OK) {
        *stats = (Stats*)malloc(sizeof(Stats));
        if (*stats == NULL) {
            result = MEMORY_ALLOCATION_ERROR;
        }
    }
    return result;
}

ErrorCode initializeCalcValues(CalcValues* calcValues) {
    ErrorCode result = calcValues == NULL ? INVALID_DATA_ERROR : OK;
    if (result == OK) {
        calcValues->max = DEFAULT_STATS_VALUE;
        calcValues->min = DEFAULT_STATS_VALUE;
        calcValues->median = DEFAULT_STATS_VALUE;
    }
    return result;
}

ErrorCode initializeStats(Stats* stats) {
    ErrorCode result = stats == NULL ? INVALID_DATA_ERROR : OK;
    if (result == OK) {
        stats->values = NULL;
        stats->years = NULL;
        stats->valuesCount = DEFAULT_VALUE;
    }
}

void freeCalcValues(CalcValues** calcValues) {
    if (calcValues != NULL && *calcValues != NULL) {
        free(*calcValues);
        *calcValues = NULL;
    }
}

void freeStats(Stats** stats) {
    if (stats != NULL && *stats != NULL) {
        if ((*stats)->values != NULL) {
            free ((*stats)->values);
            (*stats)->values = NULL;
        }
        if ((*stats)->years != NULL) {
            free ((*stats)->years);
            (*stats)->years = NULL;
        }
        free(*stats);
        *stats = NULL;
    }
}

ErrorCode createCalcValues(CalcValues** calcValues) {
    ErrorCode result = allocateCalcValuesMemory(calcValues);
    if (result == OK) {
        result = initializeCalcValues(*calcValues);
    }
    return result;
}

ErrorCode createStats(Stats** stats) {
    ErrorCode result = allocateStatsMemory(stats);
    if (result == OK) {
        initializeStats(*stats);
    }
}




//calcvalues stats

