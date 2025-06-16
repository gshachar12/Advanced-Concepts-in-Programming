#include <iostream>
#include <string>
#include <cstring>
#include "GameManager.h"
#include "MyPlayer.h"

int main(int argc, char** argv) {
    std::cout << "Starting Tank Game..." << std::endl;
    
    // Check for visualization flag
    bool visualMode = false;
    std::string boardFile;
    
    // Parse command line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--visual") == 0) {
            visualMode = true;
        } else {
            boardFile = argv[i];
        }
    }
    
    if (boardFile.empty()) {
        std::cerr << "Usage: tanks_game <game_board_input_file> [--visual]" << std::endl;
        return 1;
    }

    std::cout << "Using map file: " << boardFile << std::endl;
    std::cout << "Visual mode: " << (visualMode ? "enabled" : "disabled") << std::endl;

    try {
        std::cout << "Creating game manager..." << std::endl;
        // Create game manager with the required factory objects and visual mode
        GameManager game(MyPlayerFactory(), MyTankAlgorithmFactory(), visualMode);
        
        std::cout << "Reading board file..." << std::endl;
        // Read the board from the specified file
        if (!game.readBoard(boardFile)) {
            std::cerr << "Failed to read board from file: " << boardFile << std::endl;
            return 2;
        }
        
        std::cout << "Running game..." << std::endl;
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