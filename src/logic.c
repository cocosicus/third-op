#include "logic.h"
#include "calculations.h"
#include "csvProcessing.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to load data from CSV file
void loadData(Context* context, const char* filename) {
    context->errorCode = OK;
    ErrorCode result = OK;

    if (!context || !filename) {
        result = INVALID_DATA_ERROR;
    } else {
        clearLinkedList(context->dataList); // Clear
        context->totalRows = 0;
        context->errorRows = 0;
        context->validRows = 0;

        result = readDemographyData(filename, context->dataList, &context->totalRows, &context->validRows, &context->errorRows);

        if (result != OK) {
            context->errorCode = result;
            switch (result) {
            case FILE_OPEN_ERROR:
                strcpy(context->errorMessage, "Error opening file.");
                break;
            case MEMORY_ALLOCATION_ERROR:
                strcpy(context->errorMessage, "Memory allocation error.");
                break;
            default:
                strcpy(context->errorMessage, "Error reading data from file.");
                break;
            }
        } else {
            context->errorCode = OK;
            context->errorMessage[0] = '\0';
        }
    }

    context->errorCode = result;
}

// Function to calculate statistics
void calculateStats(Context* context, const char* region, int column) {
    context->errorCode = OK;
    ErrorCode result = OK;

    if (!context || !region) {
        result = INVALID_DATA_ERROR;
    } else {
        result = calculateStatistics(context->dataList, region, column, context->stats, context->calcValues);

        if (result != OK) {
            context->errorCode = result;
            switch (result) {
            case EMPTY_LIST_ERROR:
                strcpy(context->errorMessage, "No data found for the specified region.");
                break;
            case COLUMN_OUT_OF_RANGE_ERROR:
                strcpy(context->errorMessage, "Column number is out of range.");
                break;
            case INVALID_DATA_ERROR:
                strcpy(context->errorMessage, "Invalid input data.");
                break;
            default:
                strcpy(context->errorMessage, "Error calculating statistics.");
                break;
            }
        } else {
            context->errorCode = OK;
            context->errorMessage[0] = '\0';
        }
    }
    context->errorCode = result;
}
