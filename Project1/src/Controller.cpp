#include "Controller.h"
#include "Board.h"
#include "Position.h"
#include <iostream>
#include <fstream>
#include <queue>


int Controller::count = 0; // Initialize static variable

ActionType Controller::pickEvadeDirection(Board board, Tank &myTank, Tank &enemyTank)
{
    // Create Position objects for the tanks
    Position myPos(myTank.getX(), myTank.getY());
    Position enemyPos(enemyTank.getX(), enemyTank.getY());

    // Avoid stepping on a mine.
    if (IsMineNearby(board, myTank)) {
        return ActionType::ROTATE_RIGHT_1_8;
    }

    if(IsTankNearby( myTank, enemyTank)) {
        return ActionType::SHOOT; 
    }
    
    // Check if enemy is close enough to evade
    double distance = myPos.distanceTo(enemyPos);

    // If enemy is within 4 units, evade
    if (distance < 4.0) {
        return handleCloseEvade(board, myTank, enemyTank, myPos, enemyPos, distance);
    }
    
    // If enemy is not close enough, still try to increase distance
    return handleDistantEvade(board,myTank, enemyTank, myPos, enemyPos, distance);
}

ActionType Controller::handleCloseEvade(Board board, Tank &myTank, Tank &enemyTank, 
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

    Direction desired = Directions::angleToClosestDirection(evadeAngle);
    Direction current = myTank.getDirection();
    
    // Check if we're already facing the enemy
    Direction enemyDir = Directions::angleToClosestDirection(angleToEnemy);
    bool facingEnemy = (current == enemyDir);
    
    // If we're facing the enemy, moving backward is most efficient
    if (facingEnemy) {
        return handleFacingEnemyEvade(board, myTank);
    }
    
    // Check if facing away from enemy (ideal for moving forward)
    bool facingAway = (current == desired);
    
    if (facingAway) {
        return handleFacingAwayEvade(board, myTank);
    }
    
    // We need to rotate toward the desired direction
    return calculateRotationDirection(current, desired);
}

ActionType Controller::handleFacingEnemyEvade(Board board, Tank &myTank) {
    // Check if backward move is valid
    auto [dx, dy] = Directions::directionToOffset(myTank.getDirection());
    int backX = myTank.getX() - dx;
    int backY = myTank.getY() - dy;
    
    if (isValidPosition(board, backX, backY)) {
        return ActionType::MOVE_BACKWARD;
    }
    
    return ActionType::ROTATE_RIGHT_1_8; // Default to rotation if we can't move backward
}

