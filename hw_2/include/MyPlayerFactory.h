#pragma once
/**
 * @file MyPlayerFactory.h
 * @brief Defines factory class for creating Player instances.
 */

#include <memory>
#include "PlayerFactory.h"

/**
 * @class MyPlayerFactory
 * @brief Factory class for creating custom Player instances.
 * 
 * This class implements the Factory design pattern to create Player objects
 * with specific configurations for the tank battle game.
 */
class MyPlayerFactory final : public PlayerFactory {
public:
    /**
     * @brief Creates a new Player instance.
     * 
     * @param player_index Unique identifier for the player
     * @param x Initial x-coordinate for the player's tank
     * @param y Initial y-coordinate for the player's tank
     * @param max_steps Maximum number of steps the tank can take
     * @param num_shells Maximum number of shells the tank can fire
     * @return std::unique_ptr<Player> Smart pointer to the created Player
     */
    std::unique_ptr<Player> create(int player_index, size_t x, size_t y,
                              size_t max_steps, size_t num_shells) const override;
};
