/**
 * @file TankAlgorithmFactory.h
 * @brief Defines the factory interface for creating tank AI algorithms.
 * @author Advanced Programming Team
 */
#pragma once

#include <memory>
#include "TankAlgorithm.h"

// Use std namespace explicitly for clarity
using std::unique_ptr;

/**
 * @class TankAlgorithmFactory
 * @brief Abstract factory for creating tank AI algorithm instances
 *
 * This factory pattern implementation allows for flexible instantiation
 * of different tank AI algorithms without exposing the creation logic.
 */
class TankAlgorithmFactory {
public:
    /**
     * @brief Virtual destructor for proper inheritance cleanup
     */
    virtual ~TankAlgorithmFactory() = default;

    /**
     * @brief Creates a new TankAlgorithm instance
     *
     * @param playerID Player's unique identifier
     * @param tankID Tank's unique identifier within the player's fleet
     * @return std::unique_ptr<TankAlgorithm> A smart pointer to the created tank algorithm
     */
    virtual std::unique_ptr<TankAlgorithm> create(
        int playerID, int tankID) const = 0;
};
