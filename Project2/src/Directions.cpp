#include "Directions.h"
#include <stdexcept>
#include <algorithm>
#include <cctype>

Direction Directions::stringToDirection(const std::string& dir) {
    std::string dirLower = dir;
    std::transform(dirLower.begin(), dirLower.end(), dirLower.begin(), 
                   [](unsigned char c) { return std::tolower(c); });
    
    if (dirLower == "up") return Direction::UP;
    if (dirLower == "up_right" || dirLower == "upright" || dirLower == "northeast") return Direction::UP_RIGHT;
    if (dirLower == "right") return Direction::RIGHT;
    if (dirLower == "down_right" || dirLower == "downright" || dirLower == "southeast") return Direction::DOWN_RIGHT;
    if (dirLower == "down") return Direction::DOWN;
    if (dirLower == "down_left" || dirLower == "downleft" || dirLower == "southwest") return Direction::DOWN_LEFT;
    if (dirLower == "left") return Direction::LEFT;
    if (dirLower == "up_left" || dirLower == "upleft" || dirLower == "northwest") return Direction::UP_LEFT;
    
    throw std::invalid_argument("Invalid direction string: " + dir);
}

std::string Directions::directionToString(Direction dir) {
    switch (dir) {
        case Direction::UP: return "Up";
        case Direction::UP_RIGHT: return "UpRight";
        case Direction::RIGHT: return "Right";
        case Direction::DOWN_RIGHT: return "DownRight";
        case Direction::DOWN: return "Down";
        case Direction::DOWN_LEFT: return "DownLeft";
        case Direction::LEFT: return "Left";
        case Direction::UP_LEFT: return "UpLeft";
        default: return "Unknown";
    }
}

std::pair<int, int> Directions::directionToOffset(Direction dir) {
    switch (dir) {
        case Direction::UP: return {0, -1};
        case Direction::UP_RIGHT: return {1, -1};
        case Direction::RIGHT: return {1, 0};
        case Direction::DOWN_RIGHT: return {1, 1};
        case Direction::DOWN: return {0, 1};
        case Direction::DOWN_LEFT: return {-1, 1};
        case Direction::LEFT: return {-1, 0};
        case Direction::UP_LEFT: return {-1, -1};
        default: return {0, 0};
    }
}