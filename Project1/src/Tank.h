#ifndef TANK_H
#define TANK_H
#include "CellType.h"
#include "GameObject.h"
#include <algorithm>
#include <iostream>
#include <vector>

// For backward movement state:
enum class BackwardState {
    NOT_REQUESTED,
    WAITING_1,
    WAITING_2,
    MOVING_BACKWARD
};

class Tank : public GameObject {
private:
    bool alive;
    int shellCount;        // e.g., 16 initial shells
    int shootCooldown;     // Cooldown steps after shooting
    BackwardState backwardState;

    // Optional: valid direction strings if you want to add rotation helpers.
    static const std::vector<std::string> validDirections;

    // Helper to convert a Direction enum to a string (for printing)
    static std::string directionToString(Direction dir) {
        switch(dir) {
            case Direction::U:  return "U";
            case Direction::UR: return "UR";
            case Direction::R:  return "R";
            case Direction::DR: return "DR";
            case Direction::D:  return "D";
            case Direction::DL: return "DL";
            case Direction::L:  return "L";
            case Direction::UL: return "UL";
        }
        return "";
    }

public:
    // Constructor: Accepts a string for direction and converts it.
    Tank(int x, int y, const std::string &dir, CellType ObjectType)
            : GameObject(x, y, GameObject::stringToDirection(dir), ObjectType),
              alive(true),
              shellCount(16),
              shootCooldown(0),
              backwardState(BackwardState::NOT_REQUESTED)
    {}

    void update() override;

    bool isAlive() const { return alive; }
    void destroy() { alive = false; }

    int getShellCount() const { return shellCount; }
    int getShootCooldown() const { return shootCooldown; }
    bool canShoot() const { return (shootCooldown == 0 && shellCount > 0 && alive); }
    BackwardState getBackwardState() const { return backwardState; }

    void shoot();          // Shoot
    void moveForward();    // Move 1 step in current direction.
    void moveBackward();   // Move 1 step in opposite direction.

    void requestBackward();
    void cancelBackward();

    void printPosition();
    void printStatus() const;

    // (Optional) Rotation helpers (not mandatory if you don't need them)
    void rotateLeft1_8();
    void rotateRight1_8();
    void rotateLeft1_4();
    void rotateRight1_4();

private:
    // Helpers for rotation
    int findDirectionIndex(const std::string &d) const;
    void setDirectionByIndex(int idx);
    std::pair<int,int> directionToOffset(const std::string &dirStr) const;
};

#endif // TANK_H
