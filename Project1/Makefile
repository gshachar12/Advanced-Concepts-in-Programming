# Compiler settings
CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -pedantic -std=c++17

# Find all .cpp files in src/
SRCS := $(wildcard src/*.cpp) $(wildcard src/algorithms/*.cpp)
OBJS := $(SRCS:.cpp=.o)

# Test sources and objects
TEST_SRCS := $(wildcard tests/*.cpp) $(wildcard tests/unit/*.cpp) $(wildcard tests/integration/*.cpp) $(wildcard tests/system/*.cpp)
TEST_OBJS := $(TEST_SRCS:.cpp=.o)
TEST_BINS := $(patsubst tests/%.cpp, tests/%, $(TEST_SRCS))

# Final executable name
TARGET = tank_game

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Test targets
tests: $(TEST_BINS)

tests/%: tests/%.o $(filter-out src/main.o, $(OBJS))
	$(CXX) $(CXXFLAGS) -o $@ $^

run_tests: tests
	./tests/test_runner

clean:
	rm -f $(OBJS) $(TARGET) $(TEST_OBJS) $(TEST_BINS)

# Run target that expects board file as ARGS
run: $(TARGET)
	./$(TARGET) $(ARGS)

# Additional target for visual mode:
run_visual:
	./$(TARGET) $(ARGS) --visual

.PHONY: all clean run run_visual tests run_tests
