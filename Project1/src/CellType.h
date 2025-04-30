#pragma once  // Only need this, no need for #ifndef guards too
#include <string>

/**
 * 
 * - WALL:     '#'
 * - MINE:     '@'
 * - TANK1:    '1'
 * - TANK2:    '2'
 * - EMPTY:    ' '
 * - UNKNOWN:  
 * - SHELL:
 * - BOOM:
 * - WEAK_WALL:
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

// Declaration of the function only
std::string cellTypeToString(CellType type);
