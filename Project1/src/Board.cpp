#include "Board.h"
#include <limits>
#include <algorithm>
/**
 * Convert a character from the file into a CellType enum.
 * You can expand or tweak this logic as needed.
 */
static CellType charToCellType(char c) {
    switch(c) {
        case '#': return CellType::WALL;
        case '@': return CellType::MINE;
        case '1': return CellType::TANK1;
        case '2': return CellType::TANK2;
        case ' ': return CellType::EMPTY;
        default:  return CellType::UNKNOWN; // or EMPTY if you prefer
    }
}

/**
 * Convert a CellType back to a char if needed.
 * Could be used for display or debugging.
 */
static char cellTypeToChar(CellType t) {
    switch(t) {
        case CellType::WALL:   return '#';
        case CellType::MINE:   return '@';
        case CellType::TANK1:  return '1';
        case CellType::TANK2:  return '2';
        case CellType::EMPTY:  return ' ';
        case CellType::UNKNOWN:return '?';
    }
    return '?';
}

bool Board::loadFromFile(const std::string &filename, Tank* tank1, Tank* tank2) {
    std::ifstream fin(filename);
    if (!fin.is_open()) {
        std::cerr << "Error: Could not open file '" << filename << "'\n";
        return false;
    }

    // 1) Read width, height
    if (!(fin >> width >> height)) {
        std::cerr << "Error: Failed to read board dimensions.\n";
        return false;
    }
    if (width <= 0 || height <= 0) {
        std::cerr << "Error: Invalid board dimensions.\n";
        return false;
    }

    // If there is a third token specifying wrap-around, you could read it too

    // Ignore the rest of the line
    fin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // Resize grid and wallInfo
    grid.resize(height, std::vector<CellType>(width, CellType::EMPTY));
    wallInfo.resize(height, std::vector<WallDamage>(width));

    for (int row = 0; row < height; ++row) {
        std::string line;
        if (!std::getline(fin, line)) {
            break;
        }

        for (int col = 0; col < width; ++col) {
            char c = (col < (int)line.size()) ? line[col] : ' ';
            CellType type = charToCellType(c);
            grid[row][col] = type;
            if (type == CellType::TANK1 && tank1) {
                tank1->setPosition(col, row);
            }
            else if (type == CellType::TANK2 && tank2) {
                tank2->setPosition(col, row);
            }
            // If it's a wall, set wallInfo
            if (type == CellType::WALL) {
                wallInfo[row][col].isWall = true;
                wallInfo[row][col].hitsTaken = 0;
            }
        }
    }

    fin.close();
    return true;
}

CellType Board::getCellType(int x, int y) const {
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
    if (wallInfo[y][x].hitsTaken == 1) {
        grid[y][x] = CellType::WEAK_WALL;
        std::cout << "Weak wall at (" << x << ", " << y << ")" << std::endl;
    }
    
    // If hits >= 2, destroy the wall
    if (wallInfo[y][x].hitsTaken >= 2) {
        // turn cell into empty
        grid[y][x] = CellType::EMPTY;
        wallInfo[y][x].isWall = false;
        wallInfo[y][x].hitsTaken = 0; // reset
        std::cout << "Wall destroyed at (" << x << ", " << y << ")" << std::endl;
    }
}

