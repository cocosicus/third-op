#include "calcValues.h"
#include <stdlib.h>

#define DEFAULT_CALC_VALUE 0

ErrorCode allocateCalcValuesMemory(CalcValues** calcValues) {
    ErrorCode result = calcValues == NULL ? INVALID_DATA_ERROR : OK;
    if (result == OK) {
        *calcValues = (CalcValues*)malloc(sizeof(CalcValues));
        if (*calcValues == NULL) {
            result = MEMORY_ALLOCATION_ERROR;
        }
    }
    return result;
}


ErrorCode initializeCalcValues(CalcValues* calcValues) {
    ErrorCode result = calcValues == NULL ? INVALID_DATA_ERROR : OK;
    if (result == OK) {
        calcValues->max = DEFAULT_CALC_VALUE;
        calcValues->min = DEFAULT_CALC_VALUE;
        calcValues->median = DEFAULT_CALC_VALUE;
    }
    return result;
}


void freeCalcValues(CalcValues* calcValues) {
    if (calcValues != NULL) {
        free(calcValues);
        calcValues = NULL;
    }
}

ErrorCode createCalcValues(CalcValues** calcValues) {
    ErrorCode result = allocateCalcValuesMemory(calcValues);
    if (result == OK) {
        result = initializeCalcValues(*calcValues);
    }
    return result;
}


