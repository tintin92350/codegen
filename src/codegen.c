/**
 * @author: Quentin RODIC <quentin.rodic.pro@outlook.fr>
 * @date:   2022-01-26 22:42:59
 * @lastModifiedBy:   Quentin RODIC <quentin.rodic.pro@outlook.fr>
 * @lastModifiedTime: 2022-01-30 15:25:49
 */

// Standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Cli
#include "cli/cli.h"

#include "modules/CLI_IHC/ihc.h"
#include "modules/CLI_IHC/argument_rule/argument.rule.h"
#include "modules/CLI_IHC/command/command.h"

/**
 * @brief Main entry of the program
 * 
 * @param argc Number of entry argument
 * @param argv Arguments
 * @return int Error code
 */
int main(int argc, char **argv)
{

    // Quit the program if we do not have enough parameters set by user
    if (argc < 2)
    {
        printf("\n\033[0;31m/!\\ This program take at least one parameter (enter help to have helpfull commands)\033[0m\n\n");
        return EXIT_FAILURE;
    }

    //configureCliFromProgramArgument(argc, argv);

    string_array_t arguments = reduce_attached_arguments(argc, argv);

    if (!check_arguments_syntax(&arguments))
    {
        printf("\n\033[0;31m/!\\ The syntax is incorrect ! Please use help command to see which syntax is right\033[0m\n\n");
        return EXIT_FAILURE;
    }

    command_array_t program_arguments = configure_cli_commands();

    command_t *found_command = command_find_among_array(program_arguments.values, program_arguments.cursor, argv[1]);

    if (found_command == NULL)
    {
        printf("\n\033[0;31m/!\\ Unknown command\033[0m\n\n");
        return EXIT_FAILURE;
    }

    if (!check_business_error(found_command, &arguments))
    {
        printf("\n\033[0;31m/!\\ The business syntax is incorrect ! Please use help command to see which syntax is right\033[0m\n\n");
        return EXIT_FAILURE;
    }

    printCodegenLogoAndVersion();

    return EXIT_SUCCESS;
}