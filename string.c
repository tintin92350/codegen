#include "string.h"

short int strequals(const char * str1, const char * str2) {
    return strcmp(str1, str2) == 0;
}

char * astrcpy(const char * src) {
    const int length = strlen(src);
    char * dst = (char*)malloc(sizeof(char) * length + 1);
    
    strcpy(dst, src);
    dst[length + 1] = '\0';

    return dst;
}

int rastrcpy(char * dst, const char * src)
{

}

char * strinit(int size)
{
    return (char*)malloc(sizeof(char) * size + 1);
}