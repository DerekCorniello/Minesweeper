# Makefile for Minesweeper project

# Compiler
CC = clang

# Compiler flags
CFLAGS = -Wall -g

# Target executable
TARGET = ms

# Source files
SRCS = minesweeper.c

# Rule to build the target
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

# Clean up build artifacts
clean:
	rm -f *.exe *.ilk *.pdb *.o *.obj *.lib
