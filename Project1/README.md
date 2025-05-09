# Tank Battle Game

## Authors
- Shachar Gabbay 213144173
- Dor Liberman 318635463s

## Project Overview
This project implements a turn-based tank battle game where two tanks navigate a 2D grid and attempt to destroy each other. The game features collision detection, wall destruction mechanics, mine placement, and shell firing.

## Features
- Turn-based gameplay with two tanks
- Multiple board layouts loaded from text files
- Shell firing with collision detection
- Wall destruction mechanics (walls weaken after hits)
- Mine placement that destroys tanks on contact
- Text-based output with game state reporting

### Bonus Feature: Visual Representation 🎮
We've implemented a **visual representation mode** as a bonus feature, which displays the game state using emojis in the terminal:
- Tank 1: 🚗
- Tank 2: 🚙 
- Wall: 🟩
- Mine/Shell: 💥
- Empty space: ⬜

This visual mode makes it easier to follow the game's progress and understand the interactions between game elements.

## Building and Running

### Prerequisites
- C++17 compatible compiler (g++ or clang++)
- Make utility

### Build Instructions
```bash
# Build the game
make

# Clean build files
make clean
```

### Running the Game
```bash
# Run in text mode (standard)
./tank_game resources/exampleBoard.txt

# Run with visual representation (bonus feature)
./tank_game resources/exampleBoard.txt --visual

# Alternatively, use the make targets
make run ARGS=resources/exampleBoard.txt
make run_visual ARGS=resources/exampleBoard.txt
```

## Game Rules
- Each tank can move forward or backward, rotate, and shoot shells
- Tanks are destroyed when hit by shells or mines
- Walls weaken when hit by shells and are destroyed after 2 hits
- Shells can travel and destroy tanks or walls
- Game ends when one or both tanks are destroyed, or after 1000 turns

## Project Structure
```
Project1/
├── src/                  # Source code
│   ├── main.cpp          # Entry point
│   ├── GameManager.cpp   # Game management
│   ├── Board.cpp         # Game board implementation
│   ├── Tank.cpp          # Tank behavior
│   ├── Shell.cpp         # Shell/projectile behavior
│   ├── GameObject.h      # Base class for game objects
│   ├── ActionType.h      # Action enum definitions
│   ├── algorithms/       # AI controller algorithms
│   └── utils/            # Utility functions including visual display
├── resources/            # Game board files
│   ├── exampleBoard.txt  # Sample board
│   └── errorBoard.txt    # Board with errors (for testing)
├── inputs/               # Additional input files
│   ├── input1.txt        # Test input 1
│   ├── input2.txt        # Test input 2
│   └── input3.txt        # Test input 3
├── doc/                  # Documentation
│   └── bonus.txt         # Description of bonus features
└── Makefile              # Build instructions
```

## Design Overview
The game follows object-oriented design principles with:
- `GameObject` as the base class for all game entities
- Specialized classes for `Tank` and `Shell` that inherit from the `GameObject` base class
- `Board` class to manage the game grid with wall damage tracking and collision detection
- `GameManager` to control game flow, collision resolution, and win/loss conditions
- Separation of concerns between game logic and display through dedicated utilities
- Comprehensive enum system for directions (8-way), cell types, and tank actions

## High Level Design Documents
A high-level design document (HLD) is available in the `/doc` directory, containing:
- UML class diagrams showing inheritance hierarchy (GameObject → Tank/Shell)
- UML sequence diagrams of the main program flow and turn processing
- Design considerations and alternatives for movement, rotation, and collision systems
- Testing approach details including edge cases and integration tests
- Implementation of the Observer pattern for game state monitoring
- Strategy pattern for tank control algorithms


## Actions

- Each tank in GameManager has an algorithm (alg1, alg2)
- Use ChaseTank to chase a tank (for example alg1->ChaseTank(*board, tank1, tank2, shells);)
- Use EvadeTank to evade (for example alg2->EvadeTank(*board, tank2, tank1, shells);)
- For Other operations, you may use one of the operations in ActionType:

    MOVE_FORWARD,
    MOVE_BACKWARD,
    ROTATE_LEFT_1_8,
    ROTATE_RIGHT_1_8,
    ROTATE_LEFT_1_4,
    ROTATE_RIGHT_1_4,
    SHOOT,
    NONE
(For example, ActionType::SHOOT)
## Testing
The game has been tested through:
- Manual testing with various board configurations (open arenas, maze-like structures)
- Edge case testing for boundaries and collisions (tank-tank, tank-shell, shell-shell)
- Corner cases like simultaneous destruction and multiple shell tracking
- Backward movement delay verification (3-step waiting process)
- Visual verification using the bonus visual representation mode
- Battle scenarios with different starting positions using input1.txt, input2.txt, and input3.txt

## Bonus Implementation Details
The visual representation bonus was implemented by:
1. Adding command-line flag support (`--visual`) processed in main.cpp
2. Creating a specialized display function using Unicode emojis in utils.cpp
3. Adding rendering of the game state after each turn with configurable delay (300ms)
4. Implementing clear visual distinction between different game elements:
   - Tanks represented by different vehicle emojis (🚗/🚙) to distinguish players
   - Explosions (💥) for both mines and shell impacts for dramatic effect
   - Walls (🟩) and empty spaces (⬜) with high contrast for clear navigation
5. Terminal display optimization to prevent flickering during updates
6. Cross-platform emoji support considerations

For more details on the bonus implementation, see `doc/bonus.txt`.
