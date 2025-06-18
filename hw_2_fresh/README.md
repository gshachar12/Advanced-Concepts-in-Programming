# Tank Battle Game

## Overview

Tank Battle is a console-based strategy game where players control tanks on a 2D grid-based battlefield. The game implements modern C++ practices and various design patterns to create an immersive and challenging gameplay experience.

## Game Description

In Tank Battle, each player controls one or more tanks on a grid-based map. Players can move their tanks, rotate them in different directions, and fire shells at opponents. The goal is to destroy all enemy tanks while protecting your own.

### Game Elements

- **Tanks**: Player-controlled units that can move, rotate, and fire shells
- **Walls**: Obstacles that block movement and shells 
- **Weak Walls**: Barriers that can be destroyed after one hit from a shell (represented by '=' symbol)
- **Mines**: Explosive objects that damage tanks
- **Shells**: Projectiles fired by tanks that damage other tanks, walls, or mines

## Building the Game

To build the game, simply run:

```bash
make
```

This will compile the game and generate the `tanks_game` executable.

## Running the Game

To play the game, run:

```bash
./tanks_game inputs/<map_file>
```

Where `<map_file>` is one of the provided map files in the `inputs/` directory.

### Maps

The game includes several map files:
- `input1.txt` - Simple map with minimal obstacles
- `input2.txt` - Tactical map with multiple tanks per player
- `input3.txt` - Close combat map
- `input4.txt` - Maze challenge
- `input5.txt` - Battlefield with mines
- `input_weak_wall.txt` - Test map for weak walls

### Game Controls

The game is AI-controlled, so no user input is required during gameplay. The tanks will move and act according to their programming.

## Game Rules

1. Each tank can move forward, rotate, or fire a shell on its turn
2. Shells travel in the direction they are fired until they hit something
3. When a shell hits a wall, the wall is weakened
4. Regular walls require two hits to destroy
5. Weak walls (marked with '=') require only one hit to destroy
6. When a tank is hit by a shell, it is destroyed
7. The game ends when all tanks of one player are destroyed, or when the maximum number of steps is reached

## File Structure

- `src/` - Source code files
  - `gameManager/` - Game management and core functionality
  - `algorithms/` - Tank AI algorithms
- `include/` - Header files
- `inputs/` - Game map files
- `outputs/` - Output files from game runs
- `logs/` - Log files including error messages
- `build/` - Compiled objects and binaries

## Logging

The game generates logs in the `logs/` directory:
- `log.txt` - General game logs
- `error.txt` - Error messages
- `input_error.txt` - Input parsing errors

## Testing

To run the test suite:

```bash
./run_tests.sh
```

## Visual Mode

For a better visual experience, you can run the game with the `-g` flag:

```bash
./tanks_game -g inputs/<map_file>
```

This will display the game with emojis representing different game elements:
- 🟩 - Wall
- 🧱 - Weak Wall
- 💣 - Mine
- 🚀 - Shell
- 💥 - Explosion
- ⬜ - Empty space

## Creating Custom Maps

You can create your own maps as text files with the following format:

```
Description Line
MaxSteps=<number>
NumShells=<number>
Rows=<number>
Cols=<number>
###############
#1           #
#            #
#    ===     #
#    ===     #
#            #
#           2#
###############
```

Where:
- `#` represents walls
- `=` represents weak walls
- `@` represents mines
- `1` represents player 1 tanks
- `2` represents player 2 tanks
- Spaces represent empty cells

## Technical Information

- Language: C++17
- Build System: Make
- Design Patterns: Factory, Observer, Singleton, Strategy
- Data Structures: 2D vectors, maps, sets

## Contributors

- Advanced Concepts in Programming - TAU 2025B

## License

This project is for educational purposes only.