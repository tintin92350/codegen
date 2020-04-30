#include "codegen.project.h"

CodeGenFile* CodeGenFile_construct(const char * projectName) {
    CodeGenFile* codeGenFile = (CodeGenFile*)malloc(sizeof(CodeGenFile));
    
    codeGenFile->projectName = (char*)malloc(sizeof(char) * strlen(projectName));
    strcpy(codeGenFile->projectName, projectName);

    codeGenFile->filesCount = 0;
    codeGenFile->files = (File**)malloc(0);

    return codeGenFile;
}

void CodeGenFile_free(CodeGenFile * codeGenFile)
{
    free(codeGenFile->projectName);
    free(codeGenFile);
}

CodeGenFile* CodeGenFile_constructFromFile(const char * filename)
{
    CodeGenFile* codeGenFile = (CodeGenFile*)malloc(sizeof(CodeGenFile));
    codeGenFile->filesCount = 0;

    FILE * file = fopen(filename, "r");

    json_t *root;
    json_error_t error;
    root = json_loadf(file, 0, &error);

    if(!root)
    {
        fprintf(stderr, "error: on line %d: %s\n", error.line, error.text);
        return NULL;
    }

    if(!json_is_object(root))
    {
        fprintf(stderr, "error: root is not an object\n");
        json_decref(root);
        return NULL;
    }

    /// Retrieve project name from json data
    json_t * projectNameJson = json_object_get(root, "project-name");
    
    if (!projectNameJson) {
        fprintf(stderr, "error: projectName is not defined\n");
        json_decref(root);
        return NULL;
    }

    if(!json_is_string(projectNameJson))
    {
        fprintf(stderr, "error: projectName is not a string\n");
        json_decref(root);
        return NULL;
    }

    const char* projectName = json_string_value(projectNameJson);

    codeGenFile->projectName = (char*)malloc(sizeof(char) * strlen(projectName));
    strcpy(codeGenFile->projectName, projectName);

    /// Retrieve project files from json data
    json_t * filesJson = json_object_get(root, "files");
    
    if (!filesJson) {
        fprintf(stderr, "error: files is not defined\n");
        json_decref(root);
        return NULL;
    }

    if(!json_is_array(filesJson))
    {
        fprintf(stderr, "error: files is not an array\n");
        json_decref(root);
        return NULL;
    }

    /* array is a JSON array */
    size_t fileIndex;
    json_t * fileValueJson;

    json_array_foreach(filesJson, fileIndex, fileValueJson) {

        if(!json_is_object(fileValueJson))
        {
            fprintf(stderr, "error: file value is not an array\n");
            json_decref(root);
            return NULL;
        }

        // Retrieve file name
        json_t * fileNameJson = json_object_get(fileValueJson, "name");

        if (!fileNameJson) {
            fprintf(stderr, "error: The file name is not defined\n");
            json_decref(root);
            return NULL;
        }

        if(!json_is_string(fileNameJson))
        {
            fprintf(stderr, "error: The file name is not a string\n");
            json_decref(root);
            return NULL;
        }

        const char * fileName = json_string_value(fileNameJson);
        
        // Retrieve file extension
        json_t * fileExtJson = json_object_get(fileValueJson, "ext");

        if (!fileExtJson) {
            fprintf(stderr, "error: The file extension is not defined\n");
            json_decref(root);
            return NULL;
        }

        if(!json_is_string(fileExtJson))
        {
            fprintf(stderr, "error: The file extension is not a string\n");
            json_decref(root);
            return NULL;
        }

        const char * fileExt = json_string_value(fileExtJson);

        CodeGenFile_addFile2(codeGenFile, fileName, fileExt);
    }

    json_decref(root);

    return codeGenFile;
}

