#ifndef TANK_H
#define TANK_H
#include "CellType.h"
#include "GameObject.h"
#include "Directions.h"
#include "Board.h"
#include <algorithm>
#include <iostream>
#include <vector>

class Board;
// For backward movement state:
enum class BackwardState {
    NOT_REQUESTED,
    WAITING_1,
    WAITING_2,
    MOVING_BACKWARD
};

// Constants for tank properties
extern const int DEFAULT_SHELL_COUNT;
extern const int SHOOT_COOLDOWN;

class Tank : public GameObject {
private:
    bool alive;
    int shellCount;        // e.g., 16 initial shells
    int shootCooldown;     // Cooldown steps after shooting
    int tankID; 
    BackwardState backwardState;
public:
    // Constructor: Accepts a string for direction and converts it.
    Tank(int x, int y, const std::string &dir, CellType ObjectType, int ID)
            : GameObject(x, y, Directions::stringToDirection(dir), ObjectType),
              alive(true),
              shellCount(DEFAULT_SHELL_COUNT),
              shootCooldown(0),
              tankID(ID),
              backwardState(BackwardState::NOT_REQUESTED)

    {}

    void update();
    virtual ~Tank();
    bool isAlive() const { return alive; }
    void destroy() { alive = false;  
    }
    int getTankID() const { return tankID; }
    int getShellCount() const { return shellCount; }
    int getShootCooldown() const { return shootCooldown; }
    bool canShoot() const { return (shootCooldown == 0 && shellCount > 0 && alive); }
    BackwardState getBackwardState() const { return backwardState; }

    void shoot();          // Shoot
    std::pair<int,int>  moveForward(Board board);    // Move 1 step in current direction.
    std::pair<int,int>  moveBackward(Board board);   // Move 1 step in opposite direction.

    void requestBackward();
    void cancelBackward();

    void printPosition();
    void printStatus();

    // (Optional) Rotation helpers (not mandatory if you don't need them)
    void rotateLeft1_8();
    void rotateRight1_8();
    void rotateLeft1_4();
    void rotateRight1_4();

private:
    // Helpers for rotation
    int findDirectionIndex(const Direction &d) const;
    void setDirectionByIndex(int idx);
};

#endif // TANK_H
