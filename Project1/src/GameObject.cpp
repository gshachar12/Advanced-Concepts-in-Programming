// src/GameObject.cpp

#include "GameObject.h"
#include "Board.h"  // We need the full definition of Board here

GameObject::GameObject(int startX, int startY, Direction startDir, CellType type)
    : position({startX, startY}), direction(startDir), ObjectType(type)
{}

void GameObject::setPosition(int newX, int newY) {
    if (position.size() < 2) position.resize(2);
    position[0] = newX;
    position[1] = newY;
}

void GameObject::setDirection(Direction newDir) {
    direction = newDir;
}

void GameObject::move(int dx, int dy)
{
int new_pos_x = (getX() + dx + Global::width) % Global::width;
int new_pos_y = (getY() + dy + Global::height) % Global::height;
setPosition(new_pos_x, new_pos_y);
}

Direction GameObject::stringToDirection(const std::string &str) {
    if (str == "U") return Direction::U;
    else if (str == "UR") return Direction::UR;
    else if (str == "R") return Direction::R;
    else if (str == "DR") return Direction::DR;
    else if (str == "D") return Direction::D;
    else if (str == "DL") return Direction::DL;
    else if (str == "L") return Direction::L;
    else if (str == "UL") return Direction::UL;
    else return Direction::U;
}
