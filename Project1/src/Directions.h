/*
*
* Auxilary operation related to directions
*
*/


#pragma once
#include <iostream>
#include <array>
#include <string>
#include <vector>
#include "Position.h"

// Globally accessible direction strings for consistency
const std::vector<std::string> validDirections = {"U", "UR", "R", "DR", "D", "DL", "L", "UL"};

enum class Direction {
    U,    // Up
    UR,   // Up-Right
    R,    // Right
    DR,   // Down-Right
    D,    // Down
    DL,   // Down-Left
    L,    // Left
    UL,   // Up-Left
    NUM_DIRECTIONS  // Number of directions (for iteration)
};

class Directions {
private:
    // Cache the directions array to avoid recreating it each time
    static const std::array<Direction, static_cast<int>(Direction::NUM_DIRECTIONS)> all_directions;

public:
    // Function to convert a string to a Direction enum
    static Direction stringToDirection(const std::string &str);
    static Direction OffsetToDirection(Position d);
    static std::pair<int, int> directionToOffset(Direction dir);
    static Direction 
    
    
    
    angleToClosestDirection(double angle);
    // Function to print all directions
    static void printAllDirections();

    // Function to get a string representation of a direction
    static std::string directionToString(Direction dir);

    // Function to get all directions - now returns a const reference to avoid copying
    static const std::array<Direction, static_cast<int>(Direction::NUM_DIRECTIONS)>& getAllDirections();


};
