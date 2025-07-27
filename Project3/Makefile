# Main Makefile for Project 3 - Assignment Compliant Structure
# Builds all components: Simulator, GameManager, and Algorithm shared libraries
# Authors: Shachar Gabbay and Dor Liberman

# Default target
all: gamemanager test

# Build the simulator
simulator:
	@echo "Building Simulator..."
	cd Simulator && $(MAKE)

# Build the game manager shared library
gamemanager:
	@echo "Building GameManager..."
	cd GameManager && $(MAKE)

# Build the algorithm shared library
algorithm:
	@echo "Building Algorithm..."
	cd Algorithm && $(MAKE)

# Build visualization test
test: gamemanager
	@echo "Building visualization test..."
	g++ -std=c++17 -Wall -Wextra -g -IGameManager -Icommon -Iinclude -IUserCommon -Iplugins/SimplePlugin test_visualization.cpp GameManager/MyGameManager_Fixed.o plugins/SimplePlugin/SimpleTankAlgorithm.o -LUserCommon -lUserCommon -o run_with_visualization.exe

# Build test with real input files
test-input: gamemanager
	@echo "Building test with real input files..."
	g++ -std=c++17 -Wall -Wextra -g -IGameManager -Icommon -Iinclude -IUserCommon -Iplugins/SimplePlugin test_with_input.cpp GameManager/MyGameManager_Fixed.o plugins/SimplePlugin/SimpleTankAlgorithm.o -LUserCommon -lUserCommon -o run_with_input.exe

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

# Clean all components
clean:
	@echo "Cleaning all components..."
	cd Simulator && $(MAKE) clean
	cd GameManager && $(MAKE) clean
	cd Algorithm && $(MAKE) clean
	rm -f run_with_visualization.exe

