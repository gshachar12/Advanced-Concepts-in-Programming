#ifndef MYTANKALGORITHMFACTORY_H
#define MYTANKALGORITHMFACTORY_H

/**
 * @file MyTankAlgorithmFactory.h
 * @brief Custom factory for tank AI algorithms
 */

#include <list>
#include "TankAlgorithmFactory.h"

/**
 * @brief Factory that produces tank control algorithms
 * 
 * This class is responsible for creating appropriate AI strategies
 * for controlling tanks based on player and tank identifiers.
 */
class MyTankAlgorithmFactory final : public TankAlgorithmFactory {
public:
    /**
     * @brief Instantiate a tank control algorithm
     * 
     * Creates an appropriate strategy for a specific tank based on
     * which player it belongs to and its index number.
     * 
     * @param player_index The player owning this tank (1 or 2)
     * @param tank_index The tank's identifier within the player's fleet
     * @return unique_ptr<TankAlgorithm> A custom tank control algorithm
     */
    unique_ptr<TankAlgorithm> create(int player_index, int tank_index) const override;
};

#endif //MYTANKALGORITHMFACTORY_H
