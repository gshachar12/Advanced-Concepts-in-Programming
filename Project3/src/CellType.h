#ifndef CELL_TYPE_H
#define CELL_TYPE_H

/**
 * Represents different types of cells on the game board
 */
enum class CellType {
    EMPTY = 0,      // Empty space
    WALL,           // Regular wall (takes 2 hits to destroy)
    WEAK_WALL,      // Weak wall (takes 1 hit to destroy)
    MINE,           // Mine that explodes on contact
    TANK1,          // Player 1's tank
    TANK2,          // Player 2's tank
    SHELL,          // Active shell
    EXPLOSION       // Explosion marker
};

/**
 * Utility class for working with cell types
 */
class CellTypeUtils {
public:
    /**
     * Convert cell type to character representation
     */
    static char toChar(CellType type) {
        switch (type) {
            case CellType::EMPTY:     return ' ';
            case CellType::WALL:      return '#';
            case CellType::WEAK_WALL: return '=';
            case CellType::MINE:      return '@';
            case CellType::TANK1:     return '1';
            case CellType::TANK2:     return '2';
            case CellType::SHELL:     return '*';
            case CellType::EXPLOSION: return 'X';
            default:                  return '?';
        }
    }
    
    /**
     * Convert character to cell type
     */
    static CellType fromChar(char c) {
        switch (c) {
            case ' ': return CellType::EMPTY;
            case '#': return CellType::WALL;
            case '=': return CellType::WEAK_WALL;
            case '@': return CellType::MINE;
            case '1': return CellType::TANK1;
            case '2': return CellType::TANK2;
            case '*': return CellType::SHELL;
            case 'X': return CellType::EXPLOSION;
            default:  return CellType::EMPTY;
        }
    }
    
    /**
     * Check if cell type blocks movement
     */
    static bool blocksMovement(CellType type) {
        return type == CellType::WALL || 
               type == CellType::WEAK_WALL ||
               type == CellType::TANK1 ||
               type == CellType::TANK2;
    }
    
    /**
     * Check if cell type is destructible
     */
    static bool isDestructible(CellType type) {
        return type == CellType::WALL || 
               type == CellType::WEAK_WALL ||
               type == CellType::TANK1 ||
               type == CellType::TANK2;
    }
};

#endif // CELL_TYPE_H
