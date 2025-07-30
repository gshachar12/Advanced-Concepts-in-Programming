#ifndef MY_GAME_MANAGER_H
#define MY_GAME_MANAGER_H

#include "../common/AbstractGameManager.h"
#include "GameManager.h"
#include <memory>

namespace GameManager_123456789_987654321 {

class MyGameManager : public AbstractGameManager {
public:
    MyGameManager(bool verbose = false) : verbose_(verbose) {}

    GameResult run(
        size_t map_width, size_t map_height,
        SatelliteView& map, // <= assume it is a snapshot, NOT updated
        size_t max_steps, size_t num_shells,
        Player& player1, Player& player2,
        TankAlgorithmFactory player1_tank_algo_factory,
        TankAlgorithmFactory player2_tank_algo_factory) override;

private:
    bool verbose_;
};

} // namespace GameManager_123456789_987654321

#endif // MY_GAME_MANAGER_H
