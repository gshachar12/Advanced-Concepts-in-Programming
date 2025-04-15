// Globals.cpp
#include "Globals.h"
#include "Board.h"


std::unique_ptr<Board> Global::board;  // Define the static unique_ptr
int Global::width = 0;  // Initialize static width
int Global::height = 0; // Initialize static height
