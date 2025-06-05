#ifndef STATS_H
#define STATS_H

#include <stddef.h> //size_t
#include "ErrorCode.h"

typedef struct {
    int* years;
    double* values;
    size_t valuesCount;
} Stats;

ErrorCode allocateStatsMemory(Stats** stats);

ErrorCode initializeStats(Stats* stats);

void freeStats(Stats* stats);

ErrorCode createStats(Stats** stats);

#endif // STATS_H
