#include "calculations.h"
#include "extraction.h"
#include <stdlib.h>   // free malloc
#include <stddef.h>     // size_t

ErrorCode calculateMinMax(double* values, size_t valuesCount, double* min, double* max) {
    ErrorCode result = OK;

    if (!values || !min || !max) {
        result = INVALID_DATA_ERROR;
    }
    else if (valuesCount == 0) {
        result = EMPTY_LIST_ERROR;
    }
    else {
        *min = values[0];
        *max = values[0];
        for (size_t i = 1; i < valuesCount; ++i) {
            if (values[i] < *min) *min = values[i];
            if (values[i] > *max) *max = values[i];
        }
    }
    return result;
}

double* copy(double *src, size_t valuesCount) {
    double* copySrc = (double*)malloc(valuesCount * sizeof(double));
    if (copySrc) {
        for(int i = 0; i < valuesCount; i++) {
            copySrc[i] = src[i];
        }
    }
    return copySrc;
}

void insertionSort(double arr[], int n) {
    int i, j;
    double key;
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

ErrorCode calculateMedian(double* values, size_t valuesCount, double* median) {
    ErrorCode result = OK;

    if (!values || !median) {
        result = INVALID_DATA_ERROR;
    } else if (valuesCount == 0) {
        result = EMPTY_LIST_ERROR;
    } else {
        double* copyValues = copy(values, valuesCount);
        if (copyValues != NULL) {
            insertionSort(copyValues, valuesCount);

            if (valuesCount % 2 == 0) {
                *median = (copyValues[valuesCount / 2 - 1] + copyValues[valuesCount / 2]) / 2.0;
            } else {
                *median = copyValues[valuesCount / 2];
            }
            free(copyValues);
        } else {
            result = MEMORY_ALLOCATION_ERROR;
        }
    }
    return result;
}

ErrorCode calculateStatisticsInternal(double* values, size_t valuesCount, CalcValues* calcValues) { //подфункция которая считает
    ErrorCode result = OK;
    if (!values) {
        result = INVALID_DATA_ERROR;
    }

    if (valuesCount == 0) {
        result = EMPTY_LIST_ERROR;
    }
    if (result == OK) {
        ErrorCode minMaxResult = calculateMinMax(values, valuesCount, &calcValues->min, &calcValues->max);

        if (minMaxResult == OK) {
            ErrorCode medianResult = calculateMedian(values, valuesCount, &calcValues->median);
            if (medianResult != OK) {
                result = medianResult;
            }
        } else {
            result = minMaxResult;
        }
    }
    return result;
}

ErrorCode calculateStatistics(const LinkedList* list, const char* region, int column, Stats* stats, CalcValues* calcValues) { //главная функция
    ErrorCode result = OK;
    if (stats->values) {
        free(stats->values);
        stats->values = NULL;
    }
    if (stats->years) {
        free(stats->years);
        stats->years = NULL;
    }
    stats->valuesCount = 0;

    result = extractData(list, region, column, stats);

    if (result == OK) {
        // Calculate min, max and median
        result = calculateStatisticsInternal(stats->values, stats->valuesCount, calcValues);
    }
    return result;
}
