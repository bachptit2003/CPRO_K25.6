# Makefile for Task Queue & Activity Log System

# Compiler và flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic

# Tên chương trình
TARGET = task_manager

# Source files
SRCS = main.c task_queue.c activity_log.c

# Object files
OBJS = $(SRCS:.c=.o)

# Headers
HEADERS = task_queue.h activity_log.h

# ======================== TARGETS ========================

# Default target
all: $(TARGET)

# Link object files
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compile source files
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -f $(OBJS) $(TARGET)

# Rebuild
rebuild: clean all

# Run the program
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean rebuild run
