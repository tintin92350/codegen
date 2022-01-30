#include "cli.h"

// Standard library
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>

// UNIX / Linux header
#include <sys/stat.h>
#include <sys/ioctl.h>

#include "../context/context.h"

context_t context;

void printCodegenLogoAndVersion()
{
    printf("\033[1;31m"
           "      ___________ __  ______    \n"
           "     / ____/ ___//  |/  /   |   \n"
           "    / /    \\__ \\/ /|_/ / /| |   \n"
           "   / /___ ___/ / /  / / ___ |   \n"
           "   \\____//____/_/  /_/_/  |_|   \33[0m\n\n"
           "   C Solution Management App    \n"
           "   Beta Version                 \n\n");
}

int configureCliFromProgramArgument(int argc, char **argv)
{
    // Retrieve working directory
    if (getcwd(context.workingDirectory, sizeof(context.workingDirectory)) == NULL)
    {
        perror("getcwd() error");
        return 0;
    }

    context.argc = argc;
    context.argv = argv;

    return 1;
}

command_array_t configure_cli_commands()
{
    command_array_t commands = command_array_init(1);

    // Add 'new' command

    command_t new_command = command_init("new", 2);

    command_array_add(&commands, &new_command);

    /*argument_rule_t argument_rule_generate = argument_rule_init("generate", "g");

    argument_rule_add_rule(&argument_rule_generate, "class");
    argument_rule_add_rule(&argument_rule_generate, "struct");
    argument_rule_add_rule(&argument_rule_generate, "file");
    argument_rule_array_add(&argument_rules, &argument_rule_generate);
    
    argument_rule_t argument_rule_git = argument_rule_init("git", NULL);
    argument_rule_array_add(&argument_rules, &argument_rule_git);*/

    return commands;
}