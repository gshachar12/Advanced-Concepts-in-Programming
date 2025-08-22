#ifndef BFSPLAYER_H
#define BFSPLAYER_H
#include <vector>

#include "Player.h"

class BfsPlayer final : public Player {
    int player_index;
    size_t x;
    size_t y;
    size_t max_steps;
    size_t shells_count;

    std::vector<std::vector<char> > createBoardFromSatellite(const SatelliteView &satellite_view) const;

public:
    BfsPlayer(int player_index,
            size_t x, size_t y,
            size_t max_steps, size_t shells_count): Player(player_index, x, y, max_steps, shells_count),
                                                  player_index(player_index), x(x), y(y),
                                                  max_steps(max_steps),
                                                  shells_count(shells_count) {
    }

    void updateTankWithBattleInfo(TankAlgorithm &tank, SatelliteView &satellite_view) override;
};

#endif //BFSPLAYER_H
