#include "Directions.h"

// Initialize the static directions array
const std::array<Direction, static_cast<int>(Direction::NUM_DIRECTIONS)> Directions::all_directions = {
    Direction::U, Direction::UR, Direction::R, Direction::DR,
    Direction::D, Direction::DL, Direction::L, Direction::UL
};

// Convert string to Direction enum
Direction Directions::stringToDirection(const std::string &str) {
    if (str == "U") return Direction::U;
    else if (str == "UR") return Direction::UR;
    else if (str == "R") return Direction::R;
    else if (str == "DR") return Direction::DR;
    else if (str == "D") return Direction::D;
    else if (str == "DL") return Direction::DL;
    else if (str == "L") return Direction::L;
    else if (str == "UL") return Direction::UL;
    else return Direction::U;  // Default case (if the string is unrecognized)
}

// Convert Direction enum to string
std::string Directions::directionToString(Direction dir) {
    switch (dir) {
        case Direction::U:  return "Up";
        case Direction::UR: return "Up-Right";
        case Direction::R:  return "Right";
        case Direction::DR: return "Down-Right";
        case Direction::D:  return "Down";
        case Direction::DL: return "Down-Left";
        case Direction::L:  return "Left";
        case Direction::UL: return "Up-Left";
        default:            return "Unknown";
    }
}

// Convert Direction enum to (dx, dy) offset
std::pair<int, int> Directions::directionToOffset(Direction dir) {
    switch (dir) {
        case Direction::U:  return {0, -1};
        case Direction::UR: return {1, -1};
        case Direction::R:  return {1, 0};
        case Direction::DR: return {1, 1};
        case Direction::D:  return {0, 1};
        case Direction::DL: return {-1, 1};
        case Direction::L:  return {-1, 0};
        case Direction::UL: return {-1, -1};
        default:            return {0, 0}; // Default case
    }
}

// Function to print all directions
void Directions::printAllDirections() {
    for (Direction dir : all_directions) {
        std::cout << directionToString(dir) << std::endl;
    }
}

// Function to return all possible directions - now returns a const reference to avoid copying
const std::array<Direction, static_cast<int>(Direction::NUM_DIRECTIONS)>& Directions::getAllDirections() {
    return all_directions;
}
