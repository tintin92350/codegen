/**
 * @author: Quentin RODIC <quentin.rodic.pro@outlook.fr>
 * @date:   2022-01-26 23:03:11
 * @lastModifiedBy:   Quentin RODIC <quentin.rodic.pro@outlook.fr>
 * @lastModifiedTime: 2022-01-26 23:06:26
 */

#ifndef CONTEXT_HEADER
#define CONTEXT_HEADER

// Linux specific headers
#include <linux/limits.h>

struct context
{
    char workingDirectory[PATH_MAX];
    int argc;
    char** argv;
};

typedef struct context context_t;


#endif // CONTEXT_HEADER