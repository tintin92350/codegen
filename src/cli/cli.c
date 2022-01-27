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

int configureCliFromProgramArgument(int argc, char ** argv)
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