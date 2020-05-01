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
    printf("\033[1;31m"
            "      ___________ __  ______    \n"
            "     / ____/ ___//  |/  /   |   \n"
            "    / /    \\__ \\/ /|_/ / /| |   \n"
            "   / /___ ___/ / /  / / ___ |   \n"
            "   \\____//____/_/  /_/_/  |_|   \33[0m\n\n"
            "   C Solution Management App    \n"
            "   Beta Version                 \n\n");


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

        char * author = (char *)malloc(sizeof(char) * strlen(codegenFile->author));
        strcpy(author, codegenFile->author);

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

            addStructToSolution(codegenFile, workingDirectory, name, description, author);

            CodeGenFile_export(codegenFile, codegenFilePath);

            generateMakefile(codegenFile, workingDirectory);
            
        } else if (strequals(type, "enum") || strequals(type, "e")) {

            addEnumToSolution(codegenFile, workingDirectory, name, description, author);

            CodeGenFile_export(codegenFile, codegenFilePath);

            generateMakefile(codegenFile, workingDirectory);
            
        }  else {
            printf("\n\033[0;31m/!\\ Unknown element type '%s' (enter help to have helpfull commands)\033[0m\n\n", type);
            return 0;
        }
    }

    // Remove files from project
    else if(strequals(mainOperation, "remove") || strequals(mainOperation, "r")) {

        if (!isCodegenSolution(workingDirectory)) {
            perror("\n\033[0;31m/!\\ You need to be under a codegen generated directory to use this command\033[0m\n\n");
            return 0;
        }

        if (argc < 3) {
            perror("\n\033[0;31m/!\\ You need to provide at least one name ! (enter help to have helpfull commands)\033[0m\n\n");
            return 0;
        }
        
        CodeGenFile * codegenFile = openCodegenSolution(workingDirectory);
        char * codegenFilePath = getAbsolutePath(".codegen", workingDirectory);

        if (!codegenFile)
            return 0;

        const char * name = argv[2];

        short int remainingArguments = argc - 3;
        short int currentArgument = 3;

        short int deleteFile = 0;

        while (remainingArguments > 0) {
            char * arg = argv[currentArgument];

            if (isArgumentKey(arg)) {

                if (strequals(arg, "-d") || strequals(arg, "--delete")) {
                    deleteFile = 1;                    
                }

            } else {
                printf("\033[0;31m/!\\ Wrong argument '%s'\033[0m\n", arg);
            }
            remainingArguments--;
            currentArgument++;
        }

        if (removeFileFromSolution(codegenFile, name, workingDirectory, deleteFile)) {        
            CodeGenFile_export(codegenFile, codegenFilePath);
            generateMakefile(codegenFile, workingDirectory);
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

int addStructToSolution(CodeGenFile * codegenFile, const char * workingDirectory, const char * structName, const char * description, const char * author)
{
    char * path = getAbsolutePath(structName, workingDirectory);
    char * sourceFilename = getFilenameWithExtension(path, "c");
    char * headerFilename = getFilenameWithExtension(path, "h");

    const char * headerContentHack = "#ifndef %s_HEADER_FILE\n#define %s_HEADER_FILE\n\nstruct %s {\n\n};\n\ntypedef struct %s %s_t;\n\n#endif // %s_HEADER_FILE\n";
    const char * sourceContentHack = "#include \"%s.h\"\n";

    char * sourceContent = (char*)malloc(sizeof(char) * (strlen(sourceContentHack) + strlen(structName)));
    char * headerContent = (char*)malloc(sizeof(char) * (strlen(headerContentHack) + 6 * strlen(structName)));

    sprintf(sourceContent, sourceContentHack, structName);
    sprintf(headerContent, headerContentHack, structName, structName, structName, structName, structName, structName);

    char * headerContentTagged = appendHeaderCode(headerContent, structName, description, author);

    printf("source file : %s\n", sourceFilename);

    if (!createFile(sourceFilename, sourceContent)) {
        printf("\033[0;31m/!\\ File '%s.c' can't be created !\033[0m\n", structName);
        return 0;
    } else {
        CodeGenFile_addFile2(codegenFile, structName, "c");
        printf("\033[0;32m-> File '%s.c' generated with success !\033[0m\n", structName);
    }

    if(!createFile(headerFilename, headerContentTagged)) {
        printf("\033[0;31m/!\\ File '%s.h' can't be created !\033[0m\n", structName);
        return 0;
    } else {
        CodeGenFile_addFile2(codegenFile, structName, "h");
        printf("\033[0;32m-> File '%s.h' generated with success !\033[0m\n", structName);
    }
}

int addEnumToSolution(CodeGenFile * codegenFile, const char * workingDirectory, const char * enumName, const char * description, const char * author)
{
    char * path = getAbsolutePath(enumName, workingDirectory);
    char * headerFilename = getFilenameWithExtension(path, "h");

    const char * headerContentHack = "#ifndef %s_HEADER_FILE\n#define %s_HEADER_FILE\n\nenum %s {};\n\n#endif // %s_HEADER_FILE\n";

    char * headerContent = (char*)malloc(sizeof(char) * (strlen(headerContentHack) + 4 * strlen(enumName)));

    sprintf(headerContent, headerContentHack, enumName, enumName, enumName, enumName);

    char * headerContentTagged = appendHeaderCode(headerContent, enumName, description, author);

    if(!createFile(headerFilename, headerContentTagged)) {
        printf("\033[0;31m/!\\ File '%s.h' can't be created !\033[0m\n", enumName);
        return 0;
    } else {
        CodeGenFile_addFile2(codegenFile, enumName, "h");
        printf("\033[0;32m-> File '%s.h' generated with success !\033[0m\n", enumName);
    }
}

int removeFileFromSolution(CodeGenFile * codegenFile, const char * filename, const char * workingDirectory, short int deleteFile)
{
    File * file = File_construct2(filename);
    
    if (CodeGenFile_removeFile(codegenFile, file))
        printf("\033[0;32m-> File '%s' removed with success !\033[0m\n", filename);
    else
        printf("\033[0;31m/!\\ File '%s' can't be removed from project !\033[0m\n", filename);

    if (deleteFile) {
        if (!removeFile(workingDirectory, filename)) {
            printf("\033[0;31m/!\\ File '%s' can't be removed !\033[0m\n", filename);
            return 0;
        } else {
            printf("\033[0;32m-> File deleted with success\033[0m\n");
        }
    }

    return 1;
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