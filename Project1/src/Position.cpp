// Position.cpp
#include "Position.h"  // Include the header file where the class is declared

// Constructor to initialize a position with x and y coordinates
Position::Position(int x, int y) : x(x), y(y) {}

// Overload the equality operator to compare two positions
bool Position::operator==(const Position &other) const {
    return x == other.x && y == other.y;
}

// Overload the inequality operator to compare two positions
bool Position::operator!=(const Position &other) const {
    return !(*this == other);
}

// Add an offset to the current position
Position Position::operator+(const Position &offset) const {
    return Position(x + offset.x, y + offset.y);
}

// Subtract an offset from the current position
Position Position::operator-(const Position &offset) const {
    return Position(x - offset.x, y - offset.y);
}

// Check if the position is within grid bounds
bool Position::isInBounds(int width, int height) const {
    return x >= 0 && x < width && y >= 0 && y < height;
}

// Output position for debugging
void Position::print() const {
    std::cout << "(" << x << ", " << y << ")\n";
}
