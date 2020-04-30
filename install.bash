#/user/bash

gcc main.c codegen.project.c file.c -ljansson -Wall -o codegen
cp codegen /usr/bin