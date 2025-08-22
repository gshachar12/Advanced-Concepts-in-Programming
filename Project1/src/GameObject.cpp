// src/GameObject.cpp

#include "GameObject.h"
#include "Board.h"  
#include "Position.h"
GameObject::GameObject(int startX, int startY, Direction startDir, CellType type)
    : position(startX, startY), direction(startDir), ObjectType(type)
{}

void GameObject::setPosition(int newX, int newY) {
    position.setX( newX);
    position.setY( newY);
}

void GameObject::setDirection(Direction newDir) {
    direction = newDir;
}

void GameObject::move(Board board, int dx, int dy)
{
int new_pos_x = (position.getX() + dx + board.getWidth()) % board.getWidth();
int new_pos_y = (position.getY() + dy + board.getHeight()) % board.getHeight();
setPosition(new_pos_x, new_pos_y);
}

std::pair<int, int> GameObject::tryToMove(Board board, int dx, int dy)
{
int new_pos_x = (position.getX() + dx + board.getWidth()) % board.getWidth();
int new_pos_y = (position.getY() + dy + board.getHeight()) % board.getHeight();
return {new_pos_x, new_pos_y};
}


// Checks if this object is targeting the given obj.
bool GameObject::isTargeting( GameObject& obj)  {
    int thisX =  getX(); 
    int thisY = getY(); 
    // Get the obj position
    int objX = obj.getX();
    int objY = obj.getY();

    // Get direction offset based on the object direction
    auto [dx, dy] = Directions::directionToOffset(direction);

    // Check if the obj is in the same line of movement
    //  >      1
    // Check horizontal targeting
    if (dx != 0 && dy==0 && thisY == objY) {
            return true;  // This object is heading toward obj horizontally
    }

    // Check vertical targeting
    if (dy != 0 && dx ==0 && thisX == objX) {
            return true;  // Shell is heading toward obj vertically
    }

    // Check diagonal targeting
    if (dx != 0 && dy != 0) {
        if (std::abs(thisX - objX) == std::abs(thisY - objY)) {
            if ((dx > 0 && dy > 0 && thisX < objX && thisY < objY) ||
                (dx < 0 && dy < 0 && thisX > objX && thisY > objY) ||
                (dx > 0 && dy < 0 && thisX < objX && thisY > objY) ||
                (dx < 0 && dy > 0 && thisX > objX && thisY < objY)) {
                return true;  // this is heading toward the obj diagonally
            }
        }
    }

    return false;  // Not targeting
}
