#include "command.h"

#include <string.h>
#include <stdlib.h>

#include "../../../utils/string.h"

command_t command_init(const char *label, int initial_size)
{
    command_t command;

    copy_string(label, &command.label);

    command.arguments_rules = argument_rule_array_init(initial_size);

    return command;
}

command_t *command_find_among_array(command_t *commands, int size, const char *command)
{
    for (int i = 0; i < size; i++)
    {
        if (strcmp(commands[i].label, command) == 0)
        {
            return &commands[i];
        }
    }

    return NULL;
}

command_array_t command_array_init(int initial_size) {
    
    command_array_t array;

    array.size = initial_size;
    array.values = (command_t *)malloc(sizeof(command_t) * array.size);
    array.cursor = 0;

    return array;
}

int command_array_add(command_array_t *commands, command_t *command) {
    
    if (commands == NULL || command == NULL)
        return 0;

    if (commands->cursor == commands->size)
    {
        command_t *buffer = (command_t *)realloc(commands->values, sizeof(command_t) * (commands->size + 1));

        if (buffer == NULL)
        {
            free(buffer);
            return 0;
        }

        commands->size++;
        commands->values = buffer;
    }

    commands->values[commands->cursor++] = *command;

    return 1;
}