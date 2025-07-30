#ifndef POSITION_H
#define POSITION_H

#include <cmath>

/**
 * Represents a position on the game board
 */
struct Position {
    int x;
    int y;
    
    Position() : x(0), y(0) {}
    Position(int x_pos, int y_pos) : x(x_pos), y(y_pos) {}
    
    // Equality operator
    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }
    
    // Addition operator for movement
    Position operator+(const Position& other) const {
        return Position(x + other.x, y + other.y);
    }
    
    // Subtraction operator for distance calculation
    Position operator-(const Position& other) const {
        return Position(x - other.x, y - other.y);
    }
    
    // Calculate Manhattan distance to another position
    int distanceTo(const Position& other) const {
        return abs(x - other.x) + abs(y - other.y);
    }
    
    // Check if position is within bounds
    bool isWithinBounds(int width, int height) const {
        return x >= 0 && x < width && y >= 0 && y < height;
    }
};

#endif // POSITION_H
