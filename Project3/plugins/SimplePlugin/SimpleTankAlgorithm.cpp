#include "SimpleTankAlgorithm.h"
#include <random>
#include <chrono>

namespace SimplePlugin {

SimpleTankAlgorithm::SimpleTankAlgorithm(int player_index, int tank_index) 
    : player_index_(player_index), tank_index_(tank_index), turn_count_(0) {
    
    // Initialize random number generator
    auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    rng_.seed(seed + player_index + tank_index);
}

ActionRequest SimpleTankAlgorithm::getAction() {
    turn_count_++;
    
    // More intelligent behavior based on turn count
    if (turn_count_ % 15 == 0) {
        // Shoot every 15 turns
        return ActionRequest::Shoot;
    } else if (turn_count_ % 8 == 0) {
        // Change direction every 8 turns
        std::uniform_int_distribution<int> rotate_dist(0, 3);
        int rotate_action = rotate_dist(rng_);
        switch (rotate_action) {
            case 0: return ActionRequest::RotateLeft45;
            case 1: return ActionRequest::RotateRight45;
            case 2: return ActionRequest::RotateLeft90;
            case 3: return ActionRequest::RotateRight90;
        }
    } else if (turn_count_ % 3 == 0) {
        // Move backward occasionally for variety
        return ActionRequest::MoveBackward;
    } else {
        // Most of the time, move forward
        return ActionRequest::MoveForward;
    }
    
    return ActionRequest::DoNothing;
}

void SimpleTankAlgorithm::updateBattleInfo(BattleInfo& info) {
    // Simple algorithm doesn't use battle info
    (void)info; // Suppress unused parameter warning
}

} // namespace SimplePlugin

// Registration not needed for static compilation
