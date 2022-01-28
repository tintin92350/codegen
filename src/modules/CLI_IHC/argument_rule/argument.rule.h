/**
 * @author: Quentin RODIC <quentin.rodic.pro@outlook.fr>
 * @date:   2022-01-28 18:25:38
 * @lastModifiedBy:   Quentin RODIC <quentin.rodic.pro@outlook.fr>
 * @lastModifiedTime: 2022-01-28 19:37:27
 */

#ifndef IHC_ARGUMENT_RULE_HEADER
#define IHC_ARGUMENT_RULE_HEADER

#include "../../../utils/string.array.h"

struct argument_rule
{
    char *label;
    char *labelShortcut;
    string_array_t correctValues;
};

typedef struct argument_rule argument_rule_t;

/**
 * @brief 
 * 
 * Pass in argument the label and the shorcut.
 * Then returns the created argument rule.
 * 
 * label is required but shortcut is optional.
 * 
 * @param label 
 * @param labelShortcut 
 * @return argument_rule_t 
 */
argument_rule_t argument_rule_init(const char *label, const char *labelShortcut);

int argument_rule_add_rule(argument_rule_t *argumentRule, const char *value);

#endif // IHC_ARGUMENT_RULE_HEADER