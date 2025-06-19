# Configuration
CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -Werror -pedantic -fPIC
INCLUDES = -I./include -I./src/algorithms -I./src/gameManager -I./common
BUILD_DIR = build
EXECUTABLE = tanks_game
SIMPLE_TEST_EXECUTABLE = simple_wall_test
WALL_COLLISION_TEST_EXECUTABLE = wall_collision_test
WALL_BOUNDARY_TEST_EXECUTABLE = wall_boundary_test

# Source files
SRC_DIR = src
ALGO_DIR = $(SRC_DIR)/algorithms
GAME_MANAGER_DIR = $(SRC_DIR)/gameManager
TEST_DIR = $(SRC_DIR)/tests

# Find all source files
SOURCES := $(wildcard $(ALGO_DIR)/*.cpp) $(wildcard $(GAME_MANAGER_DIR)/*.cpp) 
MAIN_SRC = $(SRC_DIR)/main.cpp
SIMPLE_TEST_SRC = $(TEST_DIR)/simple_wall_test.cpp
OBJ_FILES := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SOURCES))
MAIN_OBJ = $(BUILD_DIR)/$(SRC_DIR)/main.o
SIMPLE_TEST_OBJ = $(BUILD_DIR)/$(SIMPLE_TEST_SRC:.cpp=.o)

# Default target
all: prepare $(EXECUTABLE)

# Create build directory structure
prepare:
	@mkdir -p $(BUILD_DIR)/$(ALGO_DIR)
	@mkdir -p $(BUILD_DIR)/$(GAME_MANAGER_DIR)
	@mkdir -p $(BUILD_DIR)/$(SRC_DIR)
	@mkdir -p $(BUILD_DIR)/$(TEST_DIR)

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

# Wall tests
test_wall:
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(TEST_DIR)/wall_test.cpp -o $(SIMPLE_TEST_EXECUTABLE)
	./$(SIMPLE_TEST_EXECUTABLE)

# Wall collision test
test_wall_collision:
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(TEST_DIR)/wall_collision_test.cpp -o $(WALL_COLLISION_TEST_EXECUTABLE)
	./$(WALL_COLLISION_TEST_EXECUTABLE)

# Wall boundary test
test_wall_boundary:
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(TEST_DIR)/wall_boundary_test.cpp -o $(WALL_BOUNDARY_TEST_EXECUTABLE)
	./$(WALL_BOUNDARY_TEST_EXECUTABLE)

# Run the wall transformation test on the test map
test_wall_transformation: all
	./$(EXECUTABLE) -g inputs/wall_transformation_test.txt

# Run all tests
test_all: test_wall test_wall_collision test_wall_boundary
	@echo "All tests completed"

.PHONY: all clean prepare lib run test_wall test_wall_collision test_wall_boundary test_wall_transformation test_all