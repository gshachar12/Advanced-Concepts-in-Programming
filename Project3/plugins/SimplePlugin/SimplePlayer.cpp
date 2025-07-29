#include "SimplePlayer.h"
#include "../../common/PlayerRegistration.h"

namespace SimplePlugin {

SimplePlayer::SimplePlayer(int player_index, size_t x, size_t y, size_t max_steps, size_t num_shells)
    : Player(player_index, x, y, max_steps, num_shells), 
      player_index_(player_index), start_x_(x), start_y_(y), max_steps_(max_steps), num_shells_(num_shells) {
}

void SimplePlayer::updateTankWithBattleInfo(TankAlgorithm& tank, SatelliteView& satellite_view) {
    // Simple player implementation - just pass the satellite view
    // The tank algorithm will handle the actual decision making
    (void)satellite_view; // Suppress unused parameter warning
    (void)tank; // Suppress unused parameter warning
}

} // namespace SimplePlugin

// Register the player for dynamic loading
using namespace SimplePlugin;
REGISTER_PLAYER(SimplePlayer);
