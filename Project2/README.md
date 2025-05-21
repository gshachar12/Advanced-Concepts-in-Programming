# Project2 - Tank Battle Game

This project is an implementation of a tank battle game where tanks controlled by algorithms compete on a battlefield. The game supports two players with multiple tanks, and features a satellite view system that allows tanks to gather information about their surroundings.

## Features

- Two-player tank battle game
- Support for multiple tanks per player
- Tanks controlled by deterministic algorithms
- Battle information system via satellite view
- Customizable game maps with mines and walls
- Shell firing and collision detection
- Tank coordination through player objects
- Output logging of game actions and results

## Building the Project

To build the project, run:

```bash
make
```

This will compile all source files and create the executable in the `bin` directory.

## Running the Game

To run the game, use:

```bash
./bin/tanks_game resources/maps/map1.txt
```

Or you can use the provided make target:

```bash
make run
```

This will run the game using the default map file `resources/maps/map1.txt`.

## Map File Format

Map files follow this format:

```
Description Line
MaxSteps = <number>
NumShells = <number>
Rows = <number>
Cols = <number>
###############
#      @      #
#  1       2  #
#             #
#     ###     #
...
```

Where:
- `#` represents walls
- `@` represents mines
- `1` represents player 1 tanks
- `2` represents player 2 tanks
- Spaces represent empty cells

## Output

The game generates an output file in the same location as the input map file, with `.out` appended to the filename. This file contains a line for each game round showing the actions of all tanks, and a final line showing the game result.

Example output:
```
MoveForward, MoveBackward, RotateLeft90, RotateRight90
RotateLeft45, RotateRight45 (ignored), Shoot, GetBattleInfo
DoNothing, DoNothing, Shoot (ignored), MoveForward
RotateLeft45, MoveForward (killed), MoveForward, MoveForward
MoveForward, killed, MoveForward, MoveForward (ignored) (killed)
Player 2 won with 2 tanks still alive
```

## Architecture

This project uses a component-based architecture with the following main classes:

- `GameManager`: Controls game flow and processes rounds
- `Board`: Represents the game board and handles cell interactions
- `Tank`: Represents a tank with movement and shooting capabilities
- `Shell`: Represents an artillery shell fired by a tank
- `Player`: Interface for player implementations that coordinate tanks
- `TankAlgorithm`: Interface for tank AI implementations that control tank actions
- `SatelliteView`: Interface for providing a view of the battlefield to tank algorithms

## Project Structure

- `src/`: Source code
  - `common/`: Common API interfaces
  - `algorithms/`: Tank algorithm implementations
- `resources/`: Resource files
  - `maps/`: Map files for the game
- `bin/`: Compiled executable
- `build/`: Build artifacts

## Author

TAU - Advanced Topics in Programming, Semester B 2025