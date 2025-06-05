#ifndef CALCVALUES_H
#define CALCVALUES_H

#include <stddef.h> //size_t
#include "ErrorCode.h"

typedef struct {
    double min, max, median;
} CalcValues;


ErrorCode allocateCalcValuesMemory(CalcValues** calcValues);

ErrorCode initializeCalcValues(CalcValues* calcValues);

void freeCalcValues(CalcValues* calcValues);

ErrorCode createCalcValues(CalcValues** calcValues);

#endif // CALCVALUES_H
