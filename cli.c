#include "cli.h"

// Standard library
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>

// UNIX / Linux header
#include <sys/stat.h>

#include "FileSystem.h"

#include "string.h"

int createNewProject(const char * projectName, const char * description, const char * author, short int initGit, const char * workingDirectory)
{
    /// CREATE THE DIRECTORY IF NOT EXISTS

    printf("-> Project directory (absolute path) : %s/%s/\n", workingDirectory, projectName);

    int newProjectDirectoryPathLength = strlen(workingDirectory) + strlen(projectName) + 2;
    char * newProjectDirectoryPath = strinit(newProjectDirectoryPathLength);
    strcpy(newProjectDirectoryPath, workingDirectory);
    strcat(newProjectDirectoryPath, "/");
    strcat(newProjectDirectoryPath, projectName);
    strcat(newProjectDirectoryPath, "/\0");
    
    DIR* newProjectDirectory = opendir(newProjectDirectoryPath);

    if (newProjectDirectory) {
        closedir(newProjectDirectory);
        perror("\033[0;33mDirectory already exists, skipped\033[0m\n");
    }
    else if (ENOENT == errno) {
        mkdir(newProjectDirectoryPath, 0700);
        printf("\033[0;32m-> Directory created with success !\033[0m\n");
    } else {
        perror("\n\033[0;31m/!\\ An error occurred when trying to create the directory...\033[0m\n\n");
        free(newProjectDirectoryPath);
        return 0;
    }

    CodeGenFile * codeGenFile = CodeGenFile_constructFull(projectName, description, author, initGit);

    CodeGenFile_addFile2(codeGenFile, projectName, "c");
    
    char * codeGenFilePath = getAbsolutePath(".codegen", newProjectDirectoryPath);

    CodeGenFile_export(codeGenFile, codeGenFilePath);
    
    char * mainFileName = getFilenameWithExtension(projectName, "c");
    char * mainFilePath = getAbsolutePath(mainFileName, newProjectDirectoryPath);

    const char * mainFileContentH = "// Standard library\n#include <stdio.h>\n#include <stdlib.h>\n\nint main(int argc, char ** argv)\n{\n\tprintf(\"hello world\\n\");\n\treturn EXIT_SUCCESS;\n}\n";
    char * mainFileContent = appendHeaderCode(mainFileContentH, mainFileName, "Main entry point of the program", author);

    if(createFile(mainFilePath, mainFileContent) != 1) {
        return 0;
    }

    free(codeGenFilePath);
    free(mainFileName);
    free(mainFilePath);
    free(mainFileContent);
    
    if (!generateMakefile(codeGenFile, newProjectDirectoryPath)) {
        perror("\n\033[0;31m/!\\ An error occurred when trying to create the makefile...\033[0m\n\n");
        return 0;
    } else {
        printf("\033[0;32m-> Makefile created with success !\033[0m\n");
    }
       
    printf("\033[0;32m-> Solution generated with success !\033[0m\n");

    return 1;
}

int generateMakefile(CodeGenFile * codegenFile, const char * workingDirectory) {

    char * makefile_path = getAbsolutePath("makefile", workingDirectory);

    FILE * makefile_file = fopen(makefile_path, "w+");

    if (makefile_file == NULL)
        return 0;

    fprintf(makefile_file, "%s : ", codegenFile->projectName);

    for (int j = 0; j < codegenFile->filesCount; j++) {
        if (codegenFile->files[j]->type == 1) {
            fprintf(makefile_file, "%s.o ", codegenFile->files[j]->name);
        }
    }

    fprintf(makefile_file, "\n\t gcc -o %s ", codegenFile->projectName);
    
    for (int j = 0; j < codegenFile->filesCount; j++) {
        if (codegenFile->files[j]->type == 1) {
            fprintf(makefile_file, "%s.o ", codegenFile->files[j]->name);
        }
    }

    fprintf(makefile_file, "\n\n");

    for (int i = 0; i < codegenFile->filesCount; i++) {
        if (codegenFile->files[i]->type == 1) {
            fprintf(makefile_file, "%s.o : %s.c\n\tgcc -c %s.c\n\n", codegenFile->files[i]->name, codegenFile->files[i]->name, codegenFile->files[i]->name);
        }
    }

    return 1;
}


