# Define the C compiler to use (gcc)
CC = gcc

# Compiler flags: -Wall enables all warnings, -Iinclude adds the include directory to header search path
CFLAGS = -Wall -Iinclude

# List of source files to compile
SRC = src/supercommand.c src/file_operations.c src/directory_operations.c src/keylogger.c

# Generate object file names by replacing .c with .o in source file names
OBJ = $(SRC:.c=.o)

# Name of the final executable
OUT = supercommand

# Default target that will be built when running just 'make'
all: $(OUT)

# Rule to link object files into the final executable
# $@ represents the target name (supercommand)
# $(OBJ) lists all the object files to be linked
$(OUT): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

# Pattern rule for compiling source files into object files
# $< represents the first dependency (the .c file)
# -o $@ specifies the output file name
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Target to clean up compiled files
# Removes all object files and the executable
clean:
	rm -f $(OBJ) $(OUT)
