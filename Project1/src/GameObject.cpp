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

