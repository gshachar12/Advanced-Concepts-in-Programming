CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g
# Windows doesn't need -ldl
ifeq ($(OS),Windows_NT)
    LDFLAGS = -pthread
else
    LDFLAGS = -ldl -pthread
endif

# Include directories
INCLUDES = -I../common -I../include

# Source files
SOURCES = main.cpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Target executable
TARGET = Simulator_123456789_987654321

# Default target
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all clean
