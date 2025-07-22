#include "Position.h"
#include <cmath>

// Calculates the Manhattan distance between this position and another
int Position::manhattanDistance(const Position& other) const {
    return std::abs(x - other.x) + std::abs(y - other.y);
}

// Calculates the Euclidean distance between this position and another
double Position::euclideanDistance(const Position& other) const {
    int dx = x - other.x;
    int dy = y - other.y;
    return std::sqrt(dx*dx + dy*dy);
}

// Checks if this position is in a range from another position
bool Position::isInRange(const Position& center, int range) const {
    return manhattanDistance(center) <= range;
}

// Creates a new position offset by dx and dy
Position Position::offset(int dx, int dy) const {
    return Position(x + dx, y + dy);
}

// Returns true if the position is within given bounds
bool Position::isWithinBounds(int maxX, int maxY) const {
    return x >= 0 && x < maxX && y >= 0 && y < maxY;
}

// Clamps the position to stay within bounds
void Position::clamp(int minVal, int maxVal) {

    
    if (x!=0&&x < minVal) x = minVal;
    if (x!=0&&x > maxVal) x = maxVal;
    if (y!=0&&y < minVal) y = minVal;
    if (y!=0&&y > maxVal) y = maxVal;
}

// Wraps the position around the bounds (like in Pac-Man)
void Position::wrap(int maxX, int maxY) {
    x = (x % maxX + maxX) % maxX;  // Ensure non-negative result with double modulo
    y = (y % maxY + maxY) % maxY;
}