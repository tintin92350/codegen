#include "ihc.h"

#include <regex.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../utils/string.h"

string_array_t reduce_attached_arguments(int count, char **argumentsSource)
{
    if (count == 0)
        return string_array_empty();

    char **arguments = flush_argument_string_array(count, argumentsSource);
    count--;

    string_array_t reducedArgumentArray = string_array_init(count);

    for (int i = 0; i < count; i++)
    {
        char *argument = arguments[i];

        if (check_if_argument_is_attached(argument))
            reducedArgumentArray.size++;
    }

    reducedArgumentArray.values = (char **)calloc(sizeof(char *), reducedArgumentArray.size);

    int i = 0;
    int extraCursor = 0;
    while (i < count)
    {
        if (check_if_argument_is_attached(arguments[i]))
        {
            char *str1 = NULL, *str2 = NULL;
            split_string_in_two(arguments[i], &str1, &str2, "=");

            if (str1 != NULL && str2 != NULL)
            {
                int len = strlen(str1);
                reducedArgumentArray.values[i + extraCursor] = (char *)malloc(sizeof(char) * len);
                strcpy(reducedArgumentArray.values[i + extraCursor], str1);

                extraCursor++;

                len = strlen(str2);
                reducedArgumentArray.values[i + extraCursor] = (char *)malloc(sizeof(char) * len);
                strcpy(reducedArgumentArray.values[i + extraCursor], str2);
            }
        }
        else
        {
            int len = strlen(arguments[i]);
            reducedArgumentArray.values[i + extraCursor] = (char *)malloc(sizeof(char) * len);
            strcpy(reducedArgumentArray.values[i + extraCursor], arguments[i]);
        }

        i++;
    }

    return reducedArgumentArray;
}

short int check_arguments_syntax(string_array_t *arguments)
{
    int lastCheckingType = -1; // Checking for argument label

    for (int i = 0; i < arguments->size; i++)
    {
        char argumentFirstLetter = arguments->values[i][0];

        if (argumentFirstLetter != '-' && lastCheckingType != 0)
        {
            return 0;
        }

        if (argumentFirstLetter == '-')
            lastCheckingType = 0;
        else
            lastCheckingType = 1;
    }

    return 1;
}

short int check_if_argument_is_attached(char *argument)
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

/**
 * @brief 
 * 
 * Remove the executable from the argument array
 * 
 * @param count 
 * @param arguments 
 * @return char** 
 */
char **flush_argument_string_array(int count, char **arguments)
{
    char **array = (char **)malloc(sizeof(char *) * (count - 1));

    for (int i = 1; i < count; i++)
        array[i - 1] = arguments[i];

    return array;
}