#pragma once
/**
 * @file MyTankAlgorithmFactory.h
 * @brief Defines factory class for creating TankAlgorithm instances.
 */

#include <memory>
#include <list>
#include "TankAlgorithmFactory.h"

/**
 * @class MyTankAlgorithmFactory
 * @brief Factory class for creating custom TankAlgorithm instances.
 * 
 * This class implements the Factory design pattern to create algorithm objects
 * that control tank behavior in the battle game.
 */
class MyTankAlgorithmFactory final : public TankAlgorithmFactory {
public:
    /**
     * @brief Creates a new TankAlgorithm instance.
     * 
     * @param player_index Index of the player who owns the tank
     * @param tank_index Unique identifier for the tank within the player's fleet
     * @return std::unique_ptr<TankAlgorithm> Smart pointer to the created algorithm
     */
    std::unique_ptr<TankAlgorithm> create(int player_index, int tank_index) const override;
};
