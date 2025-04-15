#include <iostream>
#include <string>
#include <memory>
#include "Board.h"
#include "Globals.h"
#include "Tank.h"
#include "Shell.h"
#include "GameManager.h"
#include "algorithms/Controller.h"
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

    // Load board
    Global::board = std::make_unique<Board>();
        // Create tanks with custom starting positions and directions
    Tank t1(0, 0, "L", CellType::TANK1);  
    Tank t2(0, 0, "UR", CellType::TANK2);  
    if (!Global::board->loadFromFile(filename, &t1, &t2)) {
        cerr << "Error: Failed to load board from " << filename << endl;
        return 1;
    }
    Global::width = Global::board->getWidth();
    Global::height = Global::board->getHeight();

    cout << "width: " << Global::width << " height: " << Global::height << endl;



    // Create minimal stub controllers
    auto ctrl1 = make_unique<Controller>();
    auto ctrl2 = make_unique<Controller>();

    // Create the GameManager using the new constructor
    GameManager gameManager(t1, t2, move(ctrl1), move(ctrl2), visual_mode);

    // Initialize game
    if (!gameManager.initializeGame(filename)) {
        cerr << "Error during game initialization." << endl;
        return 1;
    }

    // Run the game loop
    gameManager.runGameLoop();

    return 0;
}
