#/usr/bash

gcc cli.c codegen.project.c FileSystem.c file.c string.c -ljansson -g -o codegen
cp codegen /usr/bin

echo "codegen updated !"