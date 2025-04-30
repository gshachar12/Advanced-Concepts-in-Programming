#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#pragma once

#include <vector>
#include <string>
#include "Position.h"

#include "Globals.h"
#include "CellType.h"
#include "Directions.h"

class GameObject {
protected:
    Position position;
    Direction direction;
    CellType ObjectType;

public:
    GameObject(int startX, int startY, Direction startDir, CellType type);
    virtual ~GameObject() {}
    Position getPosition() const { return position; }
    int getX() const { return  position.x; }
    int getY() const{ return position.y; }
    Direction getDirection() const { return direction; }
    CellType getObjectType() const {return ObjectType; }
    void setObjectType(CellType objecType)  { ObjectType = objecType; }
    void move(int dx, int dy); 
    void setPosition(int newX, int newY);
    void setDirection(Direction newDir);

};

#endif // GAMEOBJECT_H
