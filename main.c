// Standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <dirent.h>
#include <errno.h>
#include <time.h>

// UNIX / Linux header
#include <sys/stat.h>

// Code gen project file
#include "codegen.project.h"

// Commands
#define CMD_NEW "new"
#define CMD_HELP "help"
#define CMD_ADD "add"
#define CMD_REMOVE "remove"
#define CMD_REGENRATE_MAKEFILE "remake"

void push(char ** arr, int index, char * value, int *size, int *capacity){
     if(*size > *capacity){
          arr = realloc(arr, *capacity * 2);
          *capacity = *capacity * 2;
     }
     
     arr[index] = malloc(sizeof(char) * 60);
     strcpy(arr[index], value);
     *size = *size + 1;
}

short int strequals(const char * str1, const char * str2) {
    return strcmp(str1, str2) == 0;
}

char * newString(const int size) {
    return (char *)malloc(size * sizeof(char));
}

int isCodegenProject(const char * directory) {
    char * codegen_file = newString(strlen(directory) + 20);
    strcpy(codegen_file, directory);
    strcat(codegen_file, "/.codegen");

    return access( codegen_file, F_OK ) != -1;
}

int generateMakefile(const char *);

int createMainFileProccess(const char *, const char *);
int createCodeGenPropertiesProccess(const char *, const char *);
int createMakefileProccess(const char *, const char *);

int addFileToProject(const char *, const char *);
int createFile(const char *, const char *, const int);

int removeFileFromProject(const char *, const char *);
int removeFile(const char *, const char *);

