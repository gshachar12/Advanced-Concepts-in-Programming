// Globals.h
#ifndef GLOBALS_H
#define GLOBALS_H

#include <memory>
#include "Board.h" // Include Board class for reference

class Global {
public:
    static std::unique_ptr<Board> board;  // Declaration of board
    static int width;
    static int height;
};

#endif // GLOBALS_H
