/**
 * @author: Quentin RODIC <quentin.rodic.pro@outlook.fr>
 * @date:   2022-01-28 18:25:38
 * @lastModifiedBy:   Quentin RODIC <quentin.rodic.pro@outlook.fr>
 * @lastModifiedTime: 2022-01-29 19:49:02
 */

#ifndef IHC_ARGUMENT_RULE_HEADER
#define IHC_ARGUMENT_RULE_HEADER

#include "../../../utils/string.array.h"

struct argument_rule
{
    char *label;
    char *label_shortcut;
    string_array_t correct_values;
};

typedef struct argument_rule argument_rule_t;

struct argument_rule_array
{
    unsigned int cursor;
    unsigned int size;
    argument_rule_t *values;
};

typedef struct argument_rule_array argument_rule_array_t;

/**
 * @brief 
 * 
 * Pass in argument the label and the shorcut.
 * Then returns the created argument rule.
 * 
 * label is required but shortcut is optional.
 * 
 * @param label 
 * @param label_shortcut 
 * @return argument_rule_t 
 */
argument_rule_t argument_rule_init(const char *label, const char *label_shortcut);

/**
 * @brief
 * 
 * Pass the argument rule into parameters then the value to add.
 * 
 * @param argument_rule 
 * @param value 
 * @return int 
 */
int argument_rule_add_rule(argument_rule_t *argument_rule, const char *value);

short int argument_rule_test_label_and_shortcut(argument_rule_t *argument_rule, char *label);
short int argument_rule_test_correct_values(argument_rule_t *argument_rule, char *value);

/**
 * @brief 
 * 
 * Pass in argument the initial size of argument rule array
 * 
 * @param initial_size
 * @return argument_rule_array_t 
 */
argument_rule_array_t argument_rule_array_init(const int initial_size);

/**
 * @brief
 * 
 * Pass the argument rule array into parameters then the argument rule to add.
 * 
 * @param argument_rule_array
 * @param argument_rule
 * @param value 
 * @return int 
 */
int argument_rule_array_add(argument_rule_array_t *argument_rule_array, argument_rule_t *argument_rule);

#endif // IHC_ARGUMENT_RULE_HEADER