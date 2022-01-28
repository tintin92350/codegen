#include "string.h"

#include <string.h>
#include <stdlib.h>

void copy_string(const char *src, char **dst)
{
    int srcLength = strlen(src);
    *dst = (char *)malloc(sizeof(char) * srcLength);
    strcpy(*dst, src);
}

void split_string_in_two(char *src, char **dst1, char **dst2, const char *del)
{
    *dst1 = strtok(src, del);
    *dst2 = strtok(NULL, del);
}