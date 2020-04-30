#include "file.h"

File* File_construct(const char * name, const char * ext) {
    File * file = (File*)malloc(sizeof(File));

    file->name = (char*)malloc(sizeof(char) * strlen(name));
    strcpy(file->name, name);

    if (strcmp(ext, "c") == 0)
        file->type = FILE_TYPE_C;
    else if (strcmp(ext, "h") == 0)
        file->type = FILE_TYPE_H;
    else 
        file->type = FILE_TYPE_UNKNOWN;

    return file;
}

File* File_construct2(const char * filename)
{
    File * file = (File*)malloc(sizeof(File));

    size_t length = strlen(filename);

    file->name = (char*)malloc(sizeof(char) * (length-1));
    char * file_ext  = (char*)malloc(sizeof(char) * 2);

    memset(file->name, 0, (length - 1));
    memset(file_ext, 0, 1);

    strncpy(file->name, filename, length - 2);
    strncpy(file_ext, filename + (length - 1), 1);

    file->name[length - 1] = '\0';
    file_ext[1] = '\0';

    file->type = (strcmp(file_ext, "h") == 0 ? FILE_TYPE_H : FILE_TYPE_C);

    return file;
}

void File_free(File * file)
{
    free(file->name);
    free(file);
}