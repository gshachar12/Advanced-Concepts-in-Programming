#ifndef DIRECTION_H
#define DIRECTION_H

#include "Position.h"
#include <array>

/**
 * Represents the 8 possible directions for tank movement and rotation
 */
enum class Direction {
    UP = 0,      // North
    UP_RIGHT,    // Northeast  
    RIGHT,       // East
    DOWN_RIGHT,  // Southeast
    DOWN,        // South
    DOWN_LEFT,   // Southwest
    LEFT,        // West
    UP_LEFT      // Northwest
};

/**
 * Utility class for working with directions
 */
class DirectionUtils {
public:
    /**
     * Convert direction to position offset
     */
    static Position directionToOffset(Direction dir) {
        static const std::array<Position, 8> offsets = {{
            {0, -1},  // UP
            {1, -1},  // UP_RIGHT
            {1, 0},   // RIGHT
            {1, 1},   // DOWN_RIGHT
            {0, 1},   // DOWN
            {-1, 1},  // DOWN_LEFT
            {-1, 0},  // LEFT
            {-1, -1}  // UP_LEFT
        }};
        
        return offsets[static_cast<int>(dir)];
    }
    
    /**
     * Rotate direction by 45 degrees clockwise
     */
    static Direction rotateClockwise45(Direction dir) {
        return static_cast<Direction>((static_cast<int>(dir) + 1) % 8);
    }
    
    /**
     * Rotate direction by 45 degrees counter-clockwise
     */
    static Direction rotateCounterClockwise45(Direction dir) {
        return static_cast<Direction>((static_cast<int>(dir) + 7) % 8);
    }
    
    /**
     * Rotate direction by 90 degrees clockwise
     */
    static Direction rotateClockwise90(Direction dir) {
        return static_cast<Direction>((static_cast<int>(dir) + 2) % 8);
    }
    
    /**
     * Rotate direction by 90 degrees counter-clockwise  
     */
    static Direction rotateCounterClockwise90(Direction dir) {
        return static_cast<Direction>((static_cast<int>(dir) + 6) % 8);
    }
    
    /**
     * Get opposite direction
     */
    static Direction getOpposite(Direction dir) {
        return static_cast<Direction>((static_cast<int>(dir) + 4) % 8);
    }
    
    /**
     * Convert direction to string for debugging
     */
    static const char* toString(Direction dir) {
        static const char* names[] = {
            "UP", "UP_RIGHT", "RIGHT", "DOWN_RIGHT",
            "DOWN", "DOWN_LEFT", "LEFT", "UP_LEFT"
        };
        return names[static_cast<int>(dir)];
    }
};

#endif // DIRECTION_H
