#ifndef POSITION_H
#define POSITION_H

#include <cmath>    // For sqrt
#include <iostream> // Optional, for debugging (e.g., operator<<)

struct Position {
    int x;
    int y;

    Position() : x(0), y(0) {}
    Position(int _x, int _y) : x(_x), y(_y) {}

    // Comparison operators
    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Position& other) const {
        return !(*this == other);
    }

    // For use in std::set, std::map, etc.
    bool operator<(const Position& other) const {
        return (y < other.y) || (y == other.y && x < other.x);
    }

    // Arithmetic operators
    Position operator+(const Position& other) const {
        return Position{x + other.x, y + other.y};
    }

    Position operator-(const Position& other) const {
        return Position{x - other.x, y - other.y};
    }
    // Getters
    int getX() const { return x; }
    int getY() const { return y; }

    // Setters
    void setX(int _x) { x = _x; }
    void setY(int _y) { y = _y; }

    // Manhattan distance (grid distance)
    int manhattanDistance(const Position& other) const;

    // Euclidean distance (direct line)
    double euclideanDistance(const Position& other) const;

    // Check if in a given range of a center
    bool isInRange(const Position& center, int range) const;

    // Offset position
    Position offset(int dx, int dy) const;

    // Bounds checking
    bool isWithinBounds(int maxX, int maxY) const;

    // Keep position inside grid
    void clamp(int minVal, int maxVal);

    // Wrap around the grid edges (e.g., Pac-Man)
    void wrap(int maxX, int maxY);
};

#endif // POSITION_H
