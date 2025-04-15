#include <iostream>
#include <string>
#include <memory>
#include "Board.h"
#include "Globals.h"
#include "Tank.h"
#include "Shell.h"
#include "GameManager.h"
#include "algorithms/Controller.h"  // Minimal stub should be provided
#include "GameObject.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <board_file> [--visual]" << endl;
        return 1;
    }

    string filename = argv[1];
    bool visual_mode = false;
    if (argc >= 3) {
        string modeArg = argv[2];
        if (modeArg == "--visual") {
            visual_mode = true;
            cout << "Running the game in visual mode." << endl;
        } else {
            cout << "Running the game in text mode." << endl;
        }
    } else {
        cout << "Running the game in text mode." << endl;
    }

    // Load board (we could display it initially if desired)
    Global::board = std::make_unique<Board>();  // Allocate memory for the Board
    if (!Global::board->loadFromFile(filename)) {
        cerr << "Error: Failed to load board from " << filename << endl;
        return 1;
    }
    Global::width = Global::board->getWidth();
    Global::height = Global::board->getHeight(); 
    
    cout << "width: " << Global::width << " height: " << Global::height << endl;


    // Create minimal stub controllers
    auto ctrl1 = make_unique<Controller>();
    auto ctrl2 = make_unique<Controller>();

    // Pass the visual_mode flag to GameManager's constructor.
    GameManager gameManager(move(ctrl1), move(ctrl2), visual_mode);
    if (!gameManager.initializeGame(filename)) {
        cerr << "Error during game initialization." << endl;
        return 1;
    }

    // Run the game loop.
    gameManager.runGameLoop();

    return 0;
}
