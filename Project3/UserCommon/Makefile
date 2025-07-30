CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g -fPIC
LDFLAGS = -shared

# Include directories
INCLUDES = -I../include -I../common

# Source files
SOURCES = UserCommonUtils.cpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Target shared library
TARGET = libUserCommon.so

# Default target
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(LDFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all clean
