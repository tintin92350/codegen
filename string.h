#ifndef STRING_HEADER_FILE
#define STRING_HEADER_FILE

// Standard Library
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// Dynamic string structure
typedef struct String_t {
    char * value;
    long int length;
    long int size;
} String;

/**
 * Build a new empty string from given length
 * @param length Max length of the string
 */
String* StringFromLength(long int length);

/**
 * Build a new string from given string
 * @param str string value to copy
 */
String* StringFromValue(const char * str);

/**
 * Free a string struct
 * @param string String struct to freed
 */
void StringFree(String * str);

/**
 * Append string value to a string
 * @param string String struct where append
 * @param string String value to append
 */
void StringAppend(String * str, const char * value);

/**
 * Copy string value to a string
 * @param string String struct where append
 * @param string String value to append
 */
void StringCopy(String * str, const char * value);

/**
 * Copy string value to a string with range
 * @param string String struct where append
 * @param string String value to append
 * @param n Number of byte to copy
 */
void StringNCopy(String * str, const char * value, long int n);

/**
 * Return string value
 * @param string String struct
 */
char* StringGetValue(String * str);


#endif // CODEGEN_PROJECT_HEADER_FILE