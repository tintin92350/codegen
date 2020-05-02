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

char * streral(const char * string, char character)
{
    int charOccurrence = 0;
    for (charOccurrence=0; string[charOccurrence]; string[charOccurrence]==character ? charOccurrence++ : *string++);

    char * nString = strinit(strlen(string) - charOccurrence);

    int write = 0;
    int i = 0;

    for (i = 0; i < strlen(string); i++) {
        if (string[i] == character) {
            continue;
        }
        else {
            nString[write] = string[i];
            write++;
        }
    }

    return nString;
}

int strrepc(char *string, char character, char replace)
{
    for (int i = 0; i < strlen(string); i++) {
        if (string[i] == character) {
            string[i] = replace;
        }
    }
}