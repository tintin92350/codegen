/**
 * @author: Quentin RODIC <quentin.rodic.pro@outlook.fr>
 * @date:   2022-01-26 22:42:59
 * @lastModifiedBy:   Quentin RODIC <quentin.rodic.pro@outlook.fr>
 * @lastModifiedTime: 2022-01-28 11:35:24
 */

// Standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Cli
#include "cli/cli.h"

#include "modules/CLI_IHC/ihc.h"

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

    printCodegenLogoAndVersion();

    //configureCliFromProgramArgument(argc, argv);

    string_array_t *arguments = reduceAttachedArguments(argc, argv);

    for (int i = 0; i < arguments->size; i++)
    {
        printf("value : %s\n", arguments->values[i]);
    }

    printf("is well formed : %d\n", checkArgumentChain(arguments));

    return EXIT_SUCCESS;
}