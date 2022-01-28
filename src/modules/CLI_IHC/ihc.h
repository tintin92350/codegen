/**
 * @author: Quentin ROIC <quentin.rodic.pro@outlook.fr>
 * @date: 27-01-2022 9:11:21 am
 * @lastModifiedBy: Quentin ROSIC <quentin.rodic.pro@outlook.fr>
 * @lastModifiedTime: 2022-01-28 19:11:13
 */

#ifndef CLI_IHC
#define CLI_IHC

#include "../../utils/string.array.h"

struct argument_rule
{
    char *label;
    char *labelShortcut;
    string_array_t correctValues;
};

typedef struct argument_rule argument_rule_t;

/**
 * @brief STEP 01 of argument processing
 * 
 * Reduce the argument into array by spliting attached argument (LABEL=CONTENT)
 * 
 * @param count 
 * @param arguments 
 * @return string_array_t 
 */
string_array_t reduceAttachedArguments(int count, char **arguments);

short int checkArgumentChain(string_array_t *arguments);

/**
 * @brief Check if an argument is formed with the form of --LABEL=CONTENT
 * 
 * @param argument Argument to check (string)
 * @return short int 1 is true, 0 is false OR exit
 */
short int checkIfArgumentIsAttached(char *argument);

void splitAttachedArgumentInTwoString(char *src, char **dst1, char **dst2);

char **removedExeArgumentArray(int count, char **arguments);

argument_rule_t createArgumentRule(const char *label, const char *labelShortcut);
int addValueToArgumentRule(argument_rule_t *argumentRule, const char *value);

#endif // CLI_IHC