/**
 * @author: Quentin ROIC <quentin.rodic.pro@outlook.fr>
 * @date: 27-01-2022 9:11:21 am
 * @lastModifiedBy: Quentin ROSIC <quentin.rodic.pro@outlook.fr>
 * @lastModifiedTime: 2022-01-30 15:12:31
 */

#ifndef CLI_IHC
#define CLI_IHC

#include "../../utils/string.array.h"
#include "argument_rule/argument.rule.h"
#include "command/command.h"

/**
 * @brief STEP 01 of argument processing
 * 
 * Reduce the argument into array by spliting attached argument (LABEL=CONTENT)
 * 
 * @param count 
 * @param arguments 
 * @return string_array_t 
 */
string_array_t reduce_attached_arguments(int count, char **arguments);

/**
 * @brief STEP 02 of argument processing
 * 
 * Check the syntax of arguments based on argument's position, etc.
 * 
 * @param arguments 
 * @return short int 
 */
short int check_arguments_syntax(string_array_t *arguments);

/**
 * @brief STEP 03 of argument processing
 * 
 * Check the business syntax of arguments based on argument label and value.
 * 
 * @param command
 * @param arguments 
 * @return short int 
 */
short int check_business_error(command_t *command, string_array_t *arguments);

/**
 * @brief Check if an argument is formed with the form of --LABEL=CONTENT
 * 
 * @param argument Argument to check (string)
 * @return short int 1 is true, 0 is false OR exit
 */
short int check_if_argument_is_attached(char *argument);

/**
 * @brief 
 * 
 * Pass argc and argv then returns the flushed string array
 * 
 * @param count 
 * @param arguments 
 * @return char** 
 */
char **flush_argument_string_array(int count, char **arguments);

#endif // CLI_IHC