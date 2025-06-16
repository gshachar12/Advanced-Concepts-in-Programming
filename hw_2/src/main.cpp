#include <cstring>
#include <iostream>
#include <thread>

#include "Logger.h"
#include "GameManager.h"
#include "MyPlayerFactory.h"
#include "MyTankAlgorithmFactory.h"

using namespace std::chrono_literals;

int main(const int argc, char *argv[]) {
    bool visual = false;
    std::string path;
    
    // Parse command line arguments
    if (argc == 2) {
        // Just the input file
        path = argv[1];
    } else if (argc == 3) {
        // Check if -v or -g flag is provided
        if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "-g") == 0) {
            visual = true;
            path = argv[2]; 
        } else {
            std::cerr << "Usage: " << argv[0] << " [-v] <game-file>" << std::endl;
            return EXIT_FAILURE;
        }
    } else {
        std::cerr << "Usage: " << argv[0] << " [-v] <game-file>" << std::endl;
        return EXIT_FAILURE;
    }

    Logger::getInstance().init(path);

    const auto player_factory = MyPlayerFactory();
    const auto tank_algorithm_factory = MyTankAlgorithmFactory();
    GameManager game(player_factory, tank_algorithm_factory);
    game.readBoard(path);
    game.setVisual(visual);
    game.run();

    return EXIT_SUCCESS;
}
