#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#pragma once

#include <vector>
#include <string>
#include "Globals.h"
#include "CellType.h"

enum class Direction {
    U, UR, R, DR, D, DL, L, UL
};

class GameObject {
protected:
    std::vector<int> position;
    Direction direction;
    CellType ObjectType;

public:
    GameObject(int startX, int startY, Direction startDir, CellType type);
    virtual ~GameObject() {}

    std::vector<int> getPosition() const { return position; }
    int getX() const { return (position.size() >= 1) ? position[0] : 0; }
    int getY() const{ return (position.size() >= 2) ? position[1] : 0; }
    Direction getDirection() const { return direction; }
    CellType getObjectType() const {return ObjectType; }
    void setObjectType(CellType objecType)  { ObjectType = objecType; }
    void move(int dx, int dy); 
    void setPosition(int newX, int newY);
    void setDirection(Direction newDir);

    static Direction stringToDirection(const std::string &str);
};

#endif // GAMEOBJECT_H
