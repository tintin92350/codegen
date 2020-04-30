#include "FileSystem.h"

#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdio.h>

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