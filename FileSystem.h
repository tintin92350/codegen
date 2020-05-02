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

/**
 * Remove a file of the fs
 * @param workingDirectory
 * @param filename
*/
int removeFile(const char *, const char *);

/**
 * Extract filename from file path
 * @param filepath
 * @return filename
*/
char * getFileNameFromFilePath(const char *);

/**
 * Extract file path without filename from file path
 * @param filepath
 * @return file path without filename
*/
char * getFilePath(const char *);

/**
 * Returns the file path deep (number of subfolder to reach the directory)
 * @param filepath
 * @return number of subfolder to reach directory
*/
int getFilePathDeep(const char *);

/**
 * Create all directories
 * @param directoryPath
*/
int createDirectories(const char *);

/**
 * file exists ?
 * @param filepath
*/
int fileExists(const char *);

/**
 * directory exists ?
 * @param directoryPath
*/
int dirExists(const char *);

/**
 * Get directories back slash from directory to parent
 * @param childDirectory
 * @param activeDirectory
*/
char * getBackDirectoryFromChildToParent(const char *, const char *);

#endif // FILE_SYSTEM_HEADER_FILE