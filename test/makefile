# Variable used to build the target
OBJ_OUTPUT_DIR=build/obj
EXE_OUTPUT_DIR=build

# Create output directories
$(shell mkdir -p $(OBJ_OUTPUT_DIR))
$(shell mkdir -p $(EXE_OUTPUT_DIR))

test : test.o 
	 gcc -o $(EXE_OUTPUT_DIR)/test $(OBJ_OUTPUT_DIR)/test.o 

test.o : test.c
	gcc -c test.c -o $(OBJ_OUTPUT_DIR)/test.o

# Clean stage
clean:
	rm -r obj/
	rm -r build/

# Clear stage
clear:
	rm -r obj/
