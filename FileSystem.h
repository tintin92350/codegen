#ifndef FILE_SYSTEM_HEADER_FILE
#define FILE_SYSTEM_HEADER_FILE

#include "codegen.project.h"

/**
 * Get an absolute path by combining working directory with filename
 * @param filename
 * @param workingDirectory
 * @return string
 */
char * getAbsolutePath(const char * filename, const char * workingDirectory);

/**
 * Concatenate in a string a filename with extension
 * @param filename
 * @param extension
 * @return string
 */
char * getFilenameWithExtension(const char * filename, const char * extension);

/**
 * Create a file to the path then fill it with the specified content
 * @param filename
 * @param content
 * @return state
 */
int createFile(const char * filename, const char * content);

/**
 * Append to the front of a string, a header-code
 * @param content
 * @return state
 */
char * appendHeaderCode(const char * content, const char * filename, const char * description, const char * author);

#endif // FILE_SYSTEM_HEADER_FILE