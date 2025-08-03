#include "SimpleTankAlgorithm.h"
#include <random>
#include <chrono>
#include <iostream> // Include for debug output

namespace SimplePlugin {

SimpleTankAlgorithm::SimpleTankAlgorithm(int player_index, int tank_index) 
    : player_index_(player_index), tank_index_(tank_index), turn_count_(0) {
    
    // Initialize random number generator
    auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    rng_.seed(seed + player_index + tank_index);
    std::cout << "[SimpleTankAlgorithm] Constructed for player " << player_index << ", tank " << tank_index << std::endl;
}

ActionRequest SimpleTankAlgorithm::getAction() {
    turn_count_++;
    ActionRequest action;
    
    // More intelligent behavior based on turn count
    if (turn_count_ % 15 == 0) {
        // Shoot every 15 turns
        action = ActionRequest::Shoot;
    } else if (turn_count_ % 8 == 0) {
        // Change direction every 8 turns
        std::uniform_int_distribution<int> rotate_dist(0, 3);
        int rotate_action = rotate_dist(rng_);
        switch (rotate_action) {
            case 0: action = ActionRequest::RotateLeft45; break;
            case 1: action = ActionRequest::RotateRight45; break;
            case 2: action = ActionRequest::RotateLeft90; break;
            case 3: action = ActionRequest::RotateRight90; break;
        }
    } else if (turn_count_ % 3 == 0) {
        // Move backward occasionally for variety
        action = ActionRequest::MoveBackward;
    } else {
        // Most of the time, move forward
        action = ActionRequest::MoveForward;
    }
    
    std::cout << "[SimpleTankAlgorithm] Player " << player_index_ << ", tank " << tank_index_ << ", turn " << turn_count_ << ": action=" << static_cast<int>(action) << std::endl;
    return action;
}

void SimpleTankAlgorithm::updateBattleInfo(BattleInfo& info) {
    // Simple algorithm doesn't use battle info
    (void)info; // Suppress unused parameter warning
}

} // namespace SimplePlugin

// Registration not needed for static compilation
