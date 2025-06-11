#include "Shell.h"
#include "Tank.h"
#include <iostream>

void Shell::update(Board &board, std::vector<std::unique_ptr<Tank>> &tanks, std::vector<Position> &explosions)
{
    // Get direction offset
    auto [dx, dy] = Directions::directionToOffset(direction);

    // Calculate new position
    auto [newX, newY] = tryToMove(board, dx, dy);

    // Check if the new position is valid
    CellType targetCell = board.getCellType(newX, newY);

    if (targetCell == CellType::WALL || targetCell == CellType::WEAK_WALL)
    {
        // Shell hits wall, weaken it
        board.weakenWall(newX, newY);
        explode(newX, newY, explosions);
    }
    else
    {
        // Shell moves to empty cell
        move(board, dx, dy);
    }
}

bool Shell::collidesWith(const GameObject &obj) const
{
    // Check if the shell and object are at the same position
    return (getX() == obj.getX() && getY() == obj.getY());
}

void Shell::explode(int newX, int newY, std::vector<Position> &explosions)
{
    exploded = true;
    explosions.emplace_back(newX, newY);
}

void Shell::explode(std::vector<Position> &explosions)
{
    exploded = true;
    explosions.emplace_back(getX(), getY());
}