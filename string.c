#include "string.h"

String* StringFromLength(long int length)
{
    String* newString = (String*)malloc(sizeof(String));
    newString->value = (char*)malloc(sizeof(char) * 0);
    newString->length = 0;
    newString->size = length;
}

String* StringFromValue(const char * str)
{
    long int length = strlen(str);
    String* newString = (String*)malloc(sizeof(String));
    newString->value = (char*)malloc(sizeof(char) * length);
    strcpy(newString->value, str);
    newString->length = length;
    newString->size = length;
}

void StringFree(String * str)
{
    free(str->value);
    free(str);
}

void StringAppend(String * str, const char * value)
{
    long int valueLength = strlen(value);
    long int currentLength = strlen(str->value);

    if (currentLength + valueLength >= str->size) {
        str->size = currentLength + valueLength;
        str->value = (char*)realloc(str->value, sizeof(char) * (currentLength + valueLength));
    }

    strcat(str->value, value);
}

void StringCopy(String * str, const char * value)
{
    long int valueLength = strlen(value);
    long int currentLength = strlen(str->value);

    if (str->size <= valueLength) {
        str->size = valueLength;
        str->value = (char*)realloc(str->value, sizeof(char) * valueLength);
    }

    strcpy(str->value, value);
}

void StringNCopy(String * str, const char * value, long int n)
{
    long int valueLength = n;
    long int currentLength = strlen(str->value);

    if (str->size <= valueLength) {
        str->size = valueLength;
        str->value = (char*)realloc(str->value, sizeof(char) * valueLength);
    }

    strncpy(str->value, value, n);    
}

char* StringGetValue(String * str)
{
    return str->value;
}
