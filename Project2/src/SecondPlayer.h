#ifndef MY_PLAYER_H
#define MY_PLAYER_H

#include "common/Player.h"
#include "TankBattleInfo.h"
#include <map>
#include <memory>

class SecondPlayer : public Player {
private:
    int playerIndex;
    size_t boardWidth;
    size_t boardHeight;
    size_t maxSteps;
    size_t numShells;
        
    // Map to store battle info objects for each tank
    std::map<int, std::unique_ptr<TankBattleInfo>> tankBattleInfos;

public:
    SecondPlayer(int player_index, size_t x, size_t y, size_t max_steps, size_t num_shells)
        : Player(player_index, x, y, max_steps, num_shells),
          playerIndex(player_index),
          boardWidth(x),
          boardHeight(y),
          maxSteps(max_steps),
          numShells(num_shells) {}
    
    void updateTankWithBattleInfo(TankAlgorithm& tank, SatelliteView& satellite_view) override;
};

#endif // MY_PLAYER_H