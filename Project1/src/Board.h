#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <string>
#include <iostream>
#include <limits>
#include <fstream>

/**
 * An enum for what might occupy each cell:
 * - WALL:     '#'
 * - MINE:     '@'
 * - TANK1:    '1'
 * - TANK2:    '2'
 * - EMPTY:    ' '
 * - UNKNOWN:  anything else (if you want to handle errors)
 */
enum class CellType {
    EMPTY,
    WALL,
    MINE,
    TANK1,
    TANK2,
    UNKNOWN
};

/**
 * Tracks how many hits a wall has taken.
 * After 2 hits, we consider it destroyed and turn it into EMPTY.
 */
struct WallDamage {
    bool isWall = false;
    int hitsTaken = 0;  // after 2 hits => destroyed
};

class Board {
private:
    int width;
    int height;
    bool wrapAround;  // if true, we do (x + width) % width for indexing

    // Our main grid storing CellType
    std::vector<std::vector<CellType>> grid;

    // Parallel 2D array for tracking wall damage
    std::vector<std::vector<WallDamage>> wallInfo;

public:
    Board() : width(0), height(0), wrapAround(false) {}

    // Load from file (similar to your earlier code),
    // but now we parse chars into CellType.
    bool loadFromFile(const std::string &filename);

    // Basic getters
    int getWidth() const { return width; }
    int getHeight() const { return height; }

    // Decide if you want wrap-around indexing
    void setWrapAround(bool enable) { wrapAround = enable; }
    bool isWrapAround() const { return wrapAround; }

    // Check if (x, y) is within normal bounds (no wrap-around here).
    bool inBounds(int x, int y) const {
        return (x >= 0 && x < width && y >= 0 && y < height);
    }

    // Get the CellType at (x, y),
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
