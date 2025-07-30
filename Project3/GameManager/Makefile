CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g -fPIC
LDFLAGS = -shared

# Include directories
INCLUDES = -I../common -I../include -I../UserCommon

# Library directories and libraries
LIBDIRS = -L../UserCommon
LIBS = -lUserCommon

# Source files
SOURCES = MyGameManager_Fixed.cpp GameManagerRegistration.cpp ../common/GameManagerRegistration.cpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Target shared library
TARGET = GameManager_123456789_987654321.so

# Default target
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBDIRS) $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all clean
