#include "SimpleAlgorithm.h"
#include <iostream>

SimpleAlgorithm::SimpleAlgorithm(int seed) 
    : current_battle_info_(nullptr), rng_(seed), turn_counter_(0), 
      last_action_(ActionRequest::DoNothing), consecutive_failed_moves_(0) {
}

void SimpleAlgorithm::updateBattleInfo(BattleInfo& info) {
    current_battle_info_ = &info;
    turn_counter_++;
}

ActionRequest SimpleAlgorithm::getAction() {
    if (!current_battle_info_) {
        return ActionRequest::DoNothing;
    }
    
    // Simple strategy:
    // 1. Shoot every SHOOT_FREQUENCY turns if we can
    // 2. Try to move forward
    // 3. If movement fails too many times, rotate
    // 4. Occasionally do nothing to add unpredictability
    
    // Check if we should shoot
    if (turn_counter_ % SHOOT_FREQUENCY == 0) {
        last_action_ = ActionRequest::Shoot;
        return ActionRequest::Shoot;
    }
    
    // Check if we've failed to move too many times
    if (consecutive_failed_moves_ >= MAX_FAILED_MOVES) {
        consecutive_failed_moves_ = 0; // Reset counter
        last_action_ = chooseRotationAction();
        return last_action_;
    }
    
    // Try to move forward most of the time
    if (canMoveForward()) {
        last_action_ = chooseMovementAction();
        return last_action_;
    } else {
        // Can't move forward, try rotating
        consecutive_failed_moves_++;
        last_action_ = chooseRotationAction();
        return last_action_;
    }
}

bool SimpleAlgorithm::canMoveForward() const {
    // This is a simplified check - in a real implementation,
    // we would analyze the battle info to see if forward movement is possible
    // For now, we'll use some randomness to simulate decision making
    std::uniform_int_distribution<int> dist(1, 10);
    return dist(const_cast<std::mt19937&>(rng_)) > 3; // 70% chance to move forward
}

ActionRequest SimpleAlgorithm::chooseMovementAction() {
    // Choose between forward and backward movement
    std::uniform_int_distribution<int> dist(1, 10);
    if (dist(rng_) <= 8) {
        return ActionRequest::MoveForward;  // 80% forward
    } else {
        return ActionRequest::MoveBackward; // 20% backward
    }
}

ActionRequest SimpleAlgorithm::chooseRotationAction() {
    // Choose rotation direction and amount
    std::uniform_int_distribution<int> dist(1, 4);
    
    switch (dist(rng_)) {
        case 1: return ActionRequest::RotateLeft45;
        case 2: return ActionRequest::RotateRight45;
        case 3: return ActionRequest::RotateLeft90;
        case 4: return ActionRequest::RotateRight90;
        default: return ActionRequest::RotateRight45;
    }
}