int main(int argc, char ** argv)
{
    printf("\033[1;31m"
            "                                                                                 \n"
            "    ________  ________  ________  _______   ________  _______   ________         \n"
            "   |\\   ____\\|\\   __  \\|\\   ___ \\|\\  ___ \\ |\\   ____\\|\\  ___ \\ |\\   ___  \\       \n"
            "   \\ \\  \\___|\\ \\  \\|\\  \\ \\  \\_|\\ \\ \\   __/|\\ \\  \\___|\\ \\   __/|\\ \\  \\\\ \\  \\      \n"
            "    \\ \\  \\    \\ \\  \\\\\\  \\ \\  \\ \\\\ \\ \\  \\_|/_\\ \\  \\  __\\ \\  \\_|/_\\ \\  \\\\ \\  \\     \n"
            "     \\ \\  \\____\\ \\  \\\\\\  \\ \\  \\_\\\\ \\ \\  \\_|\\ \\ \\  \\|\\  \\ \\  \\_|\\ \\ \\  \\\\ \\  \\    \n"
            "      \\ \\_______\\ \\_______\\ \\_______\\ \\_______\\ \\_______\\ \\_______\\ \\__\\\\ \\__\\   \n"
            "       \\|_______|\\|_______|\\|_______|\\|_______|\\|_______|\\|_______|\\|__| \\|__|   \n"                                                                           
           "   \033[0m                                                                              \n"
           " ******************************************************************************* \n"
           "     CODE GEN : Version beta                                                     \n"
           "     LANGUAGE : C                                                                \n"
           " ******************************************************************************* \n");

    if (argc < 2) {
        perror("\n\033[0;31m/!\\ This program take at least one parameter (enter help to have helpfull commands)\033[0m\n\n");
        return EXIT_FAILURE;
    }

    char workingDirectory[PATH_MAX];

    // Retrieve working directory
    if (getcwd(workingDirectory, sizeof(workingDirectory)) == NULL) {
        perror("getcwd() error");
        return EXIT_FAILURE;
    }

    // User wants to create a project
    if (strequals(argv[1], CMD_NEW)) {
        
        if (argc < 3) {
            perror("\n\033[0;31m/!\\ You need to provide the name of the project at least ! (enter help to have helpfull commands)\033[0m\n\n");
            return EXIT_FAILURE;
        }
        else {
            
        }

        printf("-> Project name : %s\n", argv[2]);

        /// CREATE THE DIRECTORY IF NOT EXISTS

        printf("-> Project directory (absolute path) : %s/%s/\n", workingDirectory, argv[2]);

        int newProjectDirectoryPathLength = strlen(workingDirectory) + strlen(argv[2]) + 3;
        char * newProjectDirectoryPath = newString(newProjectDirectoryPathLength);
        strcpy(newProjectDirectoryPath, workingDirectory);
        strcat(newProjectDirectoryPath, "/");
        strcat(newProjectDirectoryPath, argv[2]);
        strcat(newProjectDirectoryPath, "/");
        
        DIR* newProjectDirectory = opendir(newProjectDirectoryPath);

        if (newProjectDirectory) {
            closedir(newProjectDirectory);
            perror("\033[0;33mDirectory already exists, skipped\033[0m\n");
        }
        else if (ENOENT == errno) {
            mkdir(newProjectDirectoryPath, 0700);
            printf("\033[0;32m-> Directory created with success !\n");
        } else {
            perror("\n\033[0;31m/!\\ An error occurred when trying to create the directory...\033[0m\n\n");

            free(newProjectDirectoryPath);
            return EXIT_FAILURE;
        }

        // CREATE MAIN FILE
        if (!createMainFileProccess(newProjectDirectoryPath, argv[2])) {
            printf("\n\033[0;31m/!\\ An error occurred when trying to create the main file %s...\033[0m\n\n", argv[2]);
            free(newProjectDirectoryPath);
        } else {
            printf("\033[0;32m-> Main file %s created with success !\033[0m\n", argv[2]);
        }

        // CREATE CODEGEN FILE
        if (!createCodeGenPropertiesProccess(newProjectDirectoryPath, argv[2])) {
            perror("\n\033[0;31m/!\\ An error occurred when trying to create the codegen properties file...\033[0m\n\n");
            free(newProjectDirectoryPath);
        } else {
            printf("\033[0;32m-> Codegen file created with success !\033[0m\n");
        }

        // CREATE MAKEFILE
        if (!createMakefileProccess(newProjectDirectoryPath, argv[2])) {
            perror("\n\033[0;31m/!\\ An error occurred when trying to create the makefile...\033[0m\n\n");
            free(newProjectDirectoryPath);
        } else {
            printf("\033[0;32m-> Makefile created with success !\033[0m\n");
        }
        
        free(newProjectDirectoryPath);

    } else if (strequals(argv[1], CMD_ADD)) {
        
        if (argc < 3) {
            perror("\n\033[0;31m/!\\ You need to provide the name of the file to add ! (enter help to have helpfull commands)\033[0m\n\n");
            return EXIT_FAILURE;
        }

        if (!addFileToProject(workingDirectory, argv[2])) {
            perror("\n\033[0;31m/!\\ Can't add the file to the project\033[0m\n\n");
        }
    } else if (strequals(argv[1], CMD_REMOVE)) {
        
        if (argc < 3) {
            perror("\n\033[0;31m/!\\ You need to provide the name of the file to remove ! (enter help to have helpfull commands)\033[0m\n\n");
            return EXIT_FAILURE;
        }

        if (!removeFileFromProject(workingDirectory, argv[2])) {
            perror("\n\033[0;31m/!\\ Can't remove the file from the project\033[0m\n\n");
        }
    } else if (strequals(argv[1], CMD_REGENRATE_MAKEFILE)) {
        if (!generateMakefile(workingDirectory)) {
            perror("\n\033[0;31m/!\\ An error occurred when trying to re-generate makefile\033[0m\n\n");
        } else {
            printf("\033[0;32m-> Makefile generated with success !\033[0m\n");
        }
    } else if (strequals(argv[1], CMD_HELP)) {

        printf("--------------------------------\n");
        printf("List of all command availables :\n");
        printf("--------------------------------\n");
        printf("new project_name\tGenerate hello world project with the name 'project_name'\n");
        printf("add file_name   \tAdd files with the names 'file_name'.c and 'file_name'.h to the project and update it\n");
        printf("remove file_name\t Remove a file with the name 'file_name' to the project and update it\n");
        printf("remake          \tRe generate makefile from codegen project\n");

    } else {
        printf("\n\033[0;31m/!\\ Unknow command '%s'\033[0m\n\n", argv[1]);
    }

    return EXIT_SUCCESS;
}


