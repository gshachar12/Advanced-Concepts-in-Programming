#ifndef BOARD_H
#define BOARD_H

#include "../common/SatelliteView.h"
#include "Position.h"
#include "CellType.h"
#include <vector>
#include <string>

/**
 * Represents the game board and implements SatelliteView interface
 */
class Board : public SatelliteView {
private:
    size_t width_;
    size_t height_;
    std::vector<std::vector<CellType>> grid_;
    
    // Track wall damage for multi-hit destruction
    std::vector<std::vector<int>> wall_damage_;

public:
    /**
     * Constructor creates empty board
     */
    Board(size_t width, size_t height);
    
    /**
     * Load board from file
     */
    bool loadFromFile(const std::string& filename);
    
    /**
     * Get board dimensions
     */
    size_t getWidth() const { return width_; }
    size_t getHeight() const { return height_; }
    
    /**
     * SatelliteView interface implementation
     */
    char getObjectAt(size_t x, size_t y) const override;
    
    /**
     * Get cell type at position
     */
    CellType getCellType(const Position& pos) const;
    CellType getCellType(size_t x, size_t y) const;
    
    /**
     * Set cell type at position
     */
    void setCellType(const Position& pos, CellType type);
    void setCellType(size_t x, size_t y, CellType type);
    
    /**
     * Check if position is valid (within bounds)
     */
    bool isValidPosition(const Position& pos) const;
    bool isValidPosition(size_t x, size_t y) const;
    
    /**
     * Check if position is passable (can move into)
     */
    bool isPassable(const Position& pos) const;
    bool isPassable(size_t x, size_t y) const;
    
    /**
     * Handle wall damage (for multi-hit destruction)
     */
    bool damageWall(const Position& pos);
    bool damageWall(size_t x, size_t y);
    
    /**
     * Clear board (set all cells to empty)
     */
    void clear();
    
    /**
     * Print board to console for debugging
     */
    void print() const;
    
    /**
     * Find positions of specific cell type
     */
    std::vector<Position> findCellsOfType(CellType type) const;
};

#endif // BOARD_H
