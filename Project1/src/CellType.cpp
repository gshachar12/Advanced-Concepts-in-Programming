#include "CellType.h"

std::string cellTypeToString(CellType type) {
    switch (type) {
        case CellType::SHELL: return "Shell";
        case CellType::TANK1: return "Tank";
        case CellType::TANK2: return "Tank";
        case CellType::MINE: return "Mine";

        case CellType::WALL: return "Wall";
        case CellType::WEAK_WALL: return "Weak Wall ";
        case CellType::BOOM: return "Boom";
        default: return "Unknown";
    }
}
