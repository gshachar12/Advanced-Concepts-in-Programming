#ifndef SIMPLE_PLUGIN_PLAYER_H
#define SIMPLE_PLUGIN_PLAYER_H

#include "../../common/Player.h"
#include "../../common/TankAlgorithm.h"
#include "../../common/SatelliteView.h"

/**
 * Simple player plugin for dynamic loading
 */
namespace SimplePlugin {

class SimplePlayer : public Player {
private:
    int player_index_;
    size_t start_x_, start_y_;
    size_t max_steps_, num_shells_;
    
public:
    SimplePlayer(int player_index, size_t x, size_t y, size_t max_steps, size_t num_shells);
    
    void updateTankWithBattleInfo(TankAlgorithm& tank, SatelliteView& satellite_view) override;
};

} // namespace SimplePlugin

#endif // SIMPLE_PLUGIN_PLAYER_H
