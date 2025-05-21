#ifndef CELLTYPE_H
#define CELLTYPE_H

enum class CellType {
    EMPTY,      // Empty space
    WALL,       // Wall
    WEAK_WALL,  // Wall that has been hit once
    MINE,       // Mine
    TANK1,      // Tank owned by player 1
    TANK2,      // Tank owned by player 2
    SHELL,      // Artillery shell
    BOOM,       // Explosion
    UNKNOWN     // Unknown cell type or out of bounds
};

#endif // CELLTYPE_H