void CodeGenFile_addFile(CodeGenFile * codeGenFile, const char * filename)
{
    size_t length = strlen(filename);

    char * file_name = (char*)malloc(sizeof(char) * (length-1));
    char * file_ext  = (char*)malloc(sizeof(char) * 2);

    memset(file_name, 0, (length - 1));
    memset(file_ext, 0, 1);

    strncpy(file_name, filename, length - 2);
    strncpy(file_ext, filename + (length - 1), 1);

    file_name[length - 1] = '\0';
    file_ext[1] = '\0';

    int exists = CodeGenFile_fileExists(codeGenFile, file_name, file_ext);

    if (!exists) {
        codeGenFile->filesCount++;
        codeGenFile->files = (File**)realloc(codeGenFile->files, sizeof(File*) * codeGenFile->filesCount);
        codeGenFile->files[codeGenFile->filesCount - 1] = File_construct(file_name, file_ext);
    }
}

void CodeGenFile_addFile2(CodeGenFile * codeGenFile, const char * filename, const char * fileext)
{
    int exists = CodeGenFile_fileExists(codeGenFile, filename, fileext);

    if (!exists) {
        codeGenFile->filesCount++;
        codeGenFile->files = (File**)realloc(codeGenFile->files, sizeof(File*) * codeGenFile->filesCount);
        codeGenFile->files[codeGenFile->filesCount - 1] = File_construct(filename, fileext); 
    }   
}

int CodeGenFile_removeFile(CodeGenFile * codeGenFile, File * file)
{
    int index = 0;
    int rewrite = 0;

    for (int i = 0; i < codeGenFile->filesCount; i++) {

        if (strcmp(codeGenFile->files[i]->name, file->name) == 0 &&
            file->type == codeGenFile->files[i]->type )
        {
            index = 1;
            File_free(codeGenFile->files[i]);
            codeGenFile->files[i] = NULL;
            printf("file [%d] deleted\n", i);
        }
        else {
            codeGenFile->files[rewrite] = codeGenFile->files[i];
            rewrite++;
            printf("file [%d] kept\n", i);
        }
    }

    if (index) {
        codeGenFile->filesCount = codeGenFile->filesCount - 1;
        codeGenFile->files = (File**)realloc(codeGenFile->files, sizeof(File*) * codeGenFile->filesCount);
    }
    return index;
}


int CodeGenFile_fileExists(CodeGenFile * codeGenFile, const char * filename, const char * fileext)
{
    int type = strcmp(fileext, "c") == 0 ? 1 : 2;

    for (int i = 0; i < codeGenFile->filesCount; i++) {

        int cmpName = strcmp(codeGenFile->files[i]->name, filename) == 0;
        int cmpExt = type == codeGenFile->files[i]->type;

        if (cmpName && cmpExt)
            return 1;
    }

    return 0;
}

int CodeGenFile_export(CodeGenFile * codeGenFile, const char * filename)
{
    json_t * root = json_object();
    json_t * projectname = json_string(codeGenFile->projectName);
    json_t * files = json_array();

    json_object_set(root, "project-name", projectname);

    for (int i = 0; i < codeGenFile->filesCount; i++) {
        json_t * file = json_object();
        json_t * file_name_json = json_string(codeGenFile->files[i]->name);
        json_t * file_ext_json = json_string(codeGenFile->files[i]->type == 1 ? "c" : "h");
        json_object_set(file, "name", file_name_json);
        json_object_set(file, "ext", file_ext_json);
        json_array_append_new(files, file);
    }

    json_object_set(root, "files", files);

    json_dump_file(root, filename, JSON_INDENT(1));

    return 1;
}

/*int main() {
    CodeGenFile* codeGenFile = CodeGenFile_constructFromFile("test1.json");
    if (codeGenFile != NULL)
    {
        printf("project name : %s\n", codeGenFile->projectName);
        CodeGenFile_addFile(codeGenFile, "test/alpha.c");
        for (int i = 0; i < codeGenFile->filesCount; i++) {
            printf("%s.%s\n", codeGenFile->files[i]->name, codeGenFile->files[i]->type == 1 ? "c" : "h");
        }
        printf("number of file : %d\n", codeGenFile->filesCount);
        CodeGenFile_export(codeGenFile, "test1.json");
        CodeGenFile_free(codeGenFile);
    }
    return 0;
}*/