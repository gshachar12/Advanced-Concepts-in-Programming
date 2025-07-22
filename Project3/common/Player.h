#ifndef PLAYER_H
#define PLAYER_H

#include "TankAlgorithm.h"
#include "SatelliteView.h"
#include <memory>
#include <functional>
#include <cstddef>

/**
 * Abstract base class for players
 * A player manages multiple tanks and coordinates their algorithms with battle information
 */
class Player {
public:
    virtual ~Player() {}
    
    /**
     * Update a specific tank algorithm with battle information and satellite view
     * @param tank Reference to the tank algorithm to update
     * @param satellite_view Current view of the battlefield
     */
    virtual void updateTankWithBattleInfo(
        TankAlgorithm& tank, SatelliteView& satellite_view) = 0;
};

/**
 * Factory function type for creating players
 * @param player_index Index of the player
 * @param x Starting X coordinate
 * @param y Starting Y coordinate  
 * @param max_steps Maximum number of steps in the game
 * @param num_shells Number of shells each tank starts with
 * @return Unique pointer to a player instance
 */
using PlayerFactory = 
    std::function<std::unique_ptr<Player>(
        int player_index, size_t x, size_t y, size_t max_steps, size_t num_shells)>;

#endif // PLAYER_H
