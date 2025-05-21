#include "Shell.h"
#include <iostream>

void Shell::update(Board& board) {
    // Get direction offset
    auto [dx, dy] = Directions::directionToOffset(direction);
    
    // Calculate new position
    auto [newX, newY] = tryToMove(board, dx, dy);
    
    // Check if the new position is valid
    CellType targetCell = board.getCellType(newX, newY);
    
    if (targetCell == CellType::WALL || targetCell == CellType::WEAK_WALL) {
        // Shell hits wall, weaken it
        board.weakenWall(newX, newY);
        
        // Shell explodes
        explode(board);
        
        // Shell is destroyed (by changing its type)
        setObjectType(CellType::BOOM);
    }
    else if (targetCell == CellType::TANK1 || targetCell == CellType::TANK2) {
        // Shell hits tank (will be handled by GameManager's collision detection)
        explode(board);
        setObjectType(CellType::BOOM);
    }
    else if (targetCell == CellType::MINE) {
        // Shell hits mine, both explode
        explode(board);
        board.setCellType(newX, newY, CellType::EMPTY);
        setObjectType(CellType::BOOM);
    }
    else if (targetCell == CellType::EMPTY) {
        // Shell moves to empty cell
        move(board, dx, dy);
    }
    else {
        // Shell hits another object or is out of bounds
        explode(board);
        setObjectType(CellType::BOOM);
    }
}

bool Shell::collidesWith(const GameObject& obj) const {
    // Check if the shell and object are at the same position
    return (getX() == obj.getX() && getY() == obj.getY());
}

void Shell::explode(Board& board) {
    // Simple explosion: just mark the current position
    // In a more advanced implementation, you might affect surrounding cells
    board.setCellType(getX(), getY(), CellType::BOOM);
    
    // Could add logic to damage nearby objects based on explosion radius
    std::cout << "Shell exploded at (" << getX() << ", " << getY() << ")" << std::endl;
}