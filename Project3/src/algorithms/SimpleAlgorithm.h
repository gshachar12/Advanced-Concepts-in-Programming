#ifndef SIMPLE_ALGORITHM_H
#define SIMPLE_ALGORITHM_H

#include "../../common/TankAlgorithm.h"
#include "../../common/BattleInfo.h"
#include <random>

/**
 * Simple tank algorithm that performs basic actions
 * - Moves forward when possible
 * - Rotates when blocked
 * - Shoots occasionally
 */
class SimpleAlgorithm : public TankAlgorithm {
private:
    BattleInfo* current_battle_info_;
    std::mt19937 rng_;
    int turn_counter_;
    ActionRequest last_action_;
    int consecutive_failed_moves_;
    
    static const int SHOOT_FREQUENCY = 5;  // Shoot every 5 turns
    static const int MAX_FAILED_MOVES = 3; // Rotate after 3 failed moves
    
public:
    SimpleAlgorithm(int seed = 42);
    
    void updateBattleInfo(BattleInfo& info) override;
    ActionRequest getAction() override;

private:
    bool canMoveForward() const;
    ActionRequest chooseMovementAction();
    ActionRequest chooseRotationAction();
};

#endif // SIMPLE_ALGORITHM_H