ActionType Controller::handleFacingAwayEvade(Board board, Tank &myTank) {
    // Try to move forward in the direction away from enemy
    auto [dx, dy] = Directions::directionToOffset(myTank.getDirection());
    int forwardX = myTank.getX() + dx;
    int forwardY = myTank.getY() + dy;
    
    if (isValidPosition(board, forwardX, forwardY)) {
        return ActionType::MOVE_FORWARD;
    } else {
        // If there's a wall and we can shoot, break it
        if (myTank.canShoot() && 
            (board.getCellType(forwardX, forwardY) == CellType::WALL || 
             board.getCellType(forwardX, forwardY) == CellType::WEAK_WALL)) {
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

ActionType Controller::handleDistantEvade(Board board, Tank &myTank, Tank &enemyTank, 
                                          const Position &myPos, const Position &enemyPos, double distance) {
    auto [new_x, new_y] = myTank.moveForward(board);
    Position nextPos(new_x, new_y);

    if (nextPos.distanceTo(enemyPos) > distance && isValidPosition(board, new_x, new_y))
    {
        return ActionType::MOVE_FORWARD;
    }
    else
    {
        // Try to find a better direction that increases distance
        Direction bestDir = findDirectionMaximizingDistance(board, myTank, enemyPos, distance);
        
        // If current direction is not best, rotate toward best
        if (bestDir != myTank.getDirection()) {
            return calculateRotationDirection(myTank.getDirection(), bestDir);
        }

        // Fall back to shoot if we can't move
        if (myTank.canShoot())
        {
            return ActionType::SHOOT;
        }
    }

    return ActionType::ROTATE_RIGHT_1_8; // Last resort
}

Direction Controller::findDirectionMaximizingDistance(Board board, Tank &myTank, const Position &enemyPos, double currentDistance) {
    Direction bestDir = myTank.getDirection();
    double maxDist = currentDistance;
    
    for (Direction dir : Directions::getAllDirections()) {
        auto [dx, dy] = Directions::directionToOffset(dir);
        int testX = myTank.getX() + dx;
        int testY = myTank.getY() + dy;
        
        if (isValidPosition(board, testX, testY)) {
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




Position Controller::BFS(Board board, Position chaserStart, Position target)
{
    std::queue<Position> q;
    Position nextPos ;
    Position current;
    std::vector<std::vector<bool>> visited(board.getHeight(), std::vector<bool>(board.getWidth(), false));
    q.push(chaserStart);
    visited[chaserStart.y][chaserStart.x] = true;  
    // To track the direction to take based on the BFS pathfinding
    std::vector<std::vector<Position>> parent(board.getHeight(), std::vector<Position>(board.getWidth(), {-1, -1}));
    // BFS exploration to find the shortest path
    while (!q.empty())
    {
        current = q.front();
        q.pop();
        // Explore all possible directions (8 directions)
        for (auto &dir : Directions::getAllDirections())
        {
            // Get the (dx, dy) offset for the current direction
            auto [dx, dy] = Directions::directionToOffset(dir);
            // Calculate the neighbor position based on the current position
            Position neighbor = {current.x + dx, current.y + dy};
            // Check if the neighbor is within bounds, not visited, and not an obstacle (wall or mine)
            if (neighbor.x >= 0 && neighbor.x < board.getWidth() &&
                neighbor.y >= 0 && neighbor.y < board.getHeight() &&
                !visited[neighbor.y][neighbor.x] &&
                board.getCellType(neighbor.x, neighbor.y) != CellType::WALL &&
                board.getCellType(neighbor.x, neighbor.y) != CellType::MINE)
            {           
                visited[neighbor.y][neighbor.x] = true;
                parent[neighbor.y][neighbor.x] = current;
                q.push(neighbor);   
            }
        }
    }
    current = target;  // find the branch of the bfs tree that directed to the target
    while (parent[current.y][current.x] != chaserStart)
        current = parent[current.y][current.x] ;
    nextPos = current; 
    return nextPos;
}


ActionType Controller::AvoidShells(Board board, Tank &myTank, const std::vector<Shell> &shells)
{
    for (const Shell &shell : shells)
    {
        
        if (shell.isTargeting(myTank))
        {
            // If a shell is targeting my tank, dodge it
            if (IsSafeToMoveForward(board, myTank))
            {
                return ActionType::MOVE_FORWARD;
            }
            else
            {
                return ActionType::ROTATE_RIGHT_1_8;
            }
        }
    }
    return ActionType::MOVE_FORWARD;

}

ActionType Controller::ChaseTank(Board board, 
    Tank &myTank,
    Tank &enemyTank,
    const std::vector<Shell> &shells)
{
    // if (IsTankNearby(myTank, enemyTank) && CanShoot(myTank, enemyTank))
    //     return ActionType::SHOOT;
    // If we're not at the target yet, check if we can move forward or need to rotate
    if (count % 2 ==0) // calculate BFS every 4 steps
    {
        Position chaserStart = myTank.getPosition();
        Position target = enemyTank.getPosition();                                          
        Position nextPos = BFS(board, chaserStart, target);                                // Get the next position in the path
        Direction currentDirection = myTank.getDirection();                                // Get the current direction of the chaser
        Direction targetDirection = Directions::OffsetToDirection(nextPos - chaserStart ); // Calculate the target direction
        // If the chaser is not facing the correct direction, rotate
        CellType nextCell  = board.getCellType(nextPos.x, nextPos.y);
        // Rotate to face the target direction
        if (currentDirection < targetDirection)
            return ActionType::ROTATE_RIGHT_1_8;
        if (currentDirection > targetDirection)
            return ActionType::ROTATE_LEFT_1_8;
        if(nextCell == CellType::WALL||nextCell == CellType::WEAK_WALL)
            return ActionType::SHOOT;
    }   
    count++;
    return AvoidShells(board, myTank, shells);     // Try to avoid shells
}


bool Controller::isValidPosition(Board board, int x, int y)
{
    // Check if position is in bounds and not a wall or mine
    if (x < 0 || x >= board.getWidth() ||
        y < 0 || y >= board.getHeight())
    {
        return false;
    }

    CellType cellType = board.getCellType(x, y);
    return cellType != CellType::WALL &&
           cellType != CellType::WEAK_WALL &&
           cellType != CellType::MINE;
}

// Helper function to check if we can shoot the opponent's tank
bool Controller::CanShoot(Tank &myTank, Tank &enemyTank)
{
    // Check if the enemy tank is within range and line of sight (this is a placeholder logic)
    return IsInLineOfSight( myTank, enemyTank); //&& IsInRange(enemyTank);
}

// Helper function to check if the shell is targeting the tank
bool Controller::IsShellChasingTank(const Shell &shell, const Tank &myTank)
{
    // Implement logic to check if a shell is heading toward the tank
    return shell.isTargeting(myTank);
}

// Helper function to check if the tank can safely move forward (e.g., no obstacles or mines)
bool Controller::IsSafeToMoveForward(Board board,Tank &myTank)
{
    // Check if the next position is clear and doesn't contain a mine
    return !IsMineNearby(board, myTank);//&& !IsObstacleAhead(myTank);
}

// Helper function to check if there's a mine nearby
bool Controller::IsMineNearby(Board board, Tank &myTank)
{
    // Implement logic to check if the tank is about to step on a mine (based on known mine locations)
    auto [new_x, new_y] = myTank.moveForward(board);
    return board.getCellType(new_x, new_y) == CellType::MINE;
}

// Helper function to check if there's a tank nearby
bool Controller::IsTankNearby(Tank &myTank, Tank &enemyTank)
{
    // Check if the tank is about to step on another tank
    int distance = myTank.getPosition().distanceTo(enemyTank.getPosition()); 
 
    return distance <= 4;
}


 // // Helper function to check if the opponent's tank is in line of sight
bool Controller::IsInLineOfSight(Tank &myTank, Tank &enemyTank)
{
    // Implement line of sight check
    return true;
}

// // Helper function to check if the opponent's tank is within shooting range
// bool Controller::IsInRange(const Tank &enemyTank)
// {
//     return true; // Placeholder for actual range logic
// }

// // Helper function to check if there's an obstacle ahead
// bool Controller::IsObstacleAhead(const Tank &myTank)
// {
//     return false; // Placeholder for actual obstacle check
// }