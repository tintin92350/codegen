#/usr/bash

wget http://www.digip.org/jansson/releases/jansson-2.12.tar.bz2
bunzip2 -c jansson-2.12.tar.bz2 | tar xf -
cd jansson-2.12

./configure
make
make check
make install

cd ../

echo "jansson library dependency installed !"

gcc cli.c codegen.project.c FileSystem.c file.c string.c -ljansson -g -o codegen
cp codegen /usr/bin

echo "codegen installed !"