// CLI
#include "cli.h"

int main(int argc, char ** argv)
{
    if (argc < 2) {
        perror("\n\033[0;31m/!\\ This program take at least one parameter (enter help to have helpfull commands)\033[0m\n\n");
        return EXIT_FAILURE;
    }

    int code = handleCli(argc, argv);

    return code;
}