int createMainFileProccess(const char * workingDirectory, const char * filename) {
    int newProjectMainFilePathLength = strlen(workingDirectory) + strlen(filename) + 6;
    char * newProjectMainFilePath = newString(newProjectMainFilePathLength);
    strcpy(newProjectMainFilePath, workingDirectory);
    strcat(newProjectMainFilePath, "/");
    strcat(newProjectMainFilePath, filename);
    strcat(newProjectMainFilePath, ".c");

    FILE * mainFile = fopen(newProjectMainFilePath, "w+");

    if (mainFile == NULL) {
        free(newProjectMainFilePath);
        return 0;
    }

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    fprintf(mainFile,  "/**\n * name: %s.c\n * description : Definition of the entry point of the program\n * creation : %d/%d/%d at %d:%d:%d\n * author : %s\n*/\n\n// Standard library\n#include <stdio.h>\n#include <stdlib.h>\n\nint main(int argc, char ** argv)\n{\n\tprintf(\"hello world\\n\");\n\treturn EXIT_SUCCESS;\n}\n", 
                            filename, tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec, "Quentin RODIC");

    fclose(mainFile);
    
    free(newProjectMainFilePath);

    return 1;
}

int createCodeGenPropertiesProccess(const char * workingDirectory, const char * filename) {
    
    int newProjectCodeGenFilePathLength = strlen(workingDirectory) + 10;
    char * newProjectCodeGenFilePath = newString(newProjectCodeGenFilePathLength);
    strcpy(newProjectCodeGenFilePath, workingDirectory);
    strcat(newProjectCodeGenFilePath, "/.codegen");

    CodeGenFile* codeGenFile = CodeGenFile_construct(filename);

    CodeGenFile_addFile2(codeGenFile, filename, "c");

    CodeGenFile_export(codeGenFile, newProjectCodeGenFilePath);

    return 1;
}

int createMakefileProccess(const char * workingDirectory, const char * filename) {
    int newProjectMakeFilePathLength = strlen(workingDirectory) + 10;
    char * newProjectMakeFilePath = newString(newProjectMakeFilePathLength);
    strcpy(newProjectMakeFilePath, workingDirectory);
    strcat(newProjectMakeFilePath, "/makefile");

    FILE * codeGenFile = fopen(newProjectMakeFilePath, "w+");

    if (codeGenFile == NULL) {
        free(newProjectMakeFilePath);
        return 0;
    }

    fprintf(codeGenFile, "%s : %s.o\n\tgcc -o %s %s.o\n\n%s.o : %s.c\n\tgcc -c %s.c", filename, filename, filename, filename, filename, filename, filename);

    fclose(codeGenFile);
    
    free(newProjectMakeFilePath);

    return 1;
}

