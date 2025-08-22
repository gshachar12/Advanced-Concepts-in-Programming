# Tank Battle Game - Project 3

**Authors:** Shachar Gabbay and Dor Liberman

## 🎮 Overview

This is a complete tank battle simulation game built from scratch with a sophisticated plugin architecture. Tanks battle on grid-based maps using different AI algorithms, with support for tournaments, custom maps, and extensible algorithm plugins.

## 🏗️ Architecture

### Phase 1: Foundation Setup ✅
- Core interfaces and base classes
- Position, Direction, and CellType systems
- Memory management with smart pointers

### Phase 2: Core Game Components ✅  
- Board with SatelliteView integration
- Tank system with health, ammo, and direction
- Shell physics and movement
- Battle info and game state management

### Phase 3: Game Management System ✅
- Collision detection and response
- Action processing and validation
- Turn-based game loop execution

### Phase 4: Complete Game Manager ✅
- Full game orchestration
- Win condition checking
- Player and algorithm integration
- Comprehensive game state management

### Phase 5: Plugin Architecture ✅
- Dynamic algorithm registration system
- Factory pattern implementation
- Tournament management system
- Multiple algorithm implementations (Simple, BFS, Random)
- Comprehensive plugin testing framework

## 🚀 Quick Start

### Building and Running

```bash
# Build the game
mingw32-make all

# Quick demo
mingw32-make demo

# List available algorithms  
mingw32-make list

# Run tournament
mingw32-make tournament
```

### Using the Run Script

```bash
# Quick demo
run.bat demo

# Full tournament
run.bat tournament

# Custom game with specific map and algorithms
run.bat custom -map inputs\maze_map.txt -algo1 BFS -algo2 Simple -verbose -steps 50
```

## 🎯 Game Usage

### Command Line Options

```
tanks_game.exe [options]

Options:
  -map <file>         Map file to load (default: creates default map)
  -steps <num>        Maximum steps per game (default: 100)  
  -shells <num>       Shells per tank (default: 5)
  -algo1 <name>       Algorithm for player 1 (default: Simple)
  -algo2 <name>       Algorithm for player 2 (default: BFS)
  -verbose            Enable verbose output
  -tournament         Run tournament mode
  -games <num>        Games per tournament matchup (default: 3)
  -list               List available algorithms
  -help               Show help message
```

### Example Commands

```bash
# Basic game with default settings
tanks_game.exe

# Custom algorithms with verbose output
tanks_game.exe -algo1 BFS -algo2 Simple -verbose -steps 50

# Load custom map
tanks_game.exe -map inputs\maze_map.txt -verbose

# Tournament mode
tanks_game.exe -tournament -games 5

# Quick tournament
tanks_game.exe -tournament -games 1
```

## 🗺️ Maps

The game includes several pre-built maps:

- `inputs\simple_map.txt` - Basic 10x9 map with obstacles
- `inputs\maze_map.txt` - Complex maze layout  
- `inputs\open_field.txt` - Large open battlefield

### Map Format
- `#` - Wall/Obstacle
- `1` - Player 1 starting position
- `2` - Player 2 starting position  
- ` ` - Empty space

## 🤖 Available Algorithms

- **Simple** - Basic algorithm with random movement and occasional shooting
- **BFS** - Breadth-First Search pathfinding with strategic movement
- **Random** - Random variant with different behavior patterns

## 🏆 Tournament System

The tournament system runs round-robin competitions between all registered algorithms:

```bash
# Full tournament (3 games per matchup)
mingw32-make tournament

# Quick tournament (1 game per matchup)  
mingw32-make quick-tournament

# Custom tournament
tanks_game.exe -tournament -games 5 -steps 50
```

Results show wins, losses, ties, and win rates for each algorithm.

## 📁 Project Structure

