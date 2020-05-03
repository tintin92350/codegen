# Variable used to build the target
OBJ_OUTPUT_DIR=bin/obj
EXE_OUTPUT_DIR=bin

# Create output directories
$(shell mkdir -p $(EXE_OUTPUT_DIR))
$(shell mkdir -p $(OBJ_OUTPUT_DIR))

# Build stage for main entry point
codegen-cli: main.o cli.o codegen.o file.o fs.o string.o vector.o
	gcc -o $(EXE_OUTPUT_DIR)/codegen-cli $(OBJ_OUTPUT_DIR)/main.o $(OBJ_OUTPUT_DIR)/cli.o $(OBJ_OUTPUT_DIR)/codegen.o $(OBJ_OUTPUT_DIR)/file.o $(OBJ_OUTPUT_DIR)/fs.o $(OBJ_OUTPUT_DIR)/string.o $(OBJ_OUTPUT_DIR)/utils_vector.o -ljansson

# Build stages for each file
main.o: main.c
	gcc -c main.c -o $(OBJ_OUTPUT_DIR)/main.o
	
cli.o: cli.c
	gcc -c cli.c -o $(OBJ_OUTPUT_DIR)/cli.o
	
codegen.o: codegen.project.c
	gcc -c codegen.project.c -o $(OBJ_OUTPUT_DIR)/codegen.o
	
file.o: file.c
	gcc -c file.c -o $(OBJ_OUTPUT_DIR)/file.o
	
fs.o: FileSystem.c
	gcc -c FileSystem.c -o $(OBJ_OUTPUT_DIR)/fs.o
	
string.o: string.c
	gcc -c string.c -o $(OBJ_OUTPUT_DIR)/string.o

	
vector.o: utils/vector.c
	gcc -c utils/vector.c -o $(OBJ_OUTPUT_DIR)/utils_vector.o

# Clean stage
clean:
	rm -r $(OBJ_OUTPUT_DIR)/
	rm -r $(EXE_OUTPUT_DIR)/

# Clear stage
clear:
	rm -r $(OBJ_OUTPUT_DIR)/