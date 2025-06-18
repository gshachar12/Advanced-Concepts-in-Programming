#include "Controller.h"
#include "Board.h"
#include "Position.h"
#include <iostream>
#include <fstream>
#include <queue>

// Constants for Controller behavior
const double EVADE_DISTANCE_THRESHOLD = 4.0;  // Distance threshold for evasion
const int BFS_CALCULATION_INTERVAL = 1;       // How often to calculate BFS (every n steps)
const int MAX_DIRECTIONS = 8;                 // Number of possible directions
const int HALF_ROTATION_INDEX = 4;            // Half rotation index (180 degrees)
const int EVADE_STALL = 1;
const int AVOID_FLAG = 1;


int Controller::count = 0; // Initialize static variable
int Controller::evadeStall = 0; // To allow rotating and then advancing
int Controller::rotateFlag = 0; // To allow rotating and then advancing when running away from tank
int Controller::avoidFlag = 0; // To allow rotating and then advancing when running away from tank

ActionType Controller::EvadeTank(Board board, Tank &myTank, Tank &enemyTank,  std::vector<Shell> &shells)
{
    // Calculate the relative position of the enemy tank
    Position myPos = myTank.getPosition();
    Position enemyPos = enemyTank.getPosition();
    
    // Calculate the vector pointing from my tank to the enemy tank
    int dx = enemyPos.getX() - myPos.getX();
    int dy = enemyPos.getY() - myPos.getY();
    if (IsTankNearby( myTank, enemyTank))
        {
            if (rotateFlag == 0)
            {
                rotateFlag ++;
                return ActionType::ROTATE_LEFT_1_8; 
            }
            else {
                rotateFlag --;
                return ActionType::MOVE_FORWARD; 
            }
        }
    if (IsTankNearby( myTank, enemyTank))
        {
            rotateFlag = 1;
            return ActionType::ROTATE_LEFT_1_8; 
        }
     
    if (IsMineNearby(board, myTank))
        return ActionType::ROTATE_LEFT_1_4;   

    if (IsWallAhead(board, myTank))
        return ActionType::ROTATE_LEFT_1_4; 

    // Check if the enemy tank is in line of sight
    if (IsInLineOfSight(myTank, enemyTank) && evadeStall != EVADE_STALL) {
        // The enemy is targeting us, so we need to move
        
        std::cout << "Enemy tank is in line of sight! Evasion required!" << std::endl;
        evadeStall = EVADE_STALL; 

        // Move in the opposite direction of the enemy
        if (dx > 0) {
            return ActionType::ROTATE_LEFT_1_8;  // Rotate left to move away from the enemy
        }
        else if (dx < 0) {
            return ActionType::ROTATE_RIGHT_1_8;  // Rotate right to move away from the enemy
        }
        else if (dy > 0) {
            return ActionType::ROTATE_LEFT_1_8;  // Rotate to avoid downward movement
        }
        else {
            return ActionType::ROTATE_RIGHT_1_8;  // Rotate to avoid upward movement
        }
    }
    else {
    evadeStall =0; 
 
    }
    // If enemy is not in line of sight, we can safely move forward
    return AvoidShells(board, myTank, shells);
}


// New helper method to check if any valid move is possible
bool Controller::isAnyValidMovePossible(Board board, Tank &myTank) {
    // Check if any direction allows movement
    for (Direction dir : Directions::getAllDirections()) {
        auto [dx, dy] = Directions::directionToOffset(dir);
        int testX = myTank.getX() + dx;
        int testY = myTank.getY() + dy;
        
        if (isValidPosition(board, testX, testY)) {
            return true;
        }
    }
    return false;
}

ActionType Controller::calculateRotationDirection(Direction current, Direction desired) {
    int currentIdx = static_cast<int>(current);
    int desiredIdx = static_cast<int>(desired);
    int diff = (desiredIdx - currentIdx + MAX_DIRECTIONS) % MAX_DIRECTIONS;
    
    // Choose the shortest rotation direction
    if (diff == HALF_ROTATION_INDEX) {
        // 180 degrees, use 1/4 turn for faster rotation
        return ActionType::ROTATE_LEFT_1_4;
    } else if (diff < HALF_ROTATION_INDEX) {
        return ActionType::ROTATE_RIGHT_1_8;
    } else {
        return ActionType::ROTATE_LEFT_1_8;
    }
}


Position Controller::BFS(Board board, Position chaserStart, Position target)
{
    std::queue<Position> q;
    Position nextPos ;
    Position current;
    std::vector<std::vector<bool>> visited(board.getHeight(), std::vector<bool>(board.getWidth(), false));
    q.push(chaserStart);
    visited[chaserStart.getY()][chaserStart.getX()] = true;  
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
            Position neighbor = {current.getX() + dx, current.getY() + dy};
            // Check if the neighbor is within bounds, not visited, and not an obstacle (wall or mine)
            if (neighbor.getX() >= 0 && neighbor.getX() < board.getWidth() &&
                neighbor.getY() >= 0 && neighbor.getY() < board.getHeight() &&
                !visited[neighbor.getY()][neighbor.getX()] &&
                board.getCellType(neighbor.getX(), neighbor.getY()) != CellType::WALL &&
                board.getCellType(neighbor.getX(), neighbor.getY()) != CellType::MINE)
            {           
                visited[neighbor.getY()][neighbor.getX()] = true;
                parent[neighbor.getY()][neighbor.getX()] = current;
                q.push(neighbor);   
            }
        }
    }
    current = target;  // find the branch of the bfs tree that directed to the target
    while (parent[current.getY()][current.getX()] != chaserStart)
        current = parent[current.getY()][current.getX()] ;
    nextPos = current; 
    return nextPos;
}

