#include "CellType.h"
#include <string>

namespace CellTypeUtils {

std::string toString(CellType type) {
    switch (type) {
        case CellType::EMPTY:     return "Empty";
        case CellType::WALL:      return "Wall";
        case CellType::WEAK_WALL: return "WeakWall";
        case CellType::MINE:      return "Mine";
        case CellType::TANK1:     return "Tank1";
        case CellType::TANK2:     return "Tank2";
        case CellType::SHELL:     return "Shell";
        case CellType::BOOM:      return "Explosion";
        case CellType::UNKNOWN:   return "Unknown";
        default:                  return "Invalid";
    }
}

char toChar(CellType type) {
    switch (type) {
        case CellType::EMPTY:     return ' ';
        case CellType::WALL:      return '#';
        case CellType::WEAK_WALL: return ';';
        case CellType::MINE:      return '@';
        case CellType::TANK1:     return '1';
        case CellType::TANK2:     return '2';
        case CellType::SHELL:     return '*';
        case CellType::BOOM:      return '&';
        case CellType::UNKNOWN:   return '?';
        default:                  return '?';
    }
}

bool isObstacle(CellType type) {
    return type == CellType::WALL || 
           type == CellType::WEAK_WALL || 
           type == CellType::MINE;
}

bool isTank(CellType type) {
    return type == CellType::TANK1 || type == CellType::TANK2;
}

bool isDestructible(CellType type) {
    return type == CellType::WEAK_WALL || 
           type == CellType::MINE || 
           isTank(type);
}

int getPlayerIndex(CellType type) {
    if (type == CellType::TANK1) return 1;
    if (type == CellType::TANK2) return 2;
    return 0; // Not a player tank
}

} // namespace CellTypeUtils