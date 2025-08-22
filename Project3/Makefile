# Main Makefile for Project 3 - Assignment Compliant Structure
# Builds all components: Simulator, GameManager, and Algorithm shared libraries
# Authors: Shachar Gabbay and Dor Liberman

# Default target
all: gamemanager test

# Build UserCommon library first
usercommon:
	@echo "Building UserCommon library..."
	cd UserCommon && $(MAKE)

# Build the simulator
simulator:
	@echo "Building Simulator..."
	cd Simulator && $(MAKE)

# Build the game manager shared library
gamemanager: usercommon
	@echo "Building GameManager..."
	cd GameManager && $(MAKE)

# Build the algorithm shared library
algorithm:
	@echo "Building Algorithm..."
	cd Algorithm && $(MAKE)

# Build plugins
plugins:
	@echo "Building plugins..."
	cd plugins/SimplePlugin && $(MAKE)

# Build visualization test
test: gamemanager plugins
	@echo "Building visualization test..."
	cp UserCommon/libUserCommon.so .
	g++ -std=c++17 -Wall -Wextra -g -IGameManager -Icommon -Iinclude -IUserCommon -Iplugins/SimplePlugin test_visualization.cpp GameManager/MyGameManager_Fixed.o plugins/SimplePlugin/SimpleTankAlgorithm.o -L. -lUserCommon -o run_with_visualization.exe

# Build test with real input files
test-input: gamemanager plugins
	@echo "Building test with real input files..."
	cp UserCommon/libUserCommon.so .
	g++ -std=c++17 -Wall -Wextra -g -IGameManager -Icommon -Iinclude -IUserCommon -Iplugins/SimplePlugin test_with_input.cpp GameManager/MyGameManager_Fixed.o plugins/SimplePlugin/SimpleTankAlgorithm.o -L. -lUserCommon -o run_with_input.exe

# Run the game with visualization using mock data
run-viz: test
	@echo ""
	@echo "🎮 RUNNING GAME WITH VISUALIZATION..."
	@echo "======================================="
	@./run_with_visualization.exe

# Run the game with real input and algorithms
run-game: test-input
	@echo ""
	@echo "🎮 RUNNING REAL GAME WITH INPUT1..."
	@echo "==================================="
	@./run_with_input.exe inputs/input1.txt

# Run the game with different input files
run-input1: test-input
	@./run_with_input.exe inputs/input1.txt

run-input2: test-input  
	@./run_with_input.exe inputs/input2.txt

run-input3: test-input
	@./run_with_input.exe inputs/input3.txt

# Run visualization with real input files
run-viz-input1: test-input
	@echo ""
	@echo "🎮 RUNNING VISUALIZATION WITH INPUT1..."
	@echo "======================================="
	@./run_with_input.exe inputs/input1.txt

run-viz-input2: test-input
	@echo ""
	@echo "🎮 RUNNING VISUALIZATION WITH INPUT2..."
	@echo "======================================="
	@./run_with_input.exe inputs/input2.txt

run-viz-input3: test-input
	@echo ""
	@echo "🎮 RUNNING VISUALIZATION WITH INPUT3..."
	@echo "======================================="
	@./run_with_input.exe inputs/input3.txt

run-viz-simple: test-input
	@echo ""
	@echo "🎮 RUNNING VISUALIZATION WITH SIMPLE MAP..."
	@echo "==========================================="
	@./run_with_input.exe inputs/simple_map.txt

# Clean all components
clean:
	@echo "Cleaning all components..."
	cd UserCommon && $(MAKE) clean
	cd Simulator && $(MAKE) clean
	cd GameManager && $(MAKE) clean
	cd Algorithm && $(MAKE) clean
	rm -f run_with_visualization.exe
	rm -f libUserCommon.so

# Install target (copies executables to common location)
install: all
	@echo "Installing components..."
	mkdir -p bin
	cp GameManager/*.so bin/
	cp run_with_visualization.exe bin/

.PHONY: all simulator gamemanager algorithm usercommon plugins clean test install run-viz run-viz-input1 run-viz-input2 run-viz-input3 run-viz-simple
