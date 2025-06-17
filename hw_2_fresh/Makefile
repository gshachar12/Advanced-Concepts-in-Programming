# Configuration
CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -Werror -pedantic -fPIC
INCLUDES = -I./include -I./src/algo -I./src/game_manager -I./common
BUILD_DIR = build
EXECUTABLE = tanks_game

# Source files
SRC_DIR = src
ALGO_DIR = $(SRC_DIR)/algo
GAME_MANAGER_DIR = $(SRC_DIR)/game_manager

# Find all source files
SOURCES := $(wildcard $(ALGO_DIR)/*.cpp) $(wildcard $(GAME_MANAGER_DIR)/*.cpp) 
MAIN_SRC = $(SRC_DIR)/main.cpp
OBJ_FILES := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SOURCES))
MAIN_OBJ = $(BUILD_DIR)/$(SRC_DIR)/main.o

# Default target
all: prepare $(EXECUTABLE)

# Create build directory structure
prepare:
	@mkdir -p $(BUILD_DIR)/$(ALGO_DIR)
	@mkdir -p $(BUILD_DIR)/$(GAME_MANAGER_DIR)
	@mkdir -p $(BUILD_DIR)/$(SRC_DIR)

# Compile main executable
$(EXECUTABLE): $(OBJ_FILES) $(MAIN_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Compile source files
$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Create a shared library (equivalent to tanks_game_lib in CMake)
lib: $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) -shared $^ -o $(BUILD_DIR)/libtanks_game.dylib

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR)
	rm -f $(EXECUTABLE)
	rm -f output.txt errors.txt input_errors.txt

# Run target
run: all
	./$(EXECUTABLE) -g input/input_b.txt

.PHONY: all clean prepare lib run