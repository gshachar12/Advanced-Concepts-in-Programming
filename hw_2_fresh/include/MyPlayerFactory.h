#ifndef MYPLAYERFACTORY_H
#define MYPLAYERFACTORY_H

/**
 * @file MyPlayerFactory.h
 * @brief Custom player factory implementation
 */

#include "PlayerFactory.h"

/**
 * @brief Concrete factory for creating game players
 * 
 * This factory creates Player instances with appropriate strategies
 * based on the player index and other game parameters.
 */
class MyPlayerFactory final : public PlayerFactory {
public:
    /**
     * @brief Create a new player with appropriate strategy
     * 
     * @param player_index Index of the player (1 or 2)
     * @param x Initial x coordinate
     * @param y Initial y coordinate
     * @param max_steps Maximum game steps
     * @param num_shells Maximum shells allowed
     * @return unique_ptr<Player> A new player instance
     */
    unique_ptr<Player> create(int player_index, size_t x, size_t y,
                              size_t max_steps, size_t num_shells) const override;
};

#endif //MYPLAYERFACTORY_H
