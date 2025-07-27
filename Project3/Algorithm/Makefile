CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g -fPIC
LDFLAGS = -shared

# Include directories
INCLUDES = -I../common -I../include

# Source files for simple algorithm
SIMPLE_SOURCES = SimpleAlgorithm.cpp SimplePlayer.cpp AlgorithmRegistration.cpp
SIMPLE_OBJECTS = $(SIMPLE_SOURCES:.cpp=.o)
SIMPLE_TARGET = Algorithm_Simple_123456789_987654321.so

# Source files for BFS algorithm (if they exist)
BFS_SOURCES = BfsAlgorithm.cpp BfsPlayer.cpp
BFS_OBJECTS = $(BFS_SOURCES:.cpp=.o)
BFS_TARGET = Algorithm_BFS_123456789_987654321.so

# Default target
all: $(SIMPLE_TARGET)

$(SIMPLE_TARGET): $(SIMPLE_OBJECTS)
	$(CXX) $(LDFLAGS) -o $@ $^

$(BFS_TARGET): $(BFS_OBJECTS) AlgorithmRegistration.o
	$(CXX) $(LDFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f *.o *.so

.PHONY: all clean
