#include <limits>
#include <algorithm>
#include "Board.h"

// Constants for wall damage
const int WALL_DAMAGE_WEAK = 1;
const int WALL_DAMAGE_DESTROYED = 2;

/**
 * Convert a CellType back to a char if needed.
 * Could be used for display or debugging.
 */


char Board::cellTypeToChar(CellType t)
{
    switch(t) {
        case CellType::WALL:   return '#';
        case CellType::MINE:   return '@';
        case CellType::TANK1:  return '1';
        case CellType::BOOM: return '&';
        case CellType::SHELL: return '>';
        case CellType::WEAK_WALL: return ';';
        case CellType::TANK2:  return '2';
        case CellType::EMPTY:  return ' ';
        case CellType::UNKNOWN:return '?';
    }
    return '?';}

/**
 * Convert a character from the file into a CellType enum.
 * You can expand or tweak this logic as needed.
 */
CellType Board::charToCellType(char c)
{
        switch(c) {
        case '#': return CellType::WALL;
        case '@': return CellType::MINE;
        case '1': return CellType::TANK1;
        case '2': return CellType::TANK2;
        case ' ': return CellType::EMPTY;
        default:  return CellType::UNKNOWN; // or EMPTY if you prefer
    }
}

CellType Board::getCellType(int x, int y) const
{
    if (wrapAround) {
        // wrap-around indexing
        x = (x % width + width) % width;   // ensure non-negative
        y = (y % height + height) % height;
        return grid[y][x];
    } else {
        // normal bounds check
        if (!inBounds(x, y)) {
            // out of bounds => you can return a sentinel or do an error
            return CellType::UNKNOWN;
        }
        return grid[y][x];
    }
}

void Board::setCellType(int x, int y, CellType type) {
    if (wrapAround) {
        x = (x % width + width) % width;
        y = (y % height + height) % height;
    } else {
        if (!inBounds(x, y)) {
            // silently ignore or log an error
            return;
        }
    }

    grid[y][x] = type;

    // If we changed to WALL, mark wallInfo
    if (type == CellType::WALL) {
        wallInfo[y][x].isWall = true;
        wallInfo[y][x].hitsTaken = 0;
    } else {
        // If not a wall, maybe reset wallInfo
        wallInfo[y][x].isWall = false;
        wallInfo[y][x].hitsTaken = 0;
    }
}
void Board::weakenWall(int x, int y) {
    if (wrapAround) {
        x = (x % width + width) % width;
        y = (y % height + height) % height;
    } else {
        if (!inBounds(x, y)) return;
    }
    // If it's not currently a wall, do nothing
    if (!wallInfo[y][x].isWall) 
        return;
    
    wallInfo[y][x].hitsTaken++;
    // If hits = 1, turn it into a weak wall
    if (wallInfo[y][x].hitsTaken == WALL_DAMAGE_WEAK) {
        grid[y][x] = CellType::WEAK_WALL;
        std::cout << "Weak wall at (" << x << ", " << y << ")" << std::endl;
    }
    
    // If hits >= 2, destroy the wall
    if (wallInfo[y][x].hitsTaken >= WALL_DAMAGE_DESTROYED) {
        // turn cell into empty
        grid[y][x] = CellType::EMPTY;
        wallInfo[y][x].isWall = false;
        wallInfo[y][x].hitsTaken = 0; // reset
        std::cout << "Wall destroyed at (" << x << ", " << y << ")" << std::endl;
    }
}

