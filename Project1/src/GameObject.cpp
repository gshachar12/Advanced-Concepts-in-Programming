// src/GameObject.cpp

#include "GameObject.h"
#include "Board.h"  
#include "Position.h"
GameObject::GameObject(int startX, int startY, Direction startDir, CellType type)
    : position(startX, startY), direction(startDir), ObjectType(type)
{}

void GameObject::setPosition(int newX, int newY) {
    position.x = newX;
    position.y = newY;
}

void GameObject::setDirection(Direction newDir) {
    direction = newDir;
}

void GameObject::move(Board board, int dx, int dy)
{
int new_pos_x = (position.x + dx + board.getWidth()) % board.getWidth();
int new_pos_y = (position.y + dy + board.getHeight()) % board.getHeight();
setPosition(new_pos_x, new_pos_y);
}

std::pair<int, int> GameObject::tryToMove(Board board, int dx, int dy)
{
int new_pos_x = (position.x + dx + board.getWidth()) % board.getWidth();
int new_pos_y = (position.y + dy + board.getHeight()) % board.getHeight();
return {new_pos_x, new_pos_y};
}


// Checks if the shell is targeting the given obj.
bool GameObject::isTargeting(const GameObject& obj) const {
    int shellX = getX(); 
    int shellY = getY(); 

    // Get the obj position
    int objX = obj.getX();
    int objY = obj.getY();

    // Get direction offset based on the shell's direction
    auto [dx, dy] = Directions::directionToOffset(direction);

    // Check if the obj is in the same line of movement
    
    // Check horizontal targeting
    if (dx != 0 && shellY == objY) {
        if ((dx > 0 && shellX < objX) || (dx < 0 && shellX > objX)) {
            return true;  // Shell is heading toward obj horizontally
        }
    }

    // Check vertical targeting
    if (dy != 0 && shellX == objX) {
        if ((dy > 0 && shellY < objY) || (dy < 0 && shellY > objY)) {
            return true;  // Shell is heading toward obj vertically
        }
    }

    // Check diagonal targeting
    if (dx != 0 && dy != 0) {
        if (std::abs(shellX - objX) == std::abs(shellY - objY)) {
            if ((dx > 0 && dy > 0 && shellX < objX && shellY < objY) ||
                (dx < 0 && dy < 0 && shellX > objX && shellY > objY) ||
                (dx > 0 && dy < 0 && shellX < objX && shellY > objY) ||
                (dx < 0 && dy > 0 && shellX > objX && shellY < objY)) {
                return true;  // Shell is heading toward the obj diagonally
            }
        }
    }

    return false;  // Not targeting
}