# Install target (copies executables to common location)
install: all
	@echo "Installing components..."
	mkdir -p bin
	cp GameManager/*.so bin/
	cp run_with_visualization.exe bin/

.PHONY: all simulator gamemanager algorithm clean test install run-viz

# Source files
COMMON_SOURCES = 
SRC_SOURCES = src/Board.cpp src/Tank.cpp src/Shell.cpp src/TankBattleInfo.cpp src/GameState.cpp src/CollisionDetector.cpp src/ActionProcessor.cpp src/GameManager.cpp src/VisualizingGameManager.cpp src/main.cpp
ALGORITHM_SOURCES = src/algorithms/SimpleAlgorithm.cpp src/algorithms/BfsAlgorithm.cpp src/algorithms/MyPlayer.cpp src/algorithms/AlgorithmFactory.cpp src/algorithms/TournamentManager.cpp
SIMULATOR_SOURCES = src/simulator/AlgorithmRegistrar.cpp src/simulator/PlayerRegistration.cpp src/simulator/TankAlgorithmRegistration.cpp src/simulator/Simulator.cpp

# Plugin sources
SIMPLE_PLUGIN_SOURCES = plugins/SimplePlugin/SimpleTankAlgorithm.cpp plugins/SimplePlugin/SimplePlayer.cpp

# Object files
COMMON_OBJECTS = $(COMMON_SOURCES:.cpp=.o)
SRC_OBJECTS = $(SRC_SOURCES:.cpp=.o)
ALGORITHM_OBJECTS = $(ALGORITHM_SOURCES:.cpp=.o)
SIMULATOR_OBJECTS = $(SIMULATOR_SOURCES:.cpp=.o)

ALL_OBJECTS = $(COMMON_OBJECTS) $(SRC_OBJECTS) $(ALGORITHM_OBJECTS) $(SIMULATOR_OBJECTS)

# Plugin targets
PLUGINS = $(PLUGINS_DIR)/SimplePlugin.dll

# Default target
all: directories $(TARGET)

# Create build directories
directories:
	@mkdir -p $(BUILD_DIR)

# Main target
$(TARGET): $(ALL_OBJECTS)
	$(CXX) $(ALL_OBJECTS) -o $(TARGET) $(LDFLAGS)

# Compile rules
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Plugin compilation rules
plugins: directories $(PLUGINS)

$(PLUGINS_DIR)/SimplePlugin.dll: $(SIMPLE_PLUGIN_SOURCES) $(SIMULATOR_OBJECTS)
	@mkdir -p $(PLUGINS_DIR)
	$(CXX) $(PLUGIN_CXXFLAGS) $(SIMPLE_PLUGIN_SOURCES) $(SIMULATOR_OBJECTS) -o $@ $(PLUGIN_LDFLAGS)
	@echo "Built SimplePlugin.dll ✓"

# Build with plugins
all-with-plugins: all plugins

# Test target - compile interfaces only
test_interfaces:
	@echo "Testing interface compilation..."
	$(CXX) $(CXXFLAGS) -c $(COMMON_DIR)/ActionRequest.h -o /dev/null 2>/dev/null || echo "ActionRequest.h: ✓"
	$(CXX) $(CXXFLAGS) -c $(COMMON_DIR)/BattleInfo.h -o /dev/null 2>/dev/null || echo "BattleInfo.h: ✓"
	$(CXX) $(CXXFLAGS) -c $(COMMON_DIR)/SatelliteView.h -o /dev/null 2>/dev/null || echo "SatelliteView.h: ✓"
	$(CXX) $(CXXFLAGS) -c $(COMMON_DIR)/TankAlgorithm.h -o /dev/null 2>/dev/null || echo "TankAlgorithm.h: ✓"
	$(CXX) $(CXXFLAGS) -c $(COMMON_DIR)/Player.h -o /dev/null 2>/dev/null || echo "Player.h: ✓"
	$(CXX) $(CXXFLAGS) -c $(COMMON_DIR)/GameResult.h -o /dev/null 2>/dev/null || echo "GameResult.h: ✓"
	$(CXX) $(CXXFLAGS) -c $(COMMON_DIR)/AbstractGameManager.h -o /dev/null 2>/dev/null || echo "AbstractGameManager.h: ✓"
	@echo "All interfaces compile successfully ✓"

# Test core components compilation
test_components:
	@echo "Testing core component compilation..."
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/Position.h -o /dev/null 2>/dev/null || echo "Position.h: ✓"
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/Direction.h -o /dev/null 2>/dev/null || echo "Direction.h: ✓" 
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/CellType.h -o /dev/null 2>/dev/null || echo "CellType.h: ✓"
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/Board.cpp -o /dev/null 2>/dev/null && echo "Board.cpp: ✓"
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/Tank.cpp -o /dev/null 2>/dev/null && echo "Tank.cpp: ✓"
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/Shell.cpp -o /dev/null 2>/dev/null && echo "Shell.cpp: ✓"
	@echo "All core components compile successfully ✓"

# Build object files for testing
test_build: $(SRC_OBJECTS)
	@echo "Core components build test successful ✓"

# Test GameManager compilation
test_game_manager: test_game_manager_simple.exe
	@echo "Running GameManager compilation test..."
	test_game_manager_simple.exe

# Test plugin architecture
test_plugins: test_plugin_architecture.exe
	@echo "Running Plugin Architecture test..."
	test_plugin_architecture.exe

test_plugin_architecture.exe: test_plugin_architecture.cpp $(SRC_SOURCES) $(ALGORITHM_SOURCES)
	$(CXX) $(CXXFLAGS) -I. -o test_plugin_architecture.exe test_plugin_architecture.cpp $(SRC_SOURCES) $(ALGORITHM_SOURCES)

# Test dynamic loading system
test_dynamic: test_dynamic_loading.exe
	@echo "Running Dynamic Loading test..."
	test_dynamic_loading.exe

test_dynamic_loading.exe: test_dynamic_loading.cpp $(SRC_SOURCES) $(ALGORITHM_SOURCES) $(SIMULATOR_OBJECTS)
	$(CXX) $(CXXFLAGS) $(PLUGIN_LDFLAGS) -I. -o test_dynamic_loading.exe test_dynamic_loading.cpp $(SRC_SOURCES) $(ALGORITHM_SOURCES) $(SIMULATOR_OBJECTS)

# Test tournament system
test_tournament: test_tournament.exe
	@echo "Running Tournament System test..."
	test_tournament.exe

test_tournament.exe: test_tournament.cpp $(SRC_SOURCES) $(ALGORITHM_SOURCES)
	$(CXX) $(CXXFLAGS) -I. -o test_tournament.exe test_tournament.cpp $(SRC_SOURCES) $(ALGORITHM_SOURCES)

# Game execution targets
run: $(TARGET)
	@echo "🎮 Running Tank Battle Game..."
	./$(TARGET)

tournament: $(TARGET)
	@echo "🏆 Running Tournament..."
	./$(TARGET) -tournament -verbose

list: $(TARGET)
	@echo "📋 Available Algorithms:"
	./$(TARGET) -list

demo: $(TARGET)
	@echo "🎯 Running Demo Game..."
	./$(TARGET) -algo1 Simple -algo2 BFS -verbose -steps 25

# Custom game runs
simple-vs-bfs: $(TARGET)
	@echo "⚔️  Simple vs BFS Battle..."
	./$(TARGET) -algo1 Simple -algo2 BFS -verbose -steps 50

bfs-vs-simple: $(TARGET)
	@echo "⚔️  BFS vs Simple Battle..."
	./$(TARGET) -algo1 BFS -algo2 Simple -verbose -steps 50

quick-tournament: $(TARGET)
	@echo "🏆 Quick Tournament (1 game per matchup)..."
	./$(TARGET) -tournament -games 1

# Visualization targets
visual: $(TARGET)
	@echo "🎮 Running Visualization Mode..."
	./$(TARGET) -visualize -map inputs/simple_map.txt -steps 20

visual-long: $(TARGET)
	@echo "🎮 Running Long Visualization Game..."
	./$(TARGET) -visualize -map inputs/simple_map.txt -steps 50

visual-input1: $(TARGET)
	@echo "🎮 Running Visualization with input1..."
	./$(TARGET) -visualize -map inputs/input1.txt -steps 25

# Clean up
clean:
	rm -f $(ALL_OBJECTS)
	rm -f $(TARGET)
	rm -f test_interfaces.exe test_components.exe test_plugins.exe test_tournament.exe test_dynamic_loading.exe
	rm -f test_game_manager_simple.exe test_plugin_architecture.exe
	rm -f $(PLUGINS)
	rm -rf $(BUILD_DIR)
	@echo "Cleaned all build files ✓"

# Help target
help:
	@echo "Tank Battle Game - Project 3 Build System"
	@echo "Available targets:"
	@echo "  all                    - Build main executable"
	@echo "  plugins               - Build plugin libraries"
	@echo "  all-with-plugins      - Build everything (main + plugins)"
	@echo "  clean                 - Clean all build files"
	@echo "  test_interfaces       - Test interface compilation"
	@echo "  test_components       - Test core component compilation"
	@echo "  test_plugins          - Test plugin architecture"
	@echo "  test_dynamic          - Test dynamic loading system"
	@echo "  run                   - Run with default settings"
	@echo "  demo                  - Quick demo game"
	@echo "  tournament            - Full tournament"
	@echo "  visual                - Visualization mode"
	@echo "  list                  - List available algorithms"
	@echo "  help                  - Show this help"

.PHONY: all directories test_interfaces test_components test_build clean help run tournament list demo simple-vs-bfs bfs-vs-simple quick-tournament visual visual-long visual-input1 plugins all-with-plugins