```
Project3/
├── common/                 # Interface definitions
│   ├── ActionRequest.h     # Action enumerations  
│   ├── BattleInfo.h        # Battle information interface
│   ├── SatelliteView.h     # Map view interface
│   ├── TankAlgorithm.h     # Tank AI interface
│   ├── Player.h            # Player interface
│   ├── GameResult.h        # Game result structure
│   └── AbstractGameManager.h # Game manager interface
├── src/                    # Core game implementation
│   ├── Position.h          # 2D position utilities
│   ├── Direction.h         # Direction management
│   ├── CellType.h          # Map cell types
│   ├── Board.*             # Game board with collision detection
│   ├── Tank.*              # Tank implementation
│   ├── Shell.*             # Shell physics  
│   ├── TankBattleInfo.*    # Battle information
│   ├── GameState.*         # Game state management
│   ├── CollisionDetector.* # Collision detection system
│   ├── ActionProcessor.*   # Action validation and processing
│   ├── GameManager.*       # Main game orchestration
│   ├── main.cpp            # Application entry point
│   └── algorithms/         # Algorithm implementations
│       ├── SimpleAlgorithm.* # Basic random algorithm
│       ├── BfsAlgorithm.*    # Pathfinding algorithm
│       ├── MyPlayer.*        # Player implementation
│       ├── AlgorithmFactory.* # Factory functions
│       └── TournamentManager.* # Tournament system
├── inputs/                 # Sample maps
│   ├── simple_map.txt      # Basic map
│   ├── maze_map.txt        # Maze layout
│   └── open_field.txt      # Open battlefield
├── Makefile               # Build configuration
├── run.bat                # Quick run script
└── README.md              # This file
```

## 🔧 Build Targets

```bash
# Core build targets
mingw32-make all              # Build complete project
mingw32-make clean            # Clean build files
mingw32-make help             # Show available targets

# Game execution
mingw32-make run              # Run with default settings
mingw32-make demo             # Demo game
mingw32-make list             # List algorithms
mingw32-make tournament       # Full tournament
mingw32-make quick-tournament # Quick tournament

# Algorithm battles
mingw32-make simple-vs-bfs    # Simple vs BFS
mingw32-make bfs-vs-simple    # BFS vs Simple

# Testing
mingw32-make test_interfaces    # Test interface compilation
mingw32-make test_components    # Test core components
mingw32-make test_plugins       # Test plugin architecture
mingw32-make test_tournament    # Test tournament system
```

## 🎨 Features

### ✅ Completed Features
- **Complete Game Engine** - Full tank battle simulation
- **Plugin Architecture** - Dynamic algorithm registration
- **Multiple AI Algorithms** - Simple, BFS, and Random strategies
- **Tournament System** - Round-robin competitions with statistics
- **Custom Maps** - Support for user-defined battlefields
- **Verbose Output** - Detailed game state logging
- **Command Line Interface** - Flexible parameter configuration
- **Comprehensive Testing** - Unit and integration test suites

### 🔄 Game Flow
1. **Initialization** - Load map, create players, setup algorithms
2. **Turn Processing** - Each tank requests and executes actions
3. **Physics** - Move shells, detect collisions, update positions
4. **Win Conditions** - Check for tank destruction or step limits
5. **Results** - Display winner and game statistics

## 🏁 Game Rules

- Tanks start with 5 shells and full health
- Maximum 100 steps per game (configurable)
- Tanks can move, rotate (45°/90°), and shoot
- Shells travel in straight lines and damage on impact
- Games end when a tank is destroyed or step limit reached
- Ties occur when step limit is reached with both tanks alive

## 🧪 Testing

The project includes comprehensive test suites:

```bash
# Run all tests
mingw32-make test_interfaces test_components test_plugins test_tournament

# Individual test categories
mingw32-make test_interfaces   # Interface compilation
mingw32-make test_components   # Core game components  
mingw32-make test_plugins      # Plugin architecture
mingw32-make test_tournament   # Tournament system
```

## 📝 Development Notes

- Built with C++17 standard
- Uses STL containers and smart pointers
- Template-based factory system
- Interface-driven design for extensibility
- Comprehensive error handling and validation

---

**Status:** ✅ Phase 5 Complete - Full plugin architecture with tournament system implemented and tested

**Ready for Play!** 🎮 Use `run.bat demo` or `mingw32-make demo` to start playing!
