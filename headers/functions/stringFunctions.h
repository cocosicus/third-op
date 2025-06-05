#ifndef STRINGFUNCTIONS_H
#define STRINGFUNCTIONS_H

#include <stddef.h>  // size_t
#include <stdbool.h> // bool

#include "ErrorCode.h"

ErrorCode strDuplicate(const char* source, char** destination);
//ErrorCode trimLeadingWhitespace(char* str);
//ErrorCode trimTrailingWhitespace(char* str);
ErrorCode trimWhitespace(char* str);

#endif // STRINGFUNCTIONS_H
