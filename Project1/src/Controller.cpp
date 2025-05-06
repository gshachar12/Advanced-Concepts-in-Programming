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

ActionType Controller::pickEvadeDirection(Tank &myTank, Tank &enemyTank) {
    // Create Position objects for the tanks
    Position myPos(myTank.getX(), myTank.getY());
    Position enemyPos(enemyTank.getX(), enemyTank.getY());

    // Avoid stepping on a mine.
    if (IsMineNearby(myTank)) {
        return ActionType::ROTATE_RIGHT_1_8;
    }

    if(IsTankNearby(myTank)) {
        return ActionType::SHOOT; 
    }
    
    // Check if enemy is close enough to evade
    double distance = myPos.distanceTo(enemyPos);
    
    // If enemy is within 4 units, evade
    if (distance < 4.0) {
        return handleCloseEvade(myTank, enemyTank, myPos, enemyPos, distance);
    }
    
    // If enemy is not close enough, still try to increase distance
    return handleDistantEvade(myTank, enemyTank, myPos, enemyPos, distance);
}

ActionType Controller::handleCloseEvade(Tank &myTank, Tank &enemyTank, 
                                        const Position &myPos, const Position &enemyPos, double distance) {
    // Vector from myTank to enemyTank
    double dx = enemyTank.getX() - myTank.getX();
    double dy = enemyTank.getY() - myTank.getY();

    // Angle toward enemy
    double angleToEnemy = std::atan2(dy, dx);

    // Desired angle is 180° away (opposite direction)
    double evadeAngle = angleToEnemy + M_PI;
    evadeAngle = std::fmod(evadeAngle, 2.0 * M_PI);
    if (evadeAngle < 0) evadeAngle += 2.0 * M_PI;

    Direction desired = angleToClosestDirection(evadeAngle);
    Direction current = myTank.getDirection();
    
    // Check if we're already facing the enemy
    Direction enemyDir = angleToClosestDirection(angleToEnemy);
    bool facingEnemy = (current == enemyDir);
    
    // If we're facing the enemy, moving backward is most efficient
    if (facingEnemy) {
        return handleFacingEnemyEvade(myTank);
    }
    
    // Check if facing away from enemy (ideal for moving forward)
    bool facingAway = (current == desired);
    
    if (facingAway) {
        return handleFacingAwayEvade(myTank);
    }
    
    // We need to rotate toward the desired direction
    return calculateRotationDirection(current, desired);
}

ActionType Controller::handleFacingEnemyEvade(Tank &myTank) {
    // Check if backward move is valid
    auto [dx, dy] = Directions::directionToOffset(myTank.getDirection());
    int backX = myTank.getX() - dx;
    int backY = myTank.getY() - dy;
    
    if (isValidPosition(backX, backY)) {
        return ActionType::MOVE_BACKWARD;
    }
    
    return ActionType::ROTATE_RIGHT_1_8; // Default to rotation if we can't move backward
}

ActionType Controller::handleFacingAwayEvade(Tank &myTank) {
    // Try to move forward in the direction away from enemy
    auto [dx, dy] = Directions::directionToOffset(myTank.getDirection());
    int forwardX = myTank.getX() + dx;
    int forwardY = myTank.getY() + dy;
    
    if (isValidPosition(forwardX, forwardY)) {
        return ActionType::MOVE_FORWARD;
    } else {
        // If there's a wall and we can shoot, break it
        if (myTank.canShoot() && 
            (Global::board->getCellType(forwardX, forwardY) == CellType::WALL || 
             Global::board->getCellType(forwardX, forwardY) == CellType::WEAK_WALL)) {
            return ActionType::SHOOT;
        }
        // Otherwise, try to rotate to find another escape path
        return ActionType::ROTATE_RIGHT_1_8;
    }
}

ActionType Controller::calculateRotationDirection(Direction current, Direction desired) {
    int currentIdx = static_cast<int>(current);
    int desiredIdx = static_cast<int>(desired);
    int diff = (desiredIdx - currentIdx + 8) % 8;
    
    // Choose the shortest rotation direction
    if (diff == 4) {
        // 180 degrees, use 1/4 turn for faster rotation
        return ActionType::ROTATE_LEFT_1_4;
    } else if (diff < 4) {
        return ActionType::ROTATE_RIGHT_1_8;
    } else {
        return ActionType::ROTATE_LEFT_1_8;
    }
}

ActionType Controller::handleDistantEvade(Tank &myTank, Tank &enemyTank, 
                                          const Position &myPos, const Position &enemyPos, double distance) {
    auto [new_x, new_y] = myTank.moveForward();
    Position nextPos(new_x, new_y);
    
    if (nextPos.distanceTo(enemyPos) > distance && isValidPosition(new_x, new_y)) {
        return ActionType::MOVE_FORWARD;
    } else {
        // Try to find a better direction that increases distance
        Direction bestDir = findDirectionMaximizingDistance(myTank, enemyPos, distance);
        
        // If current direction is not best, rotate toward best
        if (bestDir != myTank.getDirection()) {
            return calculateRotationDirection(myTank.getDirection(), bestDir);
        }
        
        // Fall back to shoot if we can't move
        if (myTank.canShoot()) {
            return ActionType::SHOOT;
        }
    }
    
    return ActionType::ROTATE_RIGHT_1_8; // Last resort
}

Direction Controller::findDirectionMaximizingDistance(Tank &myTank, const Position &enemyPos, double currentDistance) {
    Direction bestDir = myTank.getDirection();
    double maxDist = currentDistance;
    
    for (Direction dir : Directions::getAllDirections()) {
        auto [dx, dy] = Directions::directionToOffset(dir);
        int testX = myTank.getX() + dx;
        int testY = myTank.getY() + dy;
        
        if (isValidPosition(testX, testY)) {
            Position testPos(testX, testY);
            double testDist = testPos.distanceTo(enemyPos);
            
            if (testDist > maxDist) {
                maxDist = testDist;
                bestDir = dir;
            }
        }
    }
    
    return bestDir;
}

bool Controller::isValidPosition(int x, int y) {
    // Check if position is in bounds and not a wall or mine
    if (x < 0 || x >= Global::board->getWidth() || 
        y < 0 || y >= Global::board->getHeight()) {
        return false;
    }
    
    CellType cellType = Global::board->getCellType(x, y);
    return cellType != CellType::WALL && 
           cellType != CellType::WEAK_WALL && 
           cellType != CellType::MINE;
}

Direction Controller::angleToClosestDirection(double angle) {
    // Define PI as a constant to avoid repeated calculations
    const double PI = 3.14159265358979323846;
    
    // Convert angle to 0..360 degrees
    double degrees = fmod((angle * 180.0 / PI) + 360.0, 360.0);
    // 8 directions split into 45° increments
    int dirIndex = static_cast<int>(std::round(degrees / 45.0)) % 8;
    // Map index to Direction
    switch (dirIndex) {
        case 0: return Direction::R;
        case 1: return Direction::DR;
        case 2: return Direction::D;
        case 3: return Direction::DL;
        case 4: return Direction::L;
        case 5: return Direction::UL;
        case 6: return Direction::U;
        default: return Direction::UR;
    }
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
