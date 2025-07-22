# Tank Battle Game - Project 3

## Authors
- Shachar Gabbay 213144173  
- Dor Liberman 318635463

## Project Overview
Project 3 extends the tank battle game with a dynamic plugin architecture, allowing algorithms to be loaded at runtime and compete in tournaments. This implementation is built from scratch while maintaining compatibility with previous project concepts.

## Development Progress

### ✅ Phase 1: Foundation Setup (COMPLETED)
- [x] Created project structure (common/, src/, simulator/, algorithms/)
- [x] Implemented core interfaces:
  - `ActionRequest` - Tank action enumeration
  - `BattleInfo` - Battle information base class  
  - `SatelliteView` - Read-only board view interface
  - `TankAlgorithm` - Algorithm base class and factory
  - `Player` - Player management interface and factory
  - `GameResult` - Tournament result structure
  - `AbstractGameManager` - Game execution interface
- [x] Set up build system with Makefile
- [x] Verified interface compilation

### ✅ Phase 2: Core Game Components (COMPLETED)
- [x] Implemented basic game types:
  - `Position` - 2D coordinate system with utility methods
  - `Direction` - 8-way direction system with rotation utilities
  - `CellType` - Game board cell types and conversion utilities
- [x] Implemented core game objects:
  - `Board` - Game board with SatelliteView interface implementation
  - `Tank` - Tank with movement, rotation, and combat capabilities
  - `Shell` - Projectile with movement and collision detection
- [x] Added comprehensive functionality:
  - Wall damage system (regular walls take 2 hits, weak walls take 1)
  - Tank shooting with cooldown and ammunition tracking
  - Shell movement (2 cells per turn) with maximum range
  - Position validation and boundary checking
- [x] Created and verified test suite for all components

### ✅ Phase 3: Game Management System (COMPLETED)
- [x] Implemented comprehensive battle information system:
  - `TankBattleInfo` - Concrete BattleInfo with tank state and environmental data
  - Visible enemy and shell tracking for algorithms
  - Map dimension and positioning information
- [x] Created complete game state management:
  - `GameState` - Central game state with tanks, shells, and board management
  - Multi-tank support per player with proper indexing
  - Game progression tracking (steps, win conditions, statistics)
- [x] Built collision detection and resolution system:
  - `CollisionDetector` - Comprehensive collision detection for all entity types
  - Shell-wall, shell-tank, shell-shell, tank-mine collision handling
  - Predictive collision checking for movement validation
- [x] Implemented action processing system:
  - `ActionProcessor` - Converts ActionRequest enums to game state changes
  - All movement, rotation, and shooting actions supported
  - Battle information generation for algorithm decision-making
- [x] Added complete testing and verification of all systems

### ✅ Phase 4: Complete Game Manager (COMPLETED)
- [x] Implemented GameManager with complete game loop:
  - Full AbstractGameManager interface implementation
  - Turn-based execution with algorithm coordination
  - Map initialization from SatelliteView interface
  - Tank algorithm factory integration
- [x] Added comprehensive game flow management:
  - Battle information updates for all tanks each turn
  - Action execution for all active tanks
  - Shell movement and physics updates
  - Collision detection and resolution
  - Tank state updates (cooldowns, health)
- [x] Implemented win/loss condition checking:
  - Real-time game termination detection
  - GameResult generation with proper winner determination
  - Support for ties and maximum step limits
- [x] Added verbose logging and debugging support:
  - Turn-by-turn action logging
  - Tank movement and action success/failure tracking
  - Game end notifications with results
- [x] Verified compilation and basic functionality testing

### ✅ Phase 5: Plugin Architecture (COMPLETED)
- [x] Sample Tank Algorithms:
  - SimpleAlgorithm - Basic movement with random actions and periodic shooting
  - BfsAlgorithm - Pathfinding-based algorithm with strategic movement
  - RandomAlgorithm - Variant of simple algorithm with different behavior patterns
- [x] Player Implementation:
  - MyPlayer - Concrete Player class implementing battle information updates
  - Integration with SatelliteView interface for battlefield awareness
- [x] Factory System:
  - AlgorithmFactory - Factory methods for creating all algorithm types
  - TankAlgorithmFactory and PlayerFactory function types
  - Support for player and tank index parameterization
- [x] Registration System:
  - AlgorithmRegistrar - Dynamic algorithm registration and lookup
  - Automatic registration of default algorithms
  - Algorithm information with names and descriptions
- [x] Tournament System:
  - TournamentManager - Complete tournament orchestration
  - Round-robin and single-elimination tournament support
  - Comprehensive statistics and ranking calculation
  - Configurable game parameters and verbose output
- [x] Testing and Verification:
  - Plugin architecture functionality verified
  - Algorithm creation and registration tested
  - Tournament execution with multiple algorithms confirmed

### ⏳ Phase 6: Algorithm System (PLANNED)
- [ ] Implement sample algorithms
- [ ] Create factory system  
- [ ] Add tournament capabilities

### ⏳ Phase 6: Integration & Testing (PLANNED)
- [ ] Build simulator
- [ ] Add comprehensive testing
- [ ] Final integration and cleanup

## Building the Project

### Prerequisites
- C++17 compatible compiler (g++ or clang++)
- Make utility

### Build Instructions
```bash
# Test interface compilation
make test_interfaces     # (requires make utility)

# Alternative: Direct compilation testing
g++ -std=c++17 -Wall -Wextra -Icommon -Isrc -c src/Board.cpp -o src/Board.o

# Build and run component test
g++ -std=c++17 -Wall -Wextra -Icommon -Isrc test_components.cpp src/*.o -o test_components.exe
./test_components.exe

# Build the complete project (when ready)
make

# Clean build files
make clean
```

## Key Design Principles

1. **Interface-First Design**: All interactions go through well-defined interfaces
2. **Plugin Architecture**: Algorithms can be loaded dynamically at runtime
3. **Factory Pattern**: Objects are created through factory functions for flexibility
4. **Separation of Concerns**: Game logic, algorithms, and simulation are separate
5. **Zero Dependencies**: No code imported from previous projects

## Next Steps

The next phase will implement the core game components (Tank, Shell, Board) that provide the actual game mechanics while respecting the interface contracts established in Phase 1.
