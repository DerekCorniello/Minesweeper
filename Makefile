# Makefile for Minesweeper project

# Compiler
CC = clang

# Compiler flags
CFLAGS = -Wall -g -fsanitize=address

# Target executable
TARGET = ms.exe

# Source files
SRCS = minesweeper.c

# Object files
OBJS = $(SRCS:.c=.o)

# Rule to build the target
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Rule to build object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build artifacts
.PHONY: clean
clean:
	del $(TARGET) $(OBJS) *.ilk *.pdb *.obj *.lib *.exp
