#ifndef SIMPLE_PLUGIN_ALGORITHM_H
#define SIMPLE_PLUGIN_ALGORITHM_H

#include "../../common/TankAlgorithm.h"
#include "../../common/BattleInfo.h"
#include "../../common/ActionRequest.h"
#include <random>

/**
 * Simple algorithm plugin for dynamic loading
 * This demonstrates how to create a plugin algorithm
 */
namespace SimplePlugin {

class SimpleTankAlgorithm : public TankAlgorithm {
private:
    int player_index_;
    int tank_index_;
    mutable std::mt19937 rng_;
    mutable int turn_count_;
    
public:
    SimpleTankAlgorithm(int player_index, int tank_index);
    
    ActionRequest getAction() override;
    void updateBattleInfo(BattleInfo& info) override;
};

} // namespace SimplePlugin

#endif // SIMPLE_PLUGIN_ALGORITHM_H