ActionType Controller::AvoidShells(Board board, Tank &myTank,  std::vector<Shell> &shells)
{
    for ( Shell &shell : shells)
    {
        
        if (shell.isTargeting(myTank) &&  avoidFlag != AVOID_FLAG)
        {
                avoidFlag = AVOID_FLAG; 
            // If a shell is targeting my tank, dodge it
                return ActionType::ROTATE_RIGHT_1_4;
            
        }

    }   
    avoidFlag = 0; 
    if (IsWallAhead(board, myTank))
        return ActionType::SHOOT; 
    else

        return ActionType::MOVE_FORWARD;

}

ActionType Controller::ChaseTank(Board board, Tank &myTank, Tank &enemyTank,  std::vector<Shell> &shells)
{
    if(IsTankAhead(board, myTank))
        {
            return ActionType::SHOOT;
        }
    
    // If we're not at the target yet, check if we can move forward or need to rotate
    if (count % BFS_CALCULATION_INTERVAL == 0) // calculate BFS based on interval
    {
        Position chaserStart = myTank.getPosition();
        Position target = enemyTank.getPosition();                                          
        Position nextPos = BFS(board, chaserStart, target);                                // Get the next position in the path
        Direction currentDirection = myTank.getDirection();                                // Get the current direction of the chaser
        Direction targetDirection = Directions::OffsetToDirection(nextPos - chaserStart ); // Calculate the target direction
        // If the chaser is not facing the correct direction, rotate
        CellType nextCell  = board.getCellType(nextPos.getX(), nextPos.getY());
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

// Helper function to check if the tank can safely move forward (e.g., no obstacles or mines)
bool Controller::IsSafeToMoveForward(Board board,Tank &myTank)
{
    // Check if the next position is clear and doesn't contain a mine
    return !IsMineNearby(board, myTank);//&& !IsObstacleAhead(myTank);
}

// Helper function to check if there's a tank ahead
bool Controller::IsTankAhead(Board board, Tank &myTank)
{
    // Check the forward position for mines
    auto [checkX, checkY] = myTank.moveForward(board);
        
        //If the next move is a wall
        if (
            board.getCellType(checkX, checkY) == CellType::TANK1 ||
            board.getCellType(checkX, checkY) == CellType::TANK2 ) {
            return true;
        }
    return false;
    
}

// Helper function to check if there's a wall ahead
bool Controller::IsWallAhead(Board board, Tank &myTank)
{
    // Check the forward position for mines
    auto [checkX, checkY] = myTank.moveForward(board);
        
        //If the next move is a wall
        if (
            board.getCellType(checkX, checkY) == CellType::WALL ||
            board.getCellType(checkX, checkY) == CellType::WEAK_WALL ) {
            return true;
        }
    return false;
    
}
// Helper function to check if there's a mine nearby
bool Controller::IsMineNearby(Board board, Tank &myTank)
{
    // Check the forward position for mines
    auto [new_x, new_y] = myTank.moveForward(board);
    
    // Also check surrounding cells for mines to be more cautious
    for (Direction dir : Directions::getAllDirections()) {
        auto [dx, dy] = Directions::directionToOffset(dir);
        int checkX = myTank.getX() + dx;
        int checkY = myTank.getY() + dy;
        
        // If any surrounding cell has a mine and it's in the direction we're heading
        if (checkX == new_x && checkY == new_y && 
            board.getCellType(checkX, checkY) == CellType::MINE) {
            return true;
        }
    }
    
    return board.getCellType(new_x, new_y) == CellType::MINE;
}

// Helper function to check if there's a tank nearby
bool Controller::IsTankNearby(Tank &myTank, Tank &enemyTank)
{
    // Check if the tank is about to step on another tank
    int distance = myTank.getPosition().distanceTo(enemyTank.getPosition()); 

    return distance <= EVADE_DISTANCE_THRESHOLD;
}

 // Helper function to check if the opponent's tank is in line of sight
bool Controller::IsInLineOfSight(Tank &myTank, Tank &enemyTank)
{
    // For the evading tank, we don't need to shoot at the enemy, so return false
        // Assuming we can check if the enemy tank's direction is aligned with my tank
    Direction enemyDirection = enemyTank.getDirection();
    Position enemyPos = enemyTank.getPosition();
    Position myPos = myTank.getPosition();
    Position displacement = (myPos-enemyPos);
    displacement.normalize();
   

    // Check if enemy tank's cannon is pointing at our tank's position
    if (enemyDirection == Directions::OffsetToDirection(displacement)) {
        return true;  // Enemy tank is facing my tank
    }

        // Check if enemy tank's cannon is pointing at our tank's position
    if (enemyDirection == Directions::OffsetToDirection(displacement*-1)) {
        return true;  // Enemy tank is facing my tank
    }

    return false;  // Enemy is not in line of sight
}

// // Helper function to check if the opponent's tank is within shooting range
// bool Controller::IsInRange(const Tank &enemyTank)
// {
//     return true; // Placeholder for actual range logic
// }