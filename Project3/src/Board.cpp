#include "Board.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <limits>

Board::Board(size_t width, size_t height) 
    : width_(width), height_(height) {
    grid_.resize(height_, std::vector<CellType>(width_, CellType::EMPTY));
    wall_damage_.resize(height_, std::vector<int>(width_, 0));
}

bool Board::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file " << filename << std::endl;
        return false;
    }
    
    // Read board dimensions
    size_t new_width, new_height;
    if (!(file >> new_width >> new_height)) {
        std::cerr << "Error: Cannot read board dimensions" << std::endl;
        return false;
    }
    
    // Resize board
    width_ = new_width;
    height_ = new_height;
    grid_.resize(height_, std::vector<CellType>(width_, CellType::EMPTY));
    wall_damage_.resize(height_, std::vector<int>(width_, 0));
    
    // Skip rest of first line
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    // Read board layout
    for (size_t y = 0; y < height_; ++y) {
        std::string line;
        if (!std::getline(file, line)) {
            std::cerr << "Error: Incomplete board data at line " << y << std::endl;
            return false;
        }
        
        for (size_t x = 0; x < width_; ++x) {
            char cell_char = (x < line.size()) ? line[x] : ' ';
            grid_[y][x] = CellTypeUtils::fromChar(cell_char);
        }
    }
    
    file.close();
    return true;
}

char Board::getObjectAt(size_t x, size_t y) const {
    if (!isValidPosition(x, y)) {
        return '#'; // Return wall for out-of-bounds
    }
    return CellTypeUtils::toChar(grid_[y][x]);
}

CellType Board::getCellType(const Position& pos) const {
    return getCellType(pos.x, pos.y);
}

CellType Board::getCellType(size_t x, size_t y) const {
    if (!isValidPosition(x, y)) {
        return CellType::WALL; // Treat out-of-bounds as wall
    }
    return grid_[y][x];
}

void Board::setCellType(const Position& pos, CellType type) {
    setCellType(pos.x, pos.y, type);
}

void Board::setCellType(size_t x, size_t y, CellType type) {
    if (isValidPosition(x, y)) {
        grid_[y][x] = type;
    }
}

bool Board::isValidPosition(const Position& pos) const {
    return isValidPosition(pos.x, pos.y);
}

bool Board::isValidPosition(size_t x, size_t y) const {
    return x < width_ && y < height_;
}

bool Board::isPassable(const Position& pos) const {
    return isPassable(pos.x, pos.y);
}

bool Board::isPassable(size_t x, size_t y) const {
    if (!isValidPosition(x, y)) {
        return false;
    }
    return !CellTypeUtils::blocksMovement(grid_[y][x]);
}

bool Board::damageWall(const Position& pos) {
    return damageWall(pos.x, pos.y);
}

bool Board::damageWall(size_t x, size_t y) {
    if (!isValidPosition(x, y)) {
        return false;
    }
    
    CellType cell = grid_[y][x];
    if (cell == CellType::WALL) {
        wall_damage_[y][x]++;
        if (wall_damage_[y][x] >= 2) {
            // Wall destroyed after 2 hits
            grid_[y][x] = CellType::EMPTY;
            wall_damage_[y][x] = 0;
            return true;
        } else {
            // Wall weakened after 1 hit
            grid_[y][x] = CellType::WEAK_WALL;
            return false;
        }
    } else if (cell == CellType::WEAK_WALL) {
        // Weak wall destroyed immediately
        grid_[y][x] = CellType::EMPTY;
        wall_damage_[y][x] = 0;
        return true;
    }
    
    return false;
}

void Board::clear() {
    for (size_t y = 0; y < height_; ++y) {
        for (size_t x = 0; x < width_; ++x) {
            grid_[y][x] = CellType::EMPTY;
            wall_damage_[y][x] = 0;
        }
    }
}

void Board::print() const {
    for (size_t y = 0; y < height_; ++y) {
        for (size_t x = 0; x < width_; ++x) {
            std::cout << CellTypeUtils::toChar(grid_[y][x]);
        }
        std::cout << std::endl;
    }
}

std::vector<Position> Board::findCellsOfType(CellType type) const {
    std::vector<Position> positions;
    for (size_t y = 0; y < height_; ++y) {
        for (size_t x = 0; x < width_; ++x) {
            if (grid_[y][x] == type) {
                positions.emplace_back(x, y);
            }
        }
    }
    return positions;
}
