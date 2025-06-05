#include "csvProcessing.h"
#include "stringFunctions.h"
#include "linkedList.h" //LinkedList
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DELIMITER ","
#define MAX_LINE_LENGTH 1024  // Maximum line length for fgets
#define MAX_TOKENS 7 // Maximum number of tokens in a line

// helpers
ErrorCode processLine(const char* line, LinkedList* dataList, int* validLines, int* errorLines);
ErrorCode processTokens(char* tokens[], int tokenCount, DemographyData* data);
ErrorCode parseToken(char* token, void* dest, FieldType type, DemographyData* data);
void freeTokens(char* tokens[], int tokenCount);
ErrorCode splitLineIntoTokens(const char* line, char* tokens[], int maxTokens, int* tokenCount);
void initializeDemographyData(DemographyData* data);


ErrorCode readDemographyData(const char* filename, LinkedList* dataList, int* totalLines, int* validLines, int* errorLines) {
    ErrorCode result = OK;
    FILE* file = NULL;
    char buffer[MAX_LINE_LENGTH];

    if (filename == NULL || dataList == NULL || totalLines == NULL || validLines == NULL || errorLines == NULL) {
        result = INVALID_DATA_ERROR;
    } else {
        file = fopen(filename, "r");
        if (file == NULL) {
            result = FILE_OPEN_ERROR;
        } else {
            *totalLines = 0;
            *validLines = 0;
            *errorLines = 0;

            // Skip the header line
            if (fgets(buffer, MAX_LINE_LENGTH, file) != NULL) {
                (*totalLines)++;
            }

            while (fgets(buffer, MAX_LINE_LENGTH, file) != NULL) {
                (*totalLines)++;

                if (strlen(buffer) == MAX_LINE_LENGTH - 1 && buffer[MAX_LINE_LENGTH - 2] != '\n') {
                    (*errorLines)++;
                    result = INVALID_DATA_ERROR;
                } else {
                    result = processLine(buffer, dataList, validLines, errorLines);
                }
            }
            fclose(file);
        }
    }
    return result;
}

// Helpers
ErrorCode processLine(const char* line, LinkedList* dataList, int* validLines, int* errorLines) {
    ErrorCode result = OK;
    char* tokens[MAX_TOKENS];
    int tokenCount = 0;
    DemographyData data;

    result = splitLineIntoTokens(line, tokens, MAX_TOKENS, &tokenCount);

    if (result == OK) {
        initializeDemographyData(&data);
        result = processTokens(tokens, tokenCount, &data);

        if (result == OK) {
            ErrorCode insertResult = addNode(dataList, &data);
            if (insertResult == OK) {
                (*validLines)++;
            } else {
                (*errorLines)++;
                result = insertResult;
            }
        } else {
            (*errorLines)++;
        }
    } else {
        (*errorLines)++;
    }

    // Free the memory allocated for the tokens
    freeTokens(tokens, tokenCount);

    return result;
}

// Splits a line into tokens
ErrorCode splitLineIntoTokens(const char* line, char* tokens[], int maxTokens, int* tokenCount) {
    ErrorCode result = OK;
    char* lineCopy = NULL;
    char* token;
    char* rest = NULL;
    int i = 0;

    result = strDuplicate(line, &lineCopy);
    if (result != OK) {
        return result;
    }

    rest = lineCopy;
    token = strtok_r(rest, DELIMITER, &rest);

    while (token != NULL && i < maxTokens && result == OK) {
        tokens[i] = strdup(token);
        if (tokens[i] == NULL) {
            result = MEMORY_ALLOCATION_ERROR;
        } else {
            i++;
            token = strtok_r(rest, DELIMITER, &rest);
        }
    }

    *tokenCount = i;

    //If failed, free
    if(result != OK){
        freeTokens(tokens, i);
        *tokenCount = 0;
    }

    free(lineCopy);
    lineCopy = NULL;

    return result;
}

ErrorCode processTokens(char* tokens[], int tokenCount, DemographyData* data) {
    ErrorCode result = OK;

    if (tokens == NULL || data == NULL || tokenCount != MAX_TOKENS) {
        result = INVALID_DATA_ERROR;
    } else {
        // Process the tokens
        if (parseToken(tokens[0], &data->year, FIELD_TYPE_INT, data) == OK &&
            parseToken(tokens[1], &data->region, FIELD_TYPE_STRING, data) == OK &&
            parseToken(tokens[2], &data->natural_population_growth, FIELD_TYPE_DOUBLE, data) == OK &&
            parseToken(tokens[3], &data->birth_rate, FIELD_TYPE_DOUBLE, data) == OK &&
            parseToken(tokens[4], &data->death_rate, FIELD_TYPE_DOUBLE, data) == OK &&
            parseToken(tokens[5], &data->general_demographic_weight, FIELD_TYPE_DOUBLE, data) == OK &&
            parseToken(tokens[6], &data->urbanization, FIELD_TYPE_DOUBLE, data) == OK) {
            data->valid = 1;
        } else {
            result = INVALID_DATA_ERROR;
        }
    }

    return result;
}

ErrorCode parseToken(char* token, void* dest, FieldType type, DemographyData* data) {
    ErrorCode result = OK;
    if (token == NULL) {
        result = INVALID_DATA_ERROR;
    } else {
        switch (type) {
        case FIELD_TYPE_INT:
            if (sscanf(token, "%d", (int*)dest) != 1) {
                result = INVALID_DATA_ERROR;
            }
            break;
        case FIELD_TYPE_DOUBLE:
            if (sscanf(token, "%lf", (double*)dest) != 1) {
                result = INVALID_DATA_ERROR;
            }
            break;
        case FIELD_TYPE_STRING:
            trimWhitespace(token);
            strncpy(data->region, token, sizeof(data->region) - 1);
            data->region[sizeof(data->region)] = '\0';
            break;
        default:
            result = INVALID_DATA_ERROR;
        }
    }
    return result;
}

void freeTokens(char* tokens[], int tokenCount) {
    for (int i = 0; i < tokenCount; i++) {
        if (tokens[i] != NULL) {
            free(tokens[i]);
            tokens[i] = NULL;
        }
    }
}

void initializeDemographyData(DemographyData* data) {
    if (data != NULL) {
        data->year = 0;
        memset(data->region, 0, sizeof(data->region));
        data->natural_population_growth = 0.0;
        data->birth_rate = 0.0;
        data->death_rate = 0.0;
        data->general_demographic_weight = 0.0;
        data->urbanization = 0.0;
        data->valid = 0;
    }
}
