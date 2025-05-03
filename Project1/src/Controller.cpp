#include "Controller.h"
#include "Board.h"
#include "Position.h"
#include <iostream>
#include <fstream>
#include <queue>

// BFS to find the safe path on the board

ActionType Controller::chaseTank(Position chaserStart, Position target) {
    std::queue<Position> q;
    std::vector<std::vector<bool>> visited(Global::board->getHeight(), std::vector<bool>(Global::board->getWidth(), false));
    q.push(chaserStart);
    visited[chaserStart.y][chaserStart.x] = true;

    // To track the direction to take based on the BFS pathfinding
    std::vector<std::vector<Position>> parent(Global::board->getHeight(), std::vector<Position>(Global::board->getWidth(), {-1, -1}));

    // BFS exploration
    while (!q.empty()) {
        Position current = q.front();
        q.pop();
        // If we find the target position
        if ( current == target) {
            // Backtrack to find the action to take (where the chaser should move)
            Position safeSpot = current;
            Position parentSpot = parent[safeSpot.y][safeSpot.x];

            // Determine the direction of movement from the parent
            if (parentSpot.x == safeSpot.x - 1 && parentSpot.y == safeSpot.y) {
                return ActionType::MOVE_FORWARD;  // Move forward (down)
            }
            if (parentSpot.x == safeSpot.x + 1 && parentSpot.y == safeSpot.y) {
                return ActionType::MOVE_FORWARD;  // Move backward (up)
            }
            if (parentSpot.x == safeSpot.x && parentSpot.y == safeSpot.y - 1) {
                return ActionType::ROTATE_LEFT_1_8;  // Rotate left
            }
            if (parentSpot.x == safeSpot.x && parentSpot.y == safeSpot.y + 1) {
                return ActionType::ROTATE_RIGHT_1_8;  // Rotate right
            }
        }

        // Explore all possible directions
        for (auto &dir : Directions::getAllDirections()) {
            // Get the (dx, dy) offset for the current direction
            auto [dx, dy] = Directions::directionToOffset(dir);

            // Calculate the neighbor position based on the current position
            Position neighbor = {current.x + dx, current.y + dy};

            // Check if the neighbor is within bounds, not visited, and not an obstacle (wall or mine)
            if (neighbor.x >= 0 && neighbor.x < Global::board->getWidth() &&
                neighbor.y >= 0 && neighbor.y < Global::board->getHeight() &&
                !visited[neighbor.y][neighbor.x] &&
                Global::board->getCellType(neighbor.x, neighbor.y) != CellType::WALL &&
                Global::board->getCellType(neighbor.x, neighbor.y) != CellType::MINE) {

                visited[neighbor.y][neighbor.x] = true;
                parent[neighbor.y][neighbor.x] = current;
                q.push(neighbor);
            }
        }
    }

    return ActionType::NONE;  // No safe path found
}



ActionType Controller::DecideAction(
    Tank &myTank,
    Tank &enemyTank,
    const std::vector<Shell> &shells)
{
    static int counter = 0;

    // Avoid stepping on a mine.
    if (IsMineNearby(myTank))
    {
        return ActionType::ROTATE_RIGHT_1_8; // Rotate to avoid stepping on a mine (or another suitable action)
    }

    if(IsTankNearby(myTank))
        return ActionType::SHOOT; 

    return chaseTank( myTank.getPosition(), enemyTank.getPosition());
    for (const Shell &shell : shells)
    {
        if (shell.isTargeting(myTank))
        {
            // If a shell is targeting my tank, dodge it
            if (IsSafeToMoveForward(myTank))
            {
                return ActionType::MOVE_FORWARD;
            }
            else
            {
                return ActionType::ROTATE_RIGHT_1_8;
            }
        }
    }


    // Avoid stepping on a mine.
    if (IsMineNearby(myTank)||IsTankNearby(myTank))
    {
        return ActionType::ROTATE_RIGHT_1_8; // Rotate to avoid stepping on a mine (or another suitable action)
    }
    return ActionType::MOVE_FORWARD;


    ++counter;
}

// Helper function to check if we can shoot the opponent's tank
bool Controller::CanShoot(const Tank &enemyTank)
{
    // Check if the enemy tank is within range and line of sight (this is a placeholder logic)
    return IsInLineOfSight(enemyTank) && IsInRange(enemyTank);
}

// Helper function to check if the shell is targeting the tank
bool Controller::IsShellChasingTank(const Shell &shell, const Tank &myTank)
{
    // Implement logic to check if a shell is heading toward the tank
    return shell.isTargeting(myTank);
}

// Helper function to check if the tank can safely move forward (e.g., no obstacles or mines)
bool Controller::IsSafeToMoveForward( Tank &myTank)
{
    // Check if the next position is clear and doesn't contain a mine
    return !IsMineNearby(myTank) && !IsObstacleAhead(myTank);
}

// Helper function to check if there's a mine nearby
bool Controller::IsMineNearby( Tank &myTank)
{
    // Implement logic to check if the tank is about to step on a mine (based on known mine locations)
    auto [new_x, new_y] = myTank.moveForward();
    return Global::board->getCellType(new_x, new_y) == CellType::MINE;
}

// Helper function to check if there's a tank nearby
bool Controller::IsTankNearby( Tank &myTank)
{
    // Implement logic to check if the tank is about to step on another tank 
    auto [new_x, new_y] = myTank.moveForward();
    return Global::board->getCellType(new_x, new_y) == CellType::TANK1||Global::board->getCellType(new_x, new_y) == CellType::TANK2;
}

// Helper function to check if the opponent's tank is in line of sight
bool Controller::IsInLineOfSight(const Tank &enemyTank)
{
    // Implement line of sight check
    return true; 
}

// Helper function to check if the opponent's tank is within shooting range
bool Controller::IsInRange(const Tank &enemyTank)
{
    // Implement range check (e.g., check if the enemy tank is within firing range)
    return true; // Placeholder for actual range logic
}

// Helper function to check if there's an obstacle ahead
bool Controller::IsObstacleAhead(const Tank &myTank)
{
    // Implement logic to check if there's an obstacle ahead (e.g., wall or other tank)
    return false; // Placeholder for actual obstacle check
}
