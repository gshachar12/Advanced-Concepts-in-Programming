#include "SimpleTankAlgorithm.h"
#include "../../common/TankAlgorithmRegistration.h"
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
    
    // Simple random behavior
    std::uniform_int_distribution<int> action_dist(0, 8);
    int action = action_dist(rng_);
    
    switch (action) {
        case 0: return ActionRequest::MoveForward;
        case 1: return ActionRequest::MoveBackward;
        case 2: return ActionRequest::RotateLeft90;
        case 3: return ActionRequest::RotateRight90;
        case 4: return ActionRequest::RotateLeft45;
        case 5: return ActionRequest::RotateRight45;
        case 6: return ActionRequest::Shoot;
        case 7: return ActionRequest::GetBattleInfo;
        default: return ActionRequest::DoNothing;
    }
}

void SimpleTankAlgorithm::updateBattleInfo(BattleInfo& info) {
    // Simple algorithm doesn't use battle info
    (void)info; // Suppress unused parameter warning
}

} // namespace SimplePlugin

// Register the algorithm for dynamic loading
using namespace SimplePlugin;
REGISTER_TANK_ALGORITHM(SimpleTankAlgorithm);
