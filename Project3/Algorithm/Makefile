CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g -fPIC
LDFLAGS = -shared

# Include directories
INCLUDES = -I../common -I../include

# Simplified algorithm source files (only TankAlgorithm for now)
ALL_SOURCES = SimpleAlgorithm.cpp AlgorithmRegistration.cpp ../common/TankAlgorithmRegistration.cpp
ALL_OBJECTS = $(ALL_SOURCES:.cpp=.o)

# Unified target as required by assignment
TARGET = Algorithm_123456789_987654321.so

# Default target
all: $(TARGET)

$(TARGET): $(ALL_OBJECTS)
	$(CXX) $(LDFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f *.o *.so

.PHONY: all clean
