#include <cstring>
#include <GameManager.h>
#include <iostream>
#include <thread>

#include "Logger.h"
#include "GameManager.h"
#include "MyPlayerFactory.h"
#include "MyTankAlgorithmFactory.h"

using namespace std::chrono_literals;

int main(const int argc, char *argv[]) {
    if (argc != 2 && (argc != 3 || strcmp(argv[1], "-g"))) {
        std::cerr << "Usage: " << argv[0] << " [-g] <game-file>" << std::endl;
        return EXIT_FAILURE;
    }

    try {
        const bool visual = argc == 3 && strcmp(argv[1], "-g") == 0;
        const std::string path = argc == 2 ? argv[1] : argv[2];

        Logger::getInstance().init(path);

        const auto player_factory = MyPlayerFactory();
        const auto tank_algorithm_factory = MyTankAlgorithmFactory();
        GameManager game(player_factory, tank_algorithm_factory);
        game.readBoard(path);
        game.setVisual(visual);
        game.run();
        
        return EXIT_SUCCESS;
    } catch (const std::exception& e) {
        std::cerr << "error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    } catch (...) {
        std::cerr << "unknown error occurred" << std::endl;
        return EXIT_FAILURE;
    }
}
