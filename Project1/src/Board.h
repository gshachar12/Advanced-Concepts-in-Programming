#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <string>
#include <iostream>
#include <limits>
#include <fstream>
#include "CellType.h"

// Constants for wall damage
extern const int WALL_DAMAGE_WEAK;
extern const int WALL_DAMAGE_DESTROYED;

/**
 * Tracks how many hits a wall has taken.
 * After 2 hits, we consider it destroyed and turn it into EMPTY.
 */
struct WallDamage
{
    bool isWall = false;
    int hitsTaken = 0; // after 2 hits => destroyed
};

class Board
{
private:
    int width;
    int height;
    bool wrapAround; // if true, we do (x + width) % width for indexing

public:
    Board() : width(0), height(0), wrapAround(false) {}
    std::vector<std::vector<CellType>> grid;
    std::vector<std::vector<WallDamage>> wallInfo;

    // Basic getters
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    void setWidth(int w) { width = w; }
    void setHeight(int h) { height = h; }
    void setWrapAround(bool enable) { wrapAround = enable; }
    bool isWrapAround() const { return wrapAround; }

    
    // Check if (x, y) is within normal bounds (no wrap-around here).
    bool inBounds(int x, int y) const
    {
        return (x >= 0 && x < width && y >= 0 && y < height);
    }

    // Get the CellType at (x, y),
    static CellType charToCellType(char c);
    // applying wrap-around or bounding checks as needed.
    
    CellType getCellType(int x, int y) const;
    // Set the CellType at (x, y),
    // applying wrap-around or bounding checks as needed.
    void setCellType(int x, int y, CellType type);

    // If a shell hits a wall at (x, y), increase hits.
    // If hits >= 2, remove wall (turn into EMPTY).
    void weakenWall(int x, int y);
};

#endif // BOARD_H
