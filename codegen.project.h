#ifndef CODEGEN_PROJECT_HEADER_FILE
#define CODEGEN_PROJECT_HEADER_FILE

// JSON handling
#include <jansson.h>

// Standard library
#include <string.h>

// File
#include "file.h"

// Code gen file data
typedef struct CodeGenFile_t {

    char * projectName; // Name fo the project entered by user
    File ** files;      // Files attached to the project
    int filesCount;     // Length of the files array

    char * description;
    char * author;
    short int git;

} CodeGenFile;

/**
 * Build a new code gen file structure
 * @param projectName string value that represent the project name
 */
CodeGenFile* CodeGenFile_construct(const char * projectName);

/**
 * Build a new code gen file structure
 * @param projectName string value that represent the project name
 * @param description Little description of the project
 * @param author Who is the author of the project / code
 * @param initGit Need to init a git repository ?
 */
CodeGenFile* CodeGenFile_constructFull(const char * projectName, const char * description, const char * author, short int initGit);

/**
 * Build a code gen file structure from a codegen json file
 * @param filename File data 
 */
CodeGenFile* CodeGenFile_constructFromFile(const char * filename);

/**
 * Free a code gen file structure
 * @param codeGen CodeGen struct
 */
void CodeGenFile_free(CodeGenFile * codeGenFile);

/**
 * Add a file to the project
 * @param codeGen CodeGen struct
 * @param filename Name of the file
 */
void CodeGenFile_addFile(CodeGenFile * codeGenFile, const char * filename);

/**
 * Add a file to the project
 * @param codeGen CodeGen struct
 * @param filename Name of the file
 * @param fileext Extension of the file
 */
void CodeGenFile_addFile2(CodeGenFile * codeGenFile, const char * filename, const char * fileext);

/**
 * Remove a file of the project
 * @param codeGen CodeGen struct
 * @param filename Name of the file to remove
 */
int CodeGenFile_removeFile(CodeGenFile * codeGenFile, File * file);

/**
 * Check if a file already exist in the files array
 * @param codeGen CodeGen struct
 * @param filename Name of the file
 * @param fileext Extension of the file
 */
int CodeGenFile_fileExists(CodeGenFile * codeGenFile, const char * filename, const char * fileext);

/**
 * Export the code gen to file
 * @param codeGen CodeGen struct
 * @param filename Name of the exported file
 */
int CodeGenFile_export(CodeGenFile * codeGenFile, const char * filename);

#endif // CODEGEN_PROJECT_HEADER_FILE