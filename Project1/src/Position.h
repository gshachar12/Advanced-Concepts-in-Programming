#pragma once
#include <iostream>
#include <cmath>

class Position {
public:

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
    Position operator*(int scalar) const;
    void normalize();
    // Check if the position is within grid bounds
    bool isInBounds(int width, int height) const;

    // Calculate distance between two positions
    double distanceTo(const Position& other) const;
    int getX(){ return x;}
    int getY() {return y;}
    void setX(int new_x) { x = new_x;}
    void setY(int new_y) {y = new_y;}

    // Output position for debugging
    void print() const;
    std::string toString() const ;

private:
    int x, y;

};
