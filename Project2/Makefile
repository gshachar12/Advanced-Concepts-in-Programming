```makefile
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g -I./src
LDFLAGS =

# Directories
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin
MAPS_DIR = maps
RESOURCES_DIR = resources

# Target executable
TARGET = $(BIN_DIR)/tanks_game

# Source files (automatically finds all .cpp files in SRC_DIR and its subdirectories)
SRCS = $(shell find $(SRC_DIR) -name "*.cpp")
OBJS = $(SRCS:%.cpp=$(BUILD_DIR)/%.o)
DEPS = $(OBJS:.o=.d)

# Default target
all: $(TARGET)

# Create directories
$(BUILD_DIR)/$(SRC_DIR)/algorithms $(BUILD_DIR)/$(SRC_DIR)/common $(BIN_DIR) $(BUILD_DIR)/$(MAPS_DIR) $(RESOURCES_DIR):
	mkdir -p $@

# Compile source files
$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)/$(SRC_DIR)/algorithms $(BUILD_DIR)/$(SRC_DIR)/common
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@ -MMD -MP

# Link the executable
$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# Clean up
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

# Clean and rebuild
rebuild: clean all

# Run the executable with the specified map file
run: $(TARGET)
	$(TARGET) $(RESOURCES_DIR)/maps/map1.txt

# Include dependencies
-include $(DEPS)

.PHONY: all clean rebuild run
```