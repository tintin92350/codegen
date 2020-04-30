#ifndef CLI_HEADER_FILE
#define CLI_HEADER_FILE

#include "codegen.project.h"

/**
 * Create a new project bases on current working directory and information provided by user
 * @param projectName Name of the project
 * @param description Little description of the project
 * @param author Who is the author of the project / code
 * @param initGit Need to init a git repository ?
 * @param workingDirectory Current working directory
 * @return state
 */
int createNewProject(const char * projectName, const char * description, const char * author, short int initGit, const char * workingDirectory);

/**
 * Generate a makefile
 */
int generateMakefile(CodeGenFile * codegenFile, const char * workingDirectory);

/**
 * Handle the CLI arguments and makde operations
 * @param argc Number of argument
 * @param argv Argument
 */
int handleCli(int argc, char ** argv);

/**
 * Returns true if argument is of type 'key'
 * @param argument
 */
int isArgumentKey(char * argument);

/**
 * Returns true if the current directory is under managment of codegen
 * @param workingDirectory
 */
int isCodegenSolution(const char * workingDirectory);

/**
 * Open the codegen file
 * @param workingDirectory
 */
CodeGenFile * openCodegenSolution(const char * workingDirectory);

/**
 * Add a files to the project
 * @param filename
 */
int addFileToSolution(CodeGenFile * codegenFile, const char * workingDirectory, const char * filename, const char * description, const char * author);

#endif // CLI_HEADER_FILE