#include "SimpleAlgorithm.h"

namespace Algorithm_123456789_987654321 {

SimpleAlgorithm::SimpleAlgorithm(int player_index, int tank_index) 
    : player_index_(player_index), tank_index_(tank_index), turn_count_(0) {
}

ActionRequest SimpleAlgorithm::getAction() {
    turn_count_++;
    
    // Simple algorithm: shoot every 5 turns, otherwise move forward
    if (turn_count_ % 5 == 0) {
        return ActionRequest::Shoot;
    } else if (turn_count_ % 10 == 3) {
        return ActionRequest::RotateLeft45;
    } else {
        return ActionRequest::MoveForward;
    }
}

void SimpleAlgorithm::updateBattleInfo(BattleInfo& info) {
    // For simple algorithm, we don't need to process battle info
    (void)info; // Suppress unused parameter warning
}

} // namespace Algorithm_123456789_987654321
