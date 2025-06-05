#ifndef ERRORCODE_H
#define ERRORCODE_H

// Error codes
typedef enum {
    OK,
    FILE_OPEN_ERROR,
    MEMORY_ALLOCATION_ERROR,
    INVALID_DATA_ERROR,
    EMPTY_LIST_ERROR,
    COLUMN_OUT_OF_RANGE_ERROR
} ErrorCode;

#endif // ERRORCODE_H
