// Position.cpp
#include "Position.h"  // Include the header file where the class is declared
#include <cmath>       // Include cmath for mathematical operations

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

Position Position::operator*(int scalar) const {
    return Position(x * scalar, y * scalar);
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
// not real vector normalization, but converts displacement to 1/0/-1 format 
void Position::normalize()
{
    if(x<0)
        x =-1; 
    if(x>0)
        x =1; 
    if(y<0)
       y =-1; 
    if(y>0)
       y =1; 
}

// Calculate Euclidean distance between two positions
double Position::distanceTo(const Position& other) const {
    double dx = x - other.x;
    double dy = y - other.y;
    return std::sqrt(dx * dx + dy * dy);
}

// Output position for debugging
void Position::print() const {
    std::cout << "(" << x << ", " << y << ")\n";
}

std::string Position::toString() const {
    // Create a string representing the position
    return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
}