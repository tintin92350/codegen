#include "FileSystem.h"

#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

// UNIX / Linux header
#include <sys/stat.h>

char * getAbsolutePath(const char * filename, const char * workingDirectory)
{
    int filePathLength = strlen(workingDirectory) + 1 + strlen(filename) ;
    char * filePath = (char*)malloc(sizeof(char) * filePathLength);
    strcpy(filePath, workingDirectory);
    strcat(filePath, "/");
    strcat(filePath, filename);

    return filePath;
}

char * getFilenameWithExtension(const char * filename, const char * extension)
{
    int fullNameLength = strlen(filename) + 1 + strlen(extension) ;
    char * fullName = (char*)malloc(sizeof(char) * fullNameLength);
    strcpy(fullName, filename);
    strcat(fullName, ".");
    strcat(fullName, extension); 

    return fullName;
}

int createFile(const char * filename, const char * content)
{
    struct stat st = {0};

    createDirectories(filename);

    FILE * file = fopen(filename, "w+");
    

    if (file == NULL)
        return 0;

    fprintf(file, "%s", content);

    fclose(file);

    return 1;
}

char * appendHeaderCode(const char * content, const char * filename, const char * description, const char * author)
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    size_t nContentSize = strlen(content) + strlen(filename) + strlen(description) + strlen(author) + 100;
    char * nContent = (char*)malloc(nContentSize);

    sprintf(nContent,   "/**\n"
                        " * name: %s\n"
                        " * description : %s\n"
                        " * creation : %d/%d/%d at %d:%d:%d\n"
                        " * author : %s\n"
                        "*/\n\n"
                        "%s", 
                        filename, 
                        description, 
                        tm.tm_mday, 
                        tm.tm_mon + 1, 
                        tm.tm_year + 1900, 
                        tm.tm_hour, 
                        tm.tm_min, 
                        tm.tm_sec, 
                        author, 
                        content);

    return nContent;
}

int removeFile(const char * workingDirectory, const char * filename)
{
    char * removeFilePath = getAbsolutePath(filename, workingDirectory);
    int code = remove(removeFilePath);
    free(removeFilePath);
    return code  == 0;
}

char * getFileNameFromFilePath(const char * filepath)
{
    size_t length = strlen(filepath);
    int i = length - 1;

    int writing = 0;
    char * filename = NULL;

    while (i >= 0 && filepath[i] != '/' && filepath[i] != '\\') {
        writing++;
        i--;
    }

    filename = (char*)malloc(sizeof(char) * (writing+1));

    strncpy(filename, filepath+i+1, writing+1);

    filename[writing + 1] = '\0';
    return filename;
}

char * getFilePath(const char * filepath)
{
    size_t length = strlen(filepath);
    int i = length - 2;

    char * path = NULL;

    while (i >= 0 && filepath[i] != '/' && filepath[i] != '\\') {
        i--;
    }

    if (i <= 0)
        i = 0;

    path = (char*)malloc(sizeof(char) * (i+2));
    memset(path, 0, i+2);

    strncpy(path, filepath, i);
    strcat(path, "/\0");

    return path;
}

int getFilePathDeep(const char * filepath)
{
    size_t length = strlen(filepath);
    int deep = 0;

    for (int i = 0; i < length-1; i++) {
        if (filepath[i] == '/' || filepath[i] == '\\') {
            deep++;
        }
    }

    return deep;
}

int createDirectories(const char * directoryPath)
{
    int deep = getFilePathDeep(directoryPath);
    char ** allPath = (char**)malloc(sizeof(char*) * (deep+1));
    int startZero = 1;

    allPath[0] = (char*)malloc(sizeof(char*) * strlen(directoryPath));
    strcpy(allPath[0], directoryPath);

    // Is the last fragment a directory too ?
    if (directoryPath[strlen(directoryPath)-1] == '/' || directoryPath[strlen(directoryPath)-1] == '\\') 
        startZero = 0;

    // Get all path
    for (int i = 1; i <= deep; i++)
        allPath[i] = getFilePath(allPath[i-1]);

    int start = startZero;

    // Look for the first not existing directory 
    while (start <= deep && !dirExists(allPath[start])) start++;

    // All directory exists ?
    if (start == 1) return 1;

    for (int i = start - 1; i >= startZero; i--) {
        if (mkdir(allPath[i], 0755) != 0) {
            for (int i = 0; i <= deep; i++)
                free(allPath[i]);
            free(allPath);
            return 0;
        }
    }
    
    for (int i = 0; i <= deep; i++)
        free(allPath[i]);
    free(allPath);

    return 1;
}

int fileExists(const char * filepath)
{
    return access(filepath, F_OK) != -1;    
}

int dirExists(const char * directoryPath)
{
    struct stat sb;
    return stat(directoryPath, &sb) == 0;// && S_ISDIR(sb.st_mode);  
}

char * getBackDirectoryFromChildToParent(const char * childDirectory, const char * activeDirectory)
{
    int backSlashChild = 0;
    int backSlashActive = 0;
    for (backSlashChild=0; childDirectory[backSlashChild]; childDirectory[backSlashChild]=='/' ? backSlashChild++ : *childDirectory++);
    for (backSlashActive=0; activeDirectory[backSlashActive]; activeDirectory[backSlashActive]=='/' ? backSlashActive++ : *activeDirectory++);

    int cmp = activeDirectory - childDirectory;

    if (cmp < 0) {
        return NULL;
    }

    if (childDirectory[strlen(childDirectory)-1] != '/' || childDirectory[strlen(childDirectory)-1] != '\\')
        cmp++;

    char * path = (char*)malloc(sizeof(char) * cmp*3 + 1);
    memset(path, 0, cmp);

    for (int i = 0; i < cmp; i++)
        strcat(path, "../");

    path[3 * cmp + 1] = '\0';

    return path;
}