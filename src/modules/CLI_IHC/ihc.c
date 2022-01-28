#include "ihc.h"

#include <regex.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

string_array_t reduceAttachedArguments(int count, char **argumentsSource)
{
    if (count == 0)
        return string_array_empty();

    char **arguments = removedExeArgumentArray(count, argumentsSource);
    count--;

    string_array_t reducedArgumentArray = string_array_init(count);

    for (int i = 0; i < count; i++)
    {
        char *argument = arguments[i];

        if (checkIfArgumentIsAttached(argument))
            reducedArgumentArray.size++;
    }

    reducedArgumentArray.values = (char **)calloc(sizeof(char *), reducedArgumentArray.size);

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

short int checkArgumentChain(string_array_t *arguments)
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

argument_rule_t createArgumentRule(const char *label, const char *labelShortcut)
{
    argument_rule_t argument_rule;

    argument_rule.label = NULL;
    argument_rule.labelShortcut = NULL;
    argument_rule.correctValues = string_array_empty();

    if (label == NULL)
        return argument_rule;

    int labelLength = strlen(label);
    argument_rule.label = (char *)malloc(sizeof(char) * labelLength);
    strcpy(argument_rule.label, label);

    if (labelShortcut != NULL)
    {
        int labelShortcutLength = strlen(labelShortcut);
        argument_rule.labelShortcut = (char *)malloc(sizeof(char) * labelShortcutLength);
        strcpy(argument_rule.labelShortcut, labelShortcut);
    }

    return argument_rule;
}

int addValueToArgumentRule(argument_rule_t *argument_rule, const char *value)
{
    if (argument_rule == NULL)
        return 0;

    if (string_array_is_empty(&argument_rule->correctValues))
        argument_rule->correctValues = string_array_init(1);
    else
        string_array_resize(&argument_rule->correctValues, argument_rule->correctValues.size + 1);

    argument_rule->correctValues.values[argument_rule->correctValues.size - 1] = (char *)malloc(sizeof(char) * strlen(value));
    strcpy(argument_rule->correctValues.values[argument_rule->correctValues.size - 1], value);

    return 1;
}