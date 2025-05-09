#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#pragma once

#include <vector>
#include <string>
#include "Position.h"
#include "Board.h"
#include "CellType.h"
#include "Directions.h"

class Board;

class GameObject {
protected:
    Position position;
    Direction direction;
    CellType ObjectType;

public:
    GameObject(int startX, int startY, Direction startDir, CellType type);
    virtual ~GameObject() {}
    Position getPosition() const { return position; }
    int getX()  { return  position.getX(); }
    int getY() { return position.getY(); }
    Direction getDirection() const { return direction; }
    CellType getObjectType() const {return ObjectType; }
    std::pair<int, int> tryToMove(Board board, int dx, int dy);
    void setObjectType(CellType objecType)  { ObjectType = objecType; }
    void move(Board board, int dx, int dy); 
    void setPosition(int newX, int newY);
    void setDirection(Direction newDir);
    bool isTargeting( GameObject &tank) ;

};

#endif // GAMEOBJECT_H