int addFileToProject(const char * workingDirectory, const char * filename)
{
    if (!isCodegenProject(workingDirectory)) {
        perror("\n\033[0;31m/!\\ Current working directory is not a codegen managed directory !\033[0m\n\n");
        return 0;
    }
   
    char * codegen_file_path = newString(strlen(workingDirectory) + 20);
    strcpy(codegen_file_path, workingDirectory);
    strcat(codegen_file_path, "/.codegen");

    CodeGenFile * codegenFile = CodeGenFile_constructFromFile(codegen_file_path);

    
    if (!createFile(workingDirectory, filename, 0)) {
        printf("\033[0;31m/!\\ File '%s.c' can't be created !\033[0m\n", filename);
        return 0;
    } else {
        CodeGenFile_addFile2(codegenFile, filename, "c");
        printf("\033[0;32m-> File '%s.c' generated with success !\033[0m\n", filename);
    }

    if(!createFile(workingDirectory, filename, 1)) {
        printf("\033[0;31m/!\\ File '%s.h' can't be created !\033[0m\n", filename);
        return 0;
    } else {
        CodeGenFile_addFile2(codegenFile, filename, "h");
        printf("\033[0;32m-> File '%s.h' generated with success !\033[0m\n", filename);
    }

    CodeGenFile_export(codegenFile, codegen_file_path);

    generateMakefile(workingDirectory);

   return 1;
}


int createFile(const char * workingDirectory, const char * filename, const int type) {
    int newFilePathLength = strlen(workingDirectory) + strlen(filename) + 4;
    char * newFilePath = newString(newFilePathLength);
    strcpy(newFilePath, workingDirectory);
    strcat(newFilePath, "/");
    strcat(newFilePath, filename);

    if (type == 0)
        strcat(newFilePath, ".c");
    else
        strcat(newFilePath, ".h");

    FILE * newFile = fopen(newFilePath, "w+");

    if (newFile == NULL) {
        free(newFilePath);
        return 0;
    }

    if (type == 0)
        fprintf(newFile, "#include \"%s.h\"\n", filename);
    else
        fprintf(newFile, "#ifndef %s_HEADER_FILE\n#define %s_HEADER_FILE\n\n#endif // %s_HEADER_FILE\n", filename, filename, filename);

    fclose(newFile);
    
    free(newFilePath);

    return 1;
}


int generateMakefile(const char * workingDirectory) {

    char * codegen_file_path = newString(strlen(workingDirectory) + 20);
    strcpy(codegen_file_path, workingDirectory);
    strcat(codegen_file_path, "/.codegen");
    
    CodeGenFile * codegenFile = CodeGenFile_constructFromFile(codegen_file_path);

    char * makefile_path = newString(strlen(workingDirectory) + 10);
    strcpy(makefile_path, workingDirectory);
    strcat(makefile_path, "/makefile");

    FILE * makefile_file = fopen(makefile_path, "w+");

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

int removeFileFromProject(const char * workingDirectory, const char * filename)
{
    if (!isCodegenProject(workingDirectory)) {
        perror("\n\033[0;31m/!\\ Current working directory is not a codegen managed directory !\033[0m\n\n");
        return 0;
    }
   
    char * codegen_file_path = newString(strlen(workingDirectory) + 20);
    strcpy(codegen_file_path, workingDirectory);
    strcat(codegen_file_path, "/.codegen");

    CodeGenFile * codegenFile = CodeGenFile_constructFromFile(codegen_file_path);

    
    if (!removeFile(workingDirectory, filename)) {
        printf("\033[0;31m/!\\ File '%s' can't be removed !\033[0m\n", filename);
        return 0;
    } else {
        File * file = File_construct2(filename);
        if (CodeGenFile_removeFile(codegenFile, file))
            printf("\033[0;32m-> File '%s' removed with success !\033[0m\n", filename);
        else
            printf("\033[0;31m/!\\ File '%s' can't be removed from project !\033[0m\n", filename);
    }

    CodeGenFile_export(codegenFile, codegen_file_path);

    generateMakefile(workingDirectory);

   return 1;
}

int removeFile(const char * workingDirectory, const char * filename)
{
    int removeFilePathLength = strlen(workingDirectory) + strlen(filename) + 4;
    char * removeFilePath = newString(removeFilePathLength);
    strcpy(removeFilePath, workingDirectory);
    strcat(removeFilePath, "/");
    strcat(removeFilePath, filename);
    return remove(removeFilePath) == 0;
}