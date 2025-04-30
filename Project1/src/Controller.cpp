#include "Controller.h"
#include "Board.h"
#include "Position.h"
#include <iostream>
#include <queue>

// BFS to find the safe path on the board
ActionType Controller::findSafePath(Board &gameBoard, Position start) {
    std::queue<Position> q;
    // std::vector<std::vector<bool>> visited(gameBoard.getHeight(), std::vector<bool>(gameBoard.getWidth(), false));

    // q.push(start);
    // visited[start.y][start.x] = true;

    // // To track the direction to take based on the BFS pathfinding
    // std::vector<std::vector<Position>> parent(gameBoard.getHeight(), std::vector<Position>(gameBoard.getWidth(), {-1, -1}));

    // // BFS exploration
    // while (!q.empty()) {
    //     Position current = q.front();
    //     q.pop();

    //     // If we find a safe position (an empty cell or an already discovered safe cell)
    //     if (gameBoard.getCellType(current.x, current.y) == CellType::EMPTY) {
    //         // Backtrack to find the action to take (where the tank should move)
    //         Position safeSpot = current;
    //         Position parentSpot = parent[safeSpot.y][safeSpot.x];

    //         // Determine the direction of movement from the parent
    //         if (parentSpot.x == safeSpot.x - 1 && parentSpot.y == safeSpot.y) {
    //             return ActionType::MOVE_FORWARD;  // Move down in the direction
    //         }
    //         if (parentSpot.x == safeSpot.x + 1 && parentSpot.y == safeSpot.y) {
    //             return ActionType::MOVE_BACKWARD;  // Move backward
    //         }
    //         // Add more cases for rotation if required (based on where the tank needs to move)
    //         if (parentSpot.x == safeSpot.x && parentSpot.y == safeSpot.y - 1) {
    //             return ActionType::ROTATE_LEFT_1_8;  // Rotate left
    //         }
    //         if (parentSpot.x == safeSpot.x && parentSpot.y == safeSpot.y + 1) {
    //             return ActionType::ROTATE_RIGHT_1_8;  // Rotate right
    //         }
    //     }


    //     for (auto &dir : Directions::getAllDirections()) {
    //         // Get the (dx, dy) offset for the current direction
    //         auto [dx, dy] = Directions::directionToOffset(dir);

    //         // Calculate the neighbor position based on the current position
    //         Position neighbor = {current.x + dx, current.y + dy};

    //         // Check if the neighbor is within bounds and not visited
    //         if (neighbor.x >= 0 && neighbor.x < gameBoard.getWidth() &&
    //             neighbor.y >= 0 && neighbor.y < gameBoard.getHeight() &&
    //             !visited[neighbor.y][neighbor.x] &&
    //             gameBoard.getCellType(neighbor.x, neighbor.y) != CellType::WALL && 
    //             gameBoard.getCellType(neighbor.x, neighbor.y) != CellType::MINE) {

    //             visited[neighbor.y][neighbor.x] = true;
    //             parent[neighbor.y][neighbor.x] = current;
    //             q.push(neighbor);
    //         }
    //     }
    // }

    return ActionType::NONE;  // No safe path found
}



ActionType Controller::DecideAction(
    Tank &myTank,
    Tank &enemyTank,
    const std::vector<Shell> &shells)
{
    static int counter = 0;

    // Try to shoot the opponent's tank if it's within a certain range and there is a clear shot.
    if (CanShoot(enemyTank))
    {
        return ActionType::SHOOT;
    }

    // Try to move the tank if a shell is chasing it.
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
