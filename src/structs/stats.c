#include "stats.h"
#include <stdlib.h>

#define DEFAULT_STATS_VALUE 0

ErrorCode allocateStatsMemory(Stats** stats) {
    ErrorCode result = stats == NULL ? INVALID_DATA_ERROR : OK;
    if (result == OK) {
        *stats = (Stats*)malloc(sizeof(Stats));
        if (*stats == NULL) {
            result = MEMORY_ALLOCATION_ERROR;
        }
    }
    return result;
}

ErrorCode initializeStats(Stats* stats) {
    ErrorCode result = stats == NULL ? INVALID_DATA_ERROR : OK;
    if (result == OK) {
        stats->values = NULL;
        stats->years = NULL;
        stats->valuesCount = DEFAULT_STATS_VALUE;
    }
    return result;
}

void freeStats(Stats* stats) {
    if (stats != NULL) {
        if (stats->values != NULL) {
            free (stats->values);
            stats->values = NULL;
        }
        if (stats->years != NULL) {
            free (stats->years);
            stats->years = NULL;
        }
        free(stats);
        stats = NULL;
    }
}

ErrorCode createStats(Stats** stats) {
    ErrorCode result = allocateStatsMemory(stats);
    if (result == OK) {
        initializeStats(*stats);
    }
    return result;
}

