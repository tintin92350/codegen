/**
 * @author: Quentin RODIC <quentin.rodic.pro@outlook.fr>
 * @date:   2022-01-28 18:25:38
 * @lastModifiedBy:   Quentin RODIC <quentin.rodic.pro@outlook.fr>
 * @lastModifiedTime: 2022-01-28 18:28:35
 */

#ifndef IHC_ARGUMENT_RULE_HEADER
#define IHC_ARGUMENT_RULE_HEADER

struct argument_rule
{
    char *label;
    char *labelShortcut;
    struct string_array *correctValues;
};

typedef struct argument_rule argument_rule_t;

#endif // IHC_ARGUMENT_RULE_HEADER