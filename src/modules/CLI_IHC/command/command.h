/**
 * @author: Quentin RODIC <quentin.rodic.pro@outlook.fr>
 * @date:   2022-01-30 14:56:19
 * @lastModifiedBy:   Quentin RODIC <quentin.rodic.pro@outlook.fr>
 * @lastModifiedTime: 2022-02-01 23:24:44
 */

#ifndef IHC_COMMAND_HEADER
#define IHC_COMMAND_HEADER

#include "../argument_rule/argument.rule.h"

struct command
{
    char *label;
    short int hasValue;
    argument_rule_array_t arguments_rules;
};

typedef struct command command_t;

struct command_array
{
    int size;
    int cursor;
    command_t *values;
};

typedef struct command_array command_array_t;

command_t command_init(const char *label, short int hasValue, int initial_size);

command_t *command_find_among_array(command_t *commands, int size, const char *command);

command_array_t command_array_init(int initial_size);

int command_array_add(command_array_t *commands, command_t *command);

#endif // IHC_COMMAND_HEADER