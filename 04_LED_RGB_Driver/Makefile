# Makefile for LED RGB Driver Project

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -Iinclude -g
LDFLAGS = 

# Directories
SRC_DIR = src
INC_DIR = include
BUILD_DIR = build

# Files
SOURCES = $(SRC_DIR)/led_driver.c $(SRC_DIR)/main.c
OBJECTS = $(BUILD_DIR)/led_driver.o $(BUILD_DIR)/main.o
TARGET = $(BUILD_DIR)/led_test

# Default target
all: directories $(TARGET)

# Create necessary directories
directories:
	@mkdir -p $(BUILD_DIR)

# Build the executable
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)
	@echo "Build complete! Executable: $(TARGET)"

# Compile led_driver.c
$(BUILD_DIR)/led_driver.o: $(SRC_DIR)/led_driver.c $(INC_DIR)/led_driver.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/led_driver.c -o $(BUILD_DIR)/led_driver.o

# Compile main.c
$(BUILD_DIR)/main.o: $(SRC_DIR)/main.c $(INC_DIR)/led_driver.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/main.c -o $(BUILD_DIR)/main.o

# Run the program
run: all
	@echo "Running LED RGB Driver tests..."
	@echo ""
	@./$(TARGET)

# Run with valgrind for memory leak detection
valgrind: all
	@echo "Running with Valgrind..."
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TARGET)

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR)/*.o $(TARGET)
	@echo "Cleaned build files"

# Clean everything
distclean: clean
	rm -rf $(BUILD_DIR)
	@echo "Cleaned all files"

# Help target
help:
	@echo "Available targets:"
	@echo "  make          - Build the project"
	@echo "  make run      - Build and run the test suite"
	@echo "  make valgrind - Run with memory leak detection"
	@echo "  make clean    - Remove build files"
	@echo "  make distclean- Remove all build artifacts"
	@echo "  make help     - Show this help message"

.PHONY: all directories run valgrind clean distclean help
