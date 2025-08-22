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

// Convert (dx, dy) offset  Direction enum to
Direction Directions::OffsetToDirection(Position d) {
        if (d.getX() == 0 && d.getY()==-1)  return Direction::U;
        if (d.getX() == 1 && d.getY()==-1) return Direction::UR;
        if (d.getX() == 1 && d.getY()==0) return Direction::R;
        if (d.getX() == 1 && d.getY()==1) return Direction::DR;
        if (d.getX() == 0 && d.getY()==1) return Direction::D;
        if (d.getX() == -1 && d.getY()==1) return Direction::DL;
        if (d.getX() == -1 && d.getY()==0) return Direction::L;
        if (d.getX() == -1 && d.getY()==-1) return Direction::UL;
        else            return Direction::D; // Default case
    
}


Direction Directions::angleToClosestDirection(double angle) {
     // Convert angle to 0..360 degrees
     double degrees = fmod((angle * 180.0 / M_PI) + 360.0, 360.0);
     // 8 directions split into 45° increments
     int dirIndex = static_cast<int>(std::round(degrees / 45.0)) % 8;
     // Map index to Direction
     switch (dirIndex) {
         case 0: return Direction::R;
         case 1: return Direction::DR;
         case 2: return Direction::D;
         case 3: return Direction::DL;
         case 4: return Direction::L;
         case 5: return Direction::UL;
         case 6: return Direction::U;
         default: return Direction::UR;
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
