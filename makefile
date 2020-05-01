# Variable used to build the target
OBJ_OUTPUT_DIR=obj/
EXE_OUTPUT_DIR=build/

# Create output directories
$(shell mkdir -p $(OBJ_OUTPUT_DIR))
$(shell mkdir -p $(EXE_OUTPUT_DIR))

# Build stage for main entry point
codegen-cli: main.o cli.o codegen.o file.o fs.o string.o
	gcc -o build/codegen-cli obj/main.o obj/cli.o obj/codegen.o obj/file.o obj/fs.o obj/string.o -ljansson

# Build stages for each file
main.o: main.c
	gcc -c main.c -o obj/main.o
	
cli.o: cli.c
	gcc -c cli.c -o obj/cli.o
	
codegen.o: codegen.project.c
	gcc -c codegen.project.c -o obj/codegen.o
	
file.o: file.c
	gcc -c file.c -o obj/file.o
	
fs.o: FileSystem.c
	gcc -c FileSystem.c -o obj/fs.o
	
string.o: string.c
	gcc -c string.c -o obj/string.o

# Clean stage
clean:
	rm -r obj/
	rm -r build/

# Clear stage
clear:
	rm -r obj/