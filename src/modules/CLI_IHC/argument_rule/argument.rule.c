#include "argument.rule.h"

#include <stddef.h>

#include "../../../utils/string.h"

argument_rule_t argument_rule_init(const char *label, const char *labelShortcut)
{
    argument_rule_t argument_rule;

    argument_rule.label = NULL;
    argument_rule.labelShortcut = NULL;
    argument_rule.correctValues = string_array_empty();

    if (label == NULL)
        return argument_rule;

    copy_string(label, &argument_rule.label);

    if (labelShortcut != NULL)
        copy_string(labelShortcut, &argument_rule.labelShortcut);

    return argument_rule;
}

int argument_rule_add_rule(argument_rule_t *argument_rule, const char *value)
{
    if (argument_rule == NULL)
        return 0;

    if (string_array_is_empty(&argument_rule->correctValues))
        argument_rule->correctValues = string_array_init(1);
    else
        string_array_resize(&argument_rule->correctValues, argument_rule->correctValues.size + 1);

    copy_string(value, &argument_rule->correctValues.values[argument_rule->correctValues.size - 1]);

    return 1;
}