int handleCli(int argc, char ** argv)
{
    char workingDirectory[PATH_MAX];

    // Retrieve working directory
    if (getcwd(workingDirectory, sizeof(workingDirectory)) == NULL) {
        perror("getcwd() error");
        return 0;
    }

    char * mainOperation = argv[1];

    // Create a new project
    if(strequals(mainOperation, "new") || strequals(mainOperation, "n")) {

        if (argc < 3) {
            perror("\n\033[0;31m/!\\ You need to provide the name of the project at least ! (enter help to have helpfull commands)\033[0m\n\n");
            return EXIT_FAILURE;
        }

        short int remainingArguments = argc - 3;
        short int currentArgument = 3;

        char * projectName = astrcpy(argv[2]);

        char * description = (char*)malloc(1);
        strcpy(description, "\0");

        char * author = (char*)malloc(1);
        strcpy(author, "\0");

        short int initGit = 0;

        while (remainingArguments > 0) {
            char * arg = argv[currentArgument];

            if (isArgumentKey(arg)) {

                if (strequals(arg, "-d") || strequals(arg, "--description")) {

                    if (currentArgument + 1 >= argc) {
                        printf("\033[0;31m/!\\ Missing description value\033[0m\n");
                    } else {
                        remainingArguments--;
                        currentArgument++;

                        free(description);
                        description = astrcpy(argv[currentArgument]);
                    }

                }
                else if (strequals(arg, "-a") || strequals(arg, "--author")) {

                    if (currentArgument + 1 >= argc) {
                        printf("\033[0;31m/!\\ Missing author value\033[0m\n");
                    } else {
                        remainingArguments--;
                        currentArgument++;

                        free(author);
                        author = astrcpy(argv[currentArgument]);
                    }

                }
                else if (strequals(arg, "--git")) {
                    initGit = 1;
                }
                else {
                    printf("\033[0;31m/!\\ Wrong argument '%s'\033[0m\n", arg);
                }

            } else {
                printf("\033[0;31m/!\\ Wrong argument '%s'\033[0m\n", arg);
            }
            remainingArguments--;
            currentArgument++;
        }

        int returnCode = createNewProject(projectName, description, author, initGit, workingDirectory);

    }
    
    // Generate files for project
    else if(strequals(mainOperation, "add") || strequals(mainOperation, "a")) {

        if (!isCodegenSolution(workingDirectory)) {
            perror("\n\033[0;31m/!\\ You need to be under a codegen generated directory to use this command\033[0m\n\n");
            return 0;
        }

        if (argc < 3) {
            perror("\n\033[0;31m/!\\ You need to provide the type of element you want to add ! (enter help to have helpfull commands)\033[0m\n\n");
            return 0;
        }
        else if (argc < 4) {
            perror("\n\033[0;31m/!\\ You need to provide a name ! (enter help to have helpfull commands)\033[0m\n\n");
            return 0;
        }
        
        CodeGenFile * codegenFile = openCodegenSolution(workingDirectory);
        char * codegenFilePath = getAbsolutePath(".codegen", workingDirectory);

        const char * type = argv[2];
        const char * name = argv[3];

        short int remainingArguments = argc - 4;
        short int currentArgument = 4;

        char * description = (char*)malloc(1);
        strcpy(description, "\0");

        char * author = codegenFile->author;

        short int initGit = 0;

        while (remainingArguments > 0) {
            char * arg = argv[currentArgument];

            if (isArgumentKey(arg)) {

                if (strequals(arg, "-d") || strequals(arg, "--description")) {

                    if (currentArgument + 1 >= argc) {
                        printf("\033[0;31m/!\\ Missing description value\033[0m\n");
                    } else {
                        remainingArguments--;
                        currentArgument++;

                        free(description);
                        description = astrcpy(argv[currentArgument]);
                    }

                }
                else if (strequals(arg, "-a") || strequals(arg, "--author")) {

                    if (currentArgument + 1 >= argc) {
                        printf("\033[0;31m/!\\ Missing author value\033[0m\n");
                    } else {
                        remainingArguments--;
                        currentArgument++;

                        free(author);
                        author = astrcpy(argv[currentArgument]);
                    }

                }
                else {
                    printf("\033[0;31m/!\\ Wrong argument '%s'\033[0m\n", arg);
                }

            } else {
                printf("\033[0;31m/!\\ Wrong argument '%s'\033[0m\n", arg);
            }
            remainingArguments--;
            currentArgument++;
        }

        if (!codegenFile)
            return 0;

        if (strequals(type, "file") || strequals(type, "f")) {

            addFileToSolution(codegenFile, workingDirectory, name, description, author);

            CodeGenFile_export(codegenFile, codegenFilePath);

            generateMakefile(codegenFile, workingDirectory);

        } else if (strequals(type, "struct") || strequals(type, "s")) {
            
        } else if (strequals(type, "enum") || strequals(type, "e")) {
            
        } else if (strequals(type, "union") || strequals(type, "u")) {
            
        } else {
            printf("\n\033[0;31m/!\\ Unknown element type '%s' (enter help to have helpfull commands)\033[0m\n\n", type);
            return 0;
        }
    }
}

