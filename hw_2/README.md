# Tank Battle Simulation

## Project Overview
This project implements a turn-based tank battle simulation engine where two players with multiple tanks compete on a 2D game board. The game engine handles tank movement, rotation, shooting mechanics, collisions, and various game elements including destructible and indestructible walls, mines, and shells.

## Key Features
- Tank movement and rotation in 8 directions
- Shooting mechanism with limited ammunition
- Different types of walls (standard and weakened)
- Mine placement and explosion mechanics
- Collision detection and resolution
- Support for multiple tanks per player
- Customizable tank AI through algorithm interfaces
- Configurable game parameters (board size, max steps, shell count)

## Game Elements
- **Tanks**: Mobile units that can move, rotate, and shoot
- **Walls**: Indestructible obstacles represented by '#'
- **Weak Walls**: Destructible obstacles represented by 'W'
- **Mines**: Explosive traps represented by '@'
- **Shells**: Projectiles fired by tanks

## Architecture

### Core Components
The simulation is built around several key components:

1. **GameObject System**
   - Base `GameObject` class with virtual methods for common behaviors
   - Derived classes: `Tank`, `Wall`, `WeakWall`, `Mine`, and `Shell`
   - Support for collision detection and object interaction

2. **Board Management**
   - `Board` class manages object placement and movement
   - Handles object wrapping on board edges
   - Provides object queries and state retrieval

3. **Player and Algorithm Interface**
   - `Player` interface coordinates team strategy
   - `TankAlgorithm` interface defines individual tank behavior
   - `SatelliteView` provides board observation capability

4. **Game Flow Control**
   - `GameManager` orchestrates the turn sequence
   - Processes player actions and outcomes
   - Handles game termination conditions

## Input Format
The game processes map files with the following format:
```
Description line
MaxSteps=<number>
NumShells=<number>
Rows=<number>
Cols=<number>
<map layout using '#', 'W', '@', '1', '2', and spaces>
```

## Output Format
The game produces a log file that records each round's actions:
```
<tank1_action>, <tank2_action>, <tank3_action>, ...
<tank1_action>, <tank2_action> (ignored), <tank3_action>, ...
...
Player <X> won with <Y> tanks still alive
```

## Building and Running

### Requirements
- C++17 compatible compiler
- CMake 3.10+
- Standard library

### Build Instructions
```bash
cd Project2
make
```

### Running the Game
```bash
./tanks_game -g <input_file_path>
```

## Testing
The project includes unit tests covering:
- Wall damage mechanics
- Weak wall behavior and destruction
- Board operations and object management
- Tank movement and collision handling

Run the tests with:
```bash
cd build
./tanks_game_tests
```

## Implementation Details

### Object Hierarchy
```
GameObject
├── Wall
│   └── WeakWall
├── Tank
├── Mine
└── Shell
```

### Direction System
The game uses a Direction system with 8 possible orientations:
- UP (0°)
- UP_RIGHT (45°)
- RIGHT (90°)
- DOWN_RIGHT (135°)
- DOWN (180°)
- DOWN_LEFT (225°)
- LEFT (270°)
- UP_LEFT (315°)
- NONE (-1) for stationary objects

### Health and Damage
- Regular walls are indestructible
- Weak walls have 1 hit point and are destroyed in one hit
- Tanks have varying health based on implementation

## Design Choices
- **Smart Pointers**: Used throughout to avoid memory leaks
- **Virtual Functions**: Employed for polymorphic behavior
- **Factory Pattern**: Used for object creation
- **Command Pattern**: Used for tank actions

## Acknowledgements
This project was created as part of the Advanced Topics in Programming course at Tel Aviv University, Semester B 2025.