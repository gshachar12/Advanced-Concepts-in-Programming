// Globals.h
#pragma once

#ifndef GLOBALS_H
#define GLOBALS_H

#include <memory>
class Board; // forward declaration

class Global {
public:
    static std::unique_ptr<Board> board;  // Declaration of board
    static int width;
    static int height;
};

#endif // GLOBALS_H