int isArgumentKey(char * argument) 
{
    return argument[0] == '-';
}

int isCodegenSolution(const char * workingDirectory)
{
    char * codegen_file = strinit(strlen(workingDirectory) + 20);
    strcpy(codegen_file, workingDirectory);
    strcat(codegen_file, "/.codegen");

    return access( codegen_file, F_OK ) != -1;
}

CodeGenFile * openCodegenSolution(const char * workingDirectory)
{
    char * codegen_file = strinit(strlen(workingDirectory) + 20);
    strcpy(codegen_file, workingDirectory);
    strcat(codegen_file, "/.codegen");

    return CodeGenFile_constructFromFile(codegen_file);
}

int addFileToSolution(CodeGenFile * codegenFile, const char * workingDirectory, const char * filename, const char * description, const char * author)
{
    char * path = getAbsolutePath(filename, workingDirectory);
    char * sourceFilename = getFilenameWithExtension(path, "c");
    char * headerFilename = getFilenameWithExtension(path, "h");

    char * sourceContent = (char*)malloc(sizeof(char) * (13 + strlen(filename)));
    char * headerContent = (char*)malloc(sizeof(char) * (100 + 3 * strlen(filename)));

    sprintf(sourceContent, "#include \"%s.h\"\n", filename);
    sprintf(headerContent, "#ifndef %s_HEADER_FILE\n#define %s_HEADER_FILE\n\n#endif // %s_HEADER_FILE\n", filename, filename, filename);

    char * headerContentTagged = appendHeaderCode(headerContent, filename, description, author);

    printf("source file : %s\n", sourceFilename);

    if (!createFile(sourceFilename, sourceContent)) {
        printf("\033[0;31m/!\\ File '%s.c' can't be created !\033[0m\n", filename);
        return 0;
    } else {
        CodeGenFile_addFile2(codegenFile, filename, "c");
        printf("\033[0;32m-> File '%s.c' generated with success !\033[0m\n", filename);
    }

    if(!createFile(headerFilename, headerContentTagged)) {
        printf("\033[0;31m/!\\ File '%s.h' can't be created !\033[0m\n", filename);
        return 0;
    } else {
        CodeGenFile_addFile2(codegenFile, filename, "h");
        printf("\033[0;32m-> File '%s.h' generated with success !\033[0m\n", filename);
    }
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        perror("\n\033[0;31m/!\\ This program take at least one parameter (enter help to have helpfull commands)\033[0m\n\n");
        return EXIT_FAILURE;
    }

    int code = handleCli(argc, argv);

    return code;
}