#ifndef DIRECTIONS_H
#define DIRECTIONS_H
#include "common/ActionRequest.h"
#include <string>
#include <utility>
#include <vector>

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
    static Direction offsetToDirection(int row, int col);
    static const std::vector<Direction>& getAllDirections();  

    static ActionRequest rotationTo(Direction from, Direction to);

};


#endif // DIRECTIONS_H
