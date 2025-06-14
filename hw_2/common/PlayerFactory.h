/**
 * @file PlayerFactory.h
 * @brief Defines the factory interface for creating player instances.
 * @author Advanced Programming Team
 */
#pragma once

#include <memory>
#include "Player.h"

// Use std namespace explicitly for clarity
using std::unique_ptr;

/**
 * @class PlayerFactory
 * @brief Abstract factory for creating Player instances
 * 
 * This factory pattern implementation allows for flexible instantiation
 * of different player types without exposing the creation logic.
 */
class PlayerFactory {
public:
    /**
     * @brief Virtual destructor for proper inheritance cleanup
     */
    virtual ~PlayerFactory() = default;

    /**
     * @brief Creates a new Player instance
     * 
     * @param playerIndex Player's unique identifier
     * @param startX Starting X position
     * @param startY Starting Y position
     * @param maxSteps Maximum movement steps allowed
     * @param shellCount Number of shells available
     * @return std::unique_ptr<Player> A smart pointer to the created Player
     */
    virtual std::unique_ptr<Player> create(
        int playerIndex, 
        size_t startX, 
        size_t startY,
        size_t maxSteps, 
        size_t shellCount) const = 0;
};
