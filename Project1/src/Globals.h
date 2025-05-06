// Globals.h
#pragma once

#ifndef GLOBALS_H
#define GLOBALS_H

#include <memory>
class Board; // forward declaration

// Global constants and variables for the game
// This class provides centralized access to shared game state
class Global {
public:
    static std::unique_ptr<Board> board;  // Declaration of board
    static int width;                     // Board width
    static int height;                    // Board height
    
    // Reset all global values to default state
    static void reset() {
        width = 0;
        height = 0;
        // board will be reset elsewhere when needed
    }
};

#endif // GLOBALS_H

