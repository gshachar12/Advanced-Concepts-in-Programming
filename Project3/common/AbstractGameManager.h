#ifndef ABSTRACT_GAME_MANAGER_H
#define ABSTRACT_GAME_MANAGER_H

#include "GameResult.h"
#include "SatelliteView.h"
#include "Player.h"
#include "TankAlgorithm.h"
#include <cstddef>

/**
 * Abstract interface for game managers
 * Allows different implementations of the game execution logic
 */
class AbstractGameManager {
public:
    virtual ~AbstractGameManager() {}
    
    /**
     * Run a complete game between two players
     * @param map_width Width of the game map
     * @param map_height Height of the game map
     * @param map Read-only view of the initial map state
     * @param max_steps Maximum number of steps before timeout
     * @param num_shells Number of shells each tank starts with
     * @param player1 First player
     * @param player2 Second player
     * @param player1_tank_algo_factory Factory for player 1's tank algorithms
     * @param player2_tank_algo_factory Factory for player 2's tank algorithms
     * @return Game result with winner and statistics
     */
    virtual GameResult run(
        size_t map_width, 
        size_t map_height,
        SatelliteView& map,
        size_t max_steps, 
        size_t num_shells,
        Player& player1, 
        Player& player2,
        TankAlgorithmFactory player1_tank_algo_factory,
        TankAlgorithmFactory player2_tank_algo_factory) = 0;
};

#endif // ABSTRACT_GAME_MANAGER_H
