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

void GameObject::move(int dx, int dy)
{
int new_pos_x = (position.x + dx + Global::width) % Global::width;
int new_pos_y = (position.y + dy + Global::height) % Global::height;
setPosition(new_pos_x, new_pos_y);
}
