#include "argument.rule.h"

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>

#include "../../../utils/string.h"

argument_rule_t argument_rule_init(const char *label, const char *label_shortcut)
{
    argument_rule_t argument_rule;

    argument_rule.label = NULL;
    argument_rule.label_shortcut = NULL;
    argument_rule.correct_values = string_array_empty();

    if (label == NULL)
        return argument_rule;

    copy_string(label, &argument_rule.label);

    if (label_shortcut != NULL)
        copy_string(label_shortcut, &argument_rule.label_shortcut);

    return argument_rule;
}

int argument_rule_add_rule(argument_rule_t *argument_rule, const char *value)
{
    if (argument_rule == NULL)
        return 0;

    if (string_array_is_empty(&argument_rule->correct_values))
        argument_rule->correct_values = string_array_init(1);
    else
        string_array_resize(&argument_rule->correct_values, argument_rule->correct_values.size + 1);

    copy_string(value, &argument_rule->correct_values.values[argument_rule->correct_values.size - 1]);

    return 1;
}

short int argument_rule_test_label_and_shortcut(argument_rule_t *argument_rule, char *label)
{
    return strcmp(argument_rule->label, label) == 0 || (argument_rule->label_shortcut != NULL && strcmp(argument_rule->label_shortcut, label) == 0);
}

short int argument_rule_test_correct_values(argument_rule_t *argument_rule, char *value)
{
    if (value == NULL)
        return string_array_is_empty(&argument_rule->correct_values);

    for (int i = 0; i < argument_rule->correct_values.size; i++)
    {
        if (strcmp(argument_rule->correct_values.values[i], value) == 0)
            return 1;
    }

    return 0;
}

argument_rule_array_t argument_rule_array_init(const int initial_size)
{
    argument_rule_array_t array;

    array.size = initial_size;
    array.values = (argument_rule_t *)malloc(sizeof(argument_rule_t) * array.size);
    array.cursor = 0;

    return array;
}

void print_arguments_rules(argument_rule_t *argument_rule)
{
    for (int i = 0; i < argument_rule->correct_values.size; i++)
    {
        printf("-%s\n", argument_rule->correct_values.values[i]);
    }
}

int argument_rule_array_add(argument_rule_array_t *argument_rule_array, argument_rule_t *argument_rule)
{
    if (argument_rule_array == NULL || argument_rule == NULL)
        return 0;

    if (argument_rule_array->cursor == argument_rule_array->size)
    {
        argument_rule_t *buffer = (argument_rule_t *)realloc(argument_rule_array->values, sizeof(argument_rule_t) * (argument_rule_array->size + 1));

        if (buffer == NULL)
        {
            free(buffer);
            return 0;
        }

        argument_rule_array->size++;
        argument_rule_array->values = buffer;
    }

    argument_rule_array->values[argument_rule_array->cursor++] = *argument_rule;

    return 1;
}