#ifndef CALCULATIONS_H
#define CALCULATIONS_H

#include "linkedList.h"   // LinkedList
#include "calcValues.h"
#include "stats.h"
#include <stddef.h>      // size_t

// Function prototypes
ErrorCode calculateMinMax(double* values, size_t valuesCount, double* min, double* max);
ErrorCode calculateMedian(double* values, size_t valuesCount, double* median);
void insertionSort(double arr[], int n); // Internal helper
ErrorCode calculateStatistics(const LinkedList* list, const char* region, int column, Stats* stats, CalcValues* calcValues);

#endif
