# Compiler settings
CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic -std=c++17

# Linker flags for SFML
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

# Find all .cpp files in src/ and subdirectories, excluding problematic ones
SRCS := $(filter-out src/Controller.cpp, $(wildcard src/*.cpp)) $(wildcard src/algorithms/*.cpp) $(wildcard src/api/*.cpp)
OBJS := $(SRCS:.cpp=.o)

# Final executable name
TARGET = tank_game

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

# Run target that expects board file as ARGS
run: $(TARGET)
	./$(TARGET) $(ARGS)

# Run with visual mode
run_visual: $(TARGET)
	./$(TARGET) $(ARGS) --visual

.PHONY: all clean run run_visual
