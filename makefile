# Variable used to build the target
OBJ_OUTPUT_DIR=bin/obj
EXE_OUTPUT_DIR=bin

# Create output directories
$(shell mkdir -p $(EXE_OUTPUT_DIR))
$(shell mkdir -p $(OBJ_OUTPUT_DIR))

# Build stage for main entry point
codegen-cli: main.o
	gcc -o $(EXE_OUTPUT_DIR)/codegen-cli $(OBJ_OUTPUT_DIR)/main.o -ljansson -std=c17

# Build stages for each file
main.o: src/codegen.c
	gcc -c src/codegen.c -o $(OBJ_OUTPUT_DIR)/main.o -std=c17

# Clean stage
clean:
	rm -r $(OBJ_OUTPUT_DIR)/
	rm -r $(EXE_OUTPUT_DIR)/

# Clear stage
clear:
	rm -r $(OBJ_OUTPUT_DIR)/