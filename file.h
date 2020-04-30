#ifndef FILE_HEADER_FILE
#define FILE_HEADER_FILE

// JSON handling
#include <jansson.h>

// Standard library
#include <string.h>

#define FILE_TYPE_UNKNOWN 0
#define FILE_TYPE_C 1
#define FILE_TYPE_H 2

// Code gen file data
typedef struct File_t {

    char * name;
    int type;

} File;

/**
 * Build a new file structure
 * @param name Name fo the file
 * @param extension Type of file
 */
File* File_construct(const char * name, const char * ext);

/**
 * Build a new file structure from filename
 * @param filename Name of the file
 */
File* File_construct2(const char * filename);

/**
 * Free a file structure
 * @param file File struct
 */
void File_free(File * file);

#endif // FILE_HEADER_FILE