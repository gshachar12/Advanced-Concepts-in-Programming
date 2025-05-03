#pragma once
#include <iostream>
#include <cmath>

class Position {
public:
    int x, y;

    // Constructor to initialize a position with x and y coordinates
    Position(int x = 0, int y = 0);

    // Overload the equality operator to compare two positions
    bool operator==(const Position &other) const;

    // Overload the inequality operator to compare two positions
    bool operator!=(const Position &other) const;

    // Add an offset to the current position
    Position operator+(const Position &offset) const;

    // Subtract an offset from the current position
    Position operator-(const Position &offset) const;

    // Check if the position is within grid bounds
    bool isInBounds(int width, int height) const;

    // Output position for debugging
    void print() const;
    std::string toString() const ;
};
