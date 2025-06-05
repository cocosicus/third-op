#ifndef CSVPROCESS_H
#define CSVPROCESS_H

#include "demographyData.h"  //  data structures
#include "linkedList.h" //LinkedList
#include <stddef.h> // size_t
#include <stdbool.h> // bool

typedef enum {
    FIELD_TYPE_INT,
    FIELD_TYPE_DOUBLE,
    FIELD_TYPE_STRING
} FieldType;

// CSV reading
ErrorCode readDemographyData(const char* filename, LinkedList* dataList, int* totalLines, int* validLines, int* errorLines);

// lines
/*ErrorCode splitLineIntoTokens(const char* line, char* tokens[], int maxTokens, int* tokenCount);
ErrorCode processTokens(char* tokens[], int tokenCount, DemographyData* data);
ErrorCode parseToken(char* token, void* dest, FieldType type, DemographyData* data);
void initializeDemographyData(DemographyData* data);*/

#endif
