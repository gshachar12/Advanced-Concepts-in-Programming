CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g -fPIC
LDFLAGS = -shared

# Include directories
INCLUDES = -I../../include -I../../common -I../../UserCommon

# Source files
SOURCES = SimpleTankAlgorithm.cpp SimplePlayer.cpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Default target
all: $(OBJECTS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJECTS)

.PHONY: all clean
