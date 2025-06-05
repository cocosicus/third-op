#include "extraction.h"

#include <stdlib.h>  // For malloc and free
#include <string.h>  // For strcmp
#include <stdio.h>

typedef enum {
    FIRST_COLUMN = 1,
    SECOND_COLUMN = 2,
    THIRD_COLUMN = 3,
    FOURTH_COLUMN = 4,
    FIFTH_COLUMN = 5,
    SIXTH_COLUMN = 6,
    SEVENTH_COLUMN = 7
} Columns;


// Helpers
ErrorCode extractValue(const DemographyData* data, int column, double* value) {
    ErrorCode result = OK;
    if (!data || !value) {
        result = INVALID_DATA_ERROR;
    } else {
        switch (column) {
        case FIRST_COLUMN:
            *value = (double)data->year;
            break;
        case THIRD_COLUMN:
            *value = data->natural_population_growth;
            break;
        case FOURTH_COLUMN:
            *value = data->birth_rate;
            break;
        case FIFTH_COLUMN:
            *value = data->death_rate;
            break;
        case SIXTH_COLUMN:
            *value = data->general_demographic_weight;
            break;
        case SEVENTH_COLUMN:
            *value = data->urbanization;
            break;
        default:
            result = COLUMN_OUT_OF_RANGE_ERROR;
        }
    }
    return result;
}

ErrorCode resizeArray(void** values, size_t newSize, size_t elemSize) {
    ErrorCode result = OK;
    if (!values || !newSize) {
        result = INVALID_DATA_ERROR;
    } else {
        newSize *= 2;
        void* newValues = (void*)realloc(*values, newSize * elemSize);
        if (!newValues) {
            result = MEMORY_ALLOCATION_ERROR;
        } else {
            *values = newValues;
        }
    }
    return result;
}

ErrorCode processDataInLoop(Iterator* iter, const char* region, int column, Stats* stats, size_t* allocatedSize) {
    ErrorCode result = OK;
    DemographyData* data = NULL;
    if(!iter || !region || !stats || !allocatedSize){
        result = INVALID_DATA_ERROR;
    } else {
        resetIterator(iter);

        while ((data = next(iter)) != NULL && result == OK) {
            if (data->valid && strcmp(data->region, region) == 0) {
                double value = 0.0;
                ErrorCode extractResult = extractValue(data, column, &value);

                if (extractResult == OK) {
                    if (stats->valuesCount >= *allocatedSize) {
                        ErrorCode resizeResult = resizeArray(&stats->values, *allocatedSize * 2, sizeof(double));
                        resizeResult = resizeResult == OK ? resizeArray(&stats->years, *allocatedSize * 2, sizeof(int)) : resizeResult;
                        if (resizeResult != OK) {
                            result = resizeResult;
                        }
                        else { *allocatedSize *= 2; }
                    }

                    if (result == OK) {
                        stats->values[stats->valuesCount] = value;
                        stats->years[stats->valuesCount] = data->year;
                        stats->valuesCount++;
                    }

                } else {
                    result = extractResult;
                }
            }
        }
    }
    return result;
}

ErrorCode extractData(const LinkedList* list, const char* region, int column, Stats* stats) {
    ErrorCode result = (!list || !region || !stats || (column < 1) || (column > 7)) ? INVALID_DATA_ERROR : OK;

    if (result == OK) {
        Iterator* iter;
        size_t allocatedSize = 10;
        if (stats->values != NULL) free(stats->values);
        if (stats->years != NULL) free(stats->years);
        stats->valuesCount = 0;

        createIterator(&iter, list);

        if (!iter) {
            result = MEMORY_ALLOCATION_ERROR;
        } else {
            stats->values = (double*)malloc(allocatedSize * sizeof(double)); //
            stats->years = (int*)malloc(allocatedSize * sizeof(int)); //
            if (stats->values == NULL || stats->years == NULL) {
                result = MEMORY_ALLOCATION_ERROR;
                if (stats->values != NULL) {
                    free(stats->values);
                    stats->values = NULL;
                }
                if (stats->years != NULL) {
                    free(stats->years);
                    stats->years = NULL;
                }
            }
            else {
                result = processDataInLoop(iter, region, column, stats, &allocatedSize);
            }
            freeIterator(iter);
        }
    }
    return result;
}
