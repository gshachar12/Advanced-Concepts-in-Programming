#pragma once


/**
 * 
 * - WALL:     '#'
 * - MINE:     '@'
 * - TANK1:    '1'
 * - TANK2:    '2'
 * - EMPTY:    ' '
 * - UNKNOWN:  
 */


enum class CellType {
    EMPTY,
    WALL,
    MINE,
    TANK1,
    TANK2,
    SHELL, 
    BOOM,
    WEAK_WALL, 
    UNKNOWN
};

std::string cellTypeToString(CellType type) {
    switch (type) {
        case CellType::EMPTY: return "EMPTY";
        case CellType::WALL: return "WALL";
        case CellType::MINE: return "MINE";
        case CellType::TANK1: return "TANK1";
        case CellType::TANK2: return "TANK2";
        case CellType::SHELL: return "SHELL";
        case CellType::BOOM: return "BOOM";
        case CellType::WEAK_WALL: return "WEAK_WALL";
        case CellType::UNKNOWN: return "UNKNOWN";
        default: return "INVALID";
    }
}