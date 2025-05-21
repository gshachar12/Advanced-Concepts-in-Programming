#ifndef DIRECTIONS_H
#define DIRECTIONS_H

#include <string>
#include <utility>

enum class Direction {
    UP,
    UP_RIGHT,
    RIGHT,
    DOWN_RIGHT,
    DOWN,
    DOWN_LEFT,
    LEFT,
    UP_LEFT
};

class Directions {
public:
    static Direction stringToDirection(const std::string& dir);
    static std::string directionToString(Direction dir);
    static std::pair<int, int> directionToOffset(Direction dir);
};

#endif // DIRECTIONS_H