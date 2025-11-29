# Makefile for Logger Module Project

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -Iinclude
LDFLAGS = 

# Directories
SRC_DIR = src
INC_DIR = include
BUILD_DIR = build
LOG_DIR = logs

# Files
SOURCES = $(SRC_DIR)/logger.c $(SRC_DIR)/main.c
OBJECTS = $(BUILD_DIR)/logger.o $(BUILD_DIR)/main.o
TARGET = $(BUILD_DIR)/logger_demo

# Default target
all: directories $(TARGET)

# Create necessary directories
directories:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(LOG_DIR)

# Build the executable
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)
	@echo "Build complete! Executable: $(TARGET)"

# Compile logger.c
$(BUILD_DIR)/logger.o: $(SRC_DIR)/logger.c $(INC_DIR)/logger.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/logger.c -o $(BUILD_DIR)/logger.o

# Compile main.c
$(BUILD_DIR)/main.o: $(SRC_DIR)/main.c $(INC_DIR)/logger.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/main.c -o $(BUILD_DIR)/main.o

# Run the program
run: all
	@echo "Running logger demo..."
	@./$(TARGET)

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR)/*.o $(TARGET)
	@echo "Cleaned build files"

# Clean everything including logs
distclean: clean
	rm -rf $(LOG_DIR)/*.log
	@echo "Cleaned all files including logs"

# Install (optional - for system-wide installation)
install: all
	@echo "Installation not implemented. Copy manually if needed."

# Help target
help:
	@echo "Available targets:"
	@echo "  make          - Build the project"
	@echo "  make run      - Build and run the demo"
	@echo "  make clean    - Remove build files"
	@echo "  make distclean- Remove build files and logs"
	@echo "  make help     - Show this help message"

.PHONY: all directories run clean distclean install help
