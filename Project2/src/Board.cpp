#include <limits>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <regex>
#include "Board.h"

// Constants for wall damage
const int WALL_DAMAGE_WEAK = 1;
const int WALL_DAMAGE_DESTROYED = 2;

/**
 * Convert a CellType back to a char if needed.
 * Could be used for display or debugging.
 */
static char cellTypeToChar(CellType t) {
    switch(t) {
        case CellType::WALL:   return '#';
        case CellType::MINE:   return '@';
        case CellType::TANK1:  return '1';
        case CellType::BOOM: return '&';
        case CellType::SHELL: return '*';  // Changed from '>' to '*' for Project2 satellite view
        case CellType::WEAK_WALL: return ';';
        case CellType::TANK2:  return '2';
        case CellType::EMPTY:  return ' ';
        case CellType::UNKNOWN:return '?';
    }
    return '?';
}

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
        default:  return CellType::EMPTY; // For Project2, any other character is EMPTY
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
    }
    
    // If hits >= 2, destroy the wall
    if (wallInfo[y][x].hitsTaken >= WALL_DAMAGE_DESTROYED) {
        // turn cell into empty
        grid[y][x] = CellType::EMPTY;
        wallInfo[y][x].isWall = false;
        wallInfo[y][x].hitsTaken = 0; // reset
    }
}

// New method for Project2: parse the updated map format
bool Board::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return false;
    }
    
    // Reading header information
    std::string line;
    
    // Line 1: Description (ignore)
    if (!std::getline(file, line)) {
        std::cerr << "Error: File is empty" << std::endl;
        return false;
    }
    
    // Line 2: MaxSteps
    if (!std::getline(file, line)) {
        std::cerr << "Error: Missing MaxSteps information" << std::endl;
        return false;
    }
    
    std::regex maxStepsPattern(R"(MaxSteps\s*=\s*(\d+))");
    std::smatch match;
    if (std::regex_search(line, match, maxStepsPattern) && match.size() > 1) {
        maxSteps = std::stoul(match[1].str());
    } else {
        std::cerr << "Error: Invalid MaxSteps format" << std::endl;
        return false;
    }
    
    // Line 3: NumShells
    if (!std::getline(file, line)) {
        std::cerr << "Error: Missing NumShells information" << std::endl;
        return false;
    }
    
    std::regex numShellsPattern(R"(NumShells\s*=\s*(\d+))");
    if (std::regex_search(line, match, numShellsPattern) && match.size() > 1) {
        numShellsPerTank = std::stoul(match[1].str());
    } else {
        std::cerr << "Error: Invalid NumShells format" << std::endl;
        return false;
    }
    
    // Line 4: Number of Rows
    if (!std::getline(file, line)) {
        std::cerr << "Error: Missing Rows information" << std::endl;
        return false;
    }
    
    std::regex rowsPattern(R"(Rows\s*=\s*(\d+))");
    if (std::regex_search(line, match, rowsPattern) && match.size() > 1) {
        height = std::stoi(match[1].str());
    } else {
        std::cerr << "Error: Invalid Rows format" << std::endl;
        return false;
    }
    
    // Line 5: Number of Cols
    if (!std::getline(file, line)) {
        std::cerr << "Error: Missing Cols information" << std::endl;
        return false;
    }
    
    std::regex colsPattern(R"(Cols\s*=\s*(\d+))");
    if (std::regex_search(line, match, colsPattern) && match.size() > 1) {
        width = std::stoi(match[1].str());
    } else {
        std::cerr << "Error: Invalid Cols format" << std::endl;
        return false;
    }
    
    // Initialize the grid with EMPTY cells
    grid.clear();
    grid.resize(height, std::vector<CellType>(width, CellType::EMPTY));
    
    // Initialize the wall info
    wallInfo.clear();
    wallInfo.resize(height, std::vector<WallDamage>(width));
    
    // Read the map data
    int row = 0;
    while (std::getline(file, line) && row < height) {
        for (int col = 0; col < width && col < static_cast<int>(line.length()); ++col) {
            char c = line[col];
            CellType cellType = charToCellType(c);
            grid[row][col] = cellType;
            
            // Update wall info if needed
            if (cellType == CellType::WALL) {
                wallInfo[row][col].isWall = true;
                wallInfo[row][col].hitsTaken = 0;
            }
        }
        row++;
    }
    
    // Fill in any missing rows with empty cells
    for (; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
            grid[row][col] = CellType::EMPTY;
        }
    }
    
    return true;
}