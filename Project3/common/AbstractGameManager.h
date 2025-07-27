#ifndef ABSTRACT_GAME_MANAGER_H
#define ABSTRACT_GAME_MANAGER_H

#include "GameResult.h"
#include "SatelliteView.h"
#include "Player.h"
#include "TankAlgorithm.h"
#include "TankAlgorithmFactory.h"
#include <string>
#include <functional>
#include <memory>

using std::string;

class AbstractGameManager {
public:
    virtual ~AbstractGameManager() {}
    virtual GameResult run(
        size_t map_width, size_t map_height,
        SatelliteView& map, // <= assume it is a snapshot, NOT updated
        size_t max_steps, size_t num_shells,
        Player& player1, Player& player2,
        TankAlgorithmFactory& player1_tank_algo_factory,
        TankAlgorithmFactory& player2_tank_algo_factory) = 0;
};

using GameManagerFactory = std::function<std::unique_ptr<AbstractGameManager>(bool verbose)>;

#endif // ABSTRACT_GAME_MANAGER_H
