#include "stringFunctions.h"
#include <stdlib.h>  // malloc, free
#include <string.h>  // strlen, strncpy, memmove
#include <ctype.h>   // isspace

ErrorCode strDuplicate(const char* source, char** destination) {
    ErrorCode result = OK;
    size_t length = 0;
    char* dup = NULL;

    if (source == NULL || destination == NULL) {
        result = INVALID_DATA_ERROR;
    } else {
        length = strlen(source);
        dup = (char*)malloc((length + 1) * sizeof(char));

        if (dup == NULL) {
            result = MEMORY_ALLOCATION_ERROR;
        } else {
            strncpy(dup, source, length);
            dup[length] = '\0';
            *destination = dup;
        }
    }
    if (result != OK) {
        if (dup != NULL){
            free(dup); // Free
        }
    }
    return result;
}

ErrorCode trimLeadingWhitespace(char* str) {
    ErrorCode result = OK;
    char* current = NULL;

    if (str == NULL) {
        result = INVALID_DATA_ERROR;
    } else {
        current = str;
        while (isspace((unsigned char)*current)) {
            current++;
        }

        if (current != str) {
            memmove(str, current, strlen(current) + 1);
        }
    }
    return result;
}

ErrorCode trimTrailingWhitespace(char* str) {
    ErrorCode result = OK;
    char* end = NULL;

    if (str == NULL) {
        result = INVALID_DATA_ERROR;
    } else {
        end = str + strlen(str) - 1;
        while (end >= str && isspace((unsigned char)*end)) {
            end--;
        }
        *(end + 1) = '\0';
    }

    return result;
}

ErrorCode trimWhitespace(char* str) {
    ErrorCode result = trimLeadingWhitespace(str);
    if (result == OK) {
        result = trimTrailingWhitespace(str);
    }
    return result;
}
