/**
 * @file Player.h
 * @brief Defines the player controller interface for the tank battle game.
 * @author Advanced Programming Team
 */
#pragma once

#include "SatelliteView.h"
#include "TankAlgorithm.h"

/**
 * @class Player
 * @brief Abstract interface for player controllers in the game
 * 
 * Represents a player that controls one or more tanks in the battle.
 * Each player is responsible for updating their tanks with battle information.
 */
class Player {
public:
    /**
     * @brief Constructor with initialization parameters
     * 
     * @param playerIndex Player's unique identifier
     * @param startX Starting X position
     * @param startY Starting Y position
     * @param maxSteps Maximum movement steps allowed
     * @param shellCount Number of shells available
     */
    Player(int player_index, size_t x, size_t y, 
           size_t max_steps, size_t num_shells) {
        // Parameters intentionally unused in base class
        // Derived classes will use these parameters
        (void)player_index;
        (void)x;
        (void)y;
        (void)max_steps;
        (void)num_shells;
    }

    /**
     * @brief Virtual destructor for proper inheritance cleanup
     */
    virtual ~Player() {}

    /**
     * @brief Updates tank with the current battlefield information
     * 
     * @param tank The tank algorithm to update
     * @param satelliteView Current view of the battlefield
     */
    virtual void updateTankWithBattleInfo(
        TankAlgorithm &tank, SatelliteView &satellite_view) = 0;
};
