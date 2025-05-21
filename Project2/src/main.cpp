#include <iostream>
#include <string>
#include "GameManager.h"
#include "MyPlayer.h"

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: tanks_game <game_board_input_file>" << std::endl;
        return 1;
    }

    try {
        // Create game manager with the required factory objects
        GameManager game(MyPlayerFactory(), MyTankAlgorithmFactory());
        
        // Read the board from the specified file
        if (!game.readBoard(argv[1])) {
            std::cerr << "Failed to read board from file: " << argv[1] << std::endl;
            return 2;
        }
        
        // Run the game
        game.run();
        
        std::cout << "Game completed successfully!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 3;
    } catch (...) {
        std::cerr << "Unknown error occurred" << std::endl;
        return 4;
    }
    
    return 0;
}