#pragma once
#include <iostream>
#include <array>
#include <string>
#include <vector>
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
public:
    // Function to convert a string to a Direction enum
    static Direction stringToDirection(const std::string &str);
    static std::pair<int, int> directionToOffset(Direction dir);
    // Function to print all directions
    static void printAllDirections();

    // Function to get a string representation of a direction
    static std::string directionToString(Direction dir);

    // Function to iterate over all directions and print them
    static std::array<Direction, static_cast<int>(Direction::NUM_DIRECTIONS)> getAllDirections();
};
