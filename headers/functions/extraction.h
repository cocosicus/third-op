#ifndef EXTRACTION_H
#define EXTRACTION_H

#include "stats.h"
#include "linkedList.h"

//ErrorCode extractData(const LinkedList* list, const char* region, int column, double** values, int** years, size_t* valuesCount);

ErrorCode extractData(const LinkedList* list, const char* region, int column, Stats* stats);

#endif
