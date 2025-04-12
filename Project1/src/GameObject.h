#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <vector>
#include <string>
#include <iostream>

// Example 8-direction enum
enum class Direction {
    U,  // Up
    UR, // Up-Right
    R,  // Right
    DR, // Down-Right
    D,  // Down
    DL, // Down-Left
    L,  // Left
    UL  // Up-Left
};

class GameObject {
protected:
    std::vector<int> position; // position[0] = x, position[1] = y
    Direction direction;

public:
    // Constructor: initialize 'position' using the two coordinates.
    GameObject(int startX, int startY, Direction startDir)
            : position({startX, startY}), direction(startDir)
    {}

    virtual ~GameObject() {}

    // Getters for entire position, or each coordinate separately.
    std::vector<int> getPosition() const { return position; }
    int getX() const { return (position.size() >= 1) ? position[0] : 0; }
    int getY() const { return (position.size() >= 2) ? position[1] : 0; }

    Direction getDirection() const { return direction; }

    // Setters for position and direction.
    void setPosition(int newX, int newY) {
        if (position.size() < 2) position.resize(2);
        position[0] = newX;
        position[1] = newY;
    }
    void setDirection(Direction newDir) { direction = newDir; }

    // A basic move function: moves by (dx,dy).
    virtual void move(int dx, int dy) {
        if (position.size() < 2) position.resize(2);
        position[0] += dx;
        position[1] += dy;
    }

    // A basic update function.
    virtual void update() {
        // default: do nothing
    }

    // --- NEW helper: Convert string to Direction ---
    // This lets you keep Tank's constructor parameter as a string.
    static Direction stringToDirection(const std::string &str) {
        if (str == "U")  return Direction::U;
        else if (str == "UR") return Direction::UR;
        else if (str == "R")  return Direction::R;
        else if (str == "DR") return Direction::DR;
        else if (str == "D")  return Direction::D;
        else if (str == "DL") return Direction::DL;
        else if (str == "L")  return Direction::L;
        else if (str == "UL") return Direction::UL;
        else return Direction::U; // Default fallback
    }
};

#endif // GAMEOBJECT_H
