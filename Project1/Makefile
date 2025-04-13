# Compiler settings
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

# Find all .cpp files in src/
SRCS := $(wildcard src/*.cpp) $(wildcard src/algorithms/*.cpp)
OBJS := $(SRCS:.cpp=.o)

# Final executable name
TARGET = tank_game

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

# Run target that expects board file as ARGS
run: $(TARGET)
	./$(TARGET) $(ARGS)

# Additional target for visual mode:
run_visual:
	./$(TARGET) $(ARGS) --visual
