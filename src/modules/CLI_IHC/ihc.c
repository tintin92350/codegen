#include "ihc.h"

#include <regex.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

string_array_t *reduceAttachedArguments(int count, char **argumentsSource)
{
    if (count == 0)
        return 0;

    char **arguments = removedExeArgumentArray(count, argumentsSource);
    count--;

    string_array_t *reducedArgumentArray = (string_array_t *)malloc(sizeof(string_array_t));
    reducedArgumentArray->size = count;

    for (int i = 0; i < count; i++)
    {
        char *argument = arguments[i];

        if (checkIfArgumentIsAttached(argument))
            reducedArgumentArray->size++;
    }

    reducedArgumentArray->values = (char **)calloc(sizeof(char *), reducedArgumentArray->size);

    int i = 0;
    int extraCursor = 0;
    while (i < count)
    {
        if (checkIfArgumentIsAttached(arguments[i]))
        {
            char *str1 = NULL, *str2 = NULL;
            splitAttachedArgumentInTwoString(arguments[i], &str1, &str2);

            if (str1 != NULL && str2 != NULL)
            {
                int len = strlen(str1);
                reducedArgumentArray->values[i + extraCursor] = (char *)malloc(sizeof(char) * len);
                strcpy(reducedArgumentArray->values[i + extraCursor], str1);

                extraCursor++;

                len = strlen(str2);
                reducedArgumentArray->values[i + extraCursor] = (char *)malloc(sizeof(char) * len);
                strcpy(reducedArgumentArray->values[i + extraCursor], str2);
            }
        }
        else
        {
            int len = strlen(arguments[i]);
            reducedArgumentArray->values[i + extraCursor] = (char *)malloc(sizeof(char) * len);
            strcpy(reducedArgumentArray->values[i + extraCursor], arguments[i]);
        }

        i++;
    }

    return reducedArgumentArray;
}

short int checkIfArgumentIsAttached(char *argument)
{
    regex_t regex;
    int reti;
    char msgbuf[100];

    /* Compile regular expression */
    reti = regcomp(&regex, "--[A-Za-z0-9]*=[A-Za-z0-9]*", 0);

    if (reti)
    {
        fprintf(stderr, "Could not compile regex\n");
        exit(1);
    }

    /* Execute regular expression */
    reti = regexec(&regex, argument, 0, NULL, 0);

    if (!reti)
    {
        /* Free memory allocated to the pattern buffer by regcomp() */
        regfree(&regex);
        return 1;
    }
    else if (reti == REG_NOMATCH)
    {
        /* Free memory allocated to the pattern buffer by regcomp() */
        regfree(&regex);
        return 0;
    }
    else
    {
        /* Free memory allocated to the pattern buffer by regcomp() */
        regfree(&regex);
        regerror(reti, &regex, msgbuf, sizeof(msgbuf));
        fprintf(stderr, "Regex match failed: %s\n", msgbuf);

        return -1;
    }
}

void splitAttachedArgumentInTwoString(char *src, char **dst1, char **dst2)
{
    *dst1 = strtok(src, "=");

    *dst2 = strtok(NULL, "=");
}

char **removedExeArgumentArray(int count, char **arguments)
{
    char **array = (char **)malloc(sizeof(char *) * (count - 1));

    for (int i = 1; i < count; i++)
        array[i - 1] = arguments[i];

    return array;
}