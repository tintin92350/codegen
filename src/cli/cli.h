/*
 * @author: Quentin ROIC (quentin.rodic.pro@outlook.fr)
 * @date: 01-01-1970 1:00:00 am
 * @lastModifiedBy: Quentin ROIC (quentin.rodic.pro@outlook.fr>)
 * @lastModifiedTime: 27-01-2022 8:46:58 am
 */

#ifndef CLI_HEADER
#define CLI_HEADER

#include "../modules/CLI_IHC/argument_rule/argument.rule.h"
#include "../modules/CLI_IHC/command/command.h"

void printCodegenLogoAndVersion();

int configureCliFromProgramArgument(int argc, char **argv);

command_array_t configure_cli_commands();

#endif // CLI_HEADER