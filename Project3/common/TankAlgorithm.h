#ifndef TANK_ALGORITHM_H
#define TANK_ALGORITHM_H

#include "ActionRequest.h"
#include "BattleInfo.h"
#include <memory>
#include <functional>

/**
 * Abstract base class for tank algorithms
 * Each algorithm must be able to decide on actions and process battle information
 */
class TankAlgorithm {
public:
    virtual ~TankAlgorithm() {}
    
    /**
     * Get the next action for this tank to perform
     * @return The action request
     */
    virtual ActionRequest getAction() = 0;
    
    /**
     * Update the algorithm with new battle information
     * @param info Battle information to process
     */
    virtual void updateBattleInfo(BattleInfo& info) = 0;
};

/**
 * Factory function type for creating tank algorithms
 * @param player_index Index of the player (0 or 1)
 * @param tank_index Index of the tank for that player
 * @return Unique pointer to a tank algorithm instance
 */
using TankAlgorithmFactory = 
    std::function<std::unique_ptr<TankAlgorithm>(int player_index, int tank_index)>;

#endif // TANK_ALGORITHM_H
