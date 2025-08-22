#include "BfsAlgorithm.h"
#include "../Direction.h"
#include <algorithm>
#include <cmath>

BfsAlgorithm::BfsAlgorithm() 
    : current_battle_info_(nullptr), action_index_(0), turn_counter_(0) {
}

void BfsAlgorithm::updateBattleInfo(BattleInfo& info) {
    // Try to cast to TankBattleInfo to get more detailed information
    current_battle_info_ = dynamic_cast<TankBattleInfo*>(&info);
    turn_counter_++;
    
    // Replan actions periodically or when we've completed our current plan
    if (action_index_ >= planned_actions_.size() || 
        turn_counter_ % REPLANNING_FREQUENCY == 0) {
        planActions();
        action_index_ = 0;
    }
}

ActionRequest BfsAlgorithm::getAction() {
    if (!current_battle_info_) {
        return ActionRequest::DoNothing;
    }
    
    // Check if we should shoot first (high priority action)
    if (shouldShoot()) {
        return ActionRequest::Shoot;
    }
    
    // Execute planned actions
    if (action_index_ < planned_actions_.size()) {
        ActionRequest action = planned_actions_[action_index_];
        action_index_++;
        return action;
    }
    
    // No planned actions, replan immediately
    planActions();
    if (!planned_actions_.empty()) {
        ActionRequest action = planned_actions_[0];
        action_index_ = 1;
        return action;
    }
    
    return ActionRequest::DoNothing;
}

void BfsAlgorithm::planActions() {
    planned_actions_.clear();
    
    if (!current_battle_info_) {
        return;
    }
    
    // Find the best target position
    auto [target_x, target_y] = findBestTarget();
    
    // Find path to target
    std::vector<ActionRequest> path = findPathToTarget(target_x, target_y);
    
    // Add the path to our planned actions
    planned_actions_.insert(planned_actions_.end(), path.begin(), path.end());
    
    // If path is empty or short, add some exploration moves
    if (planned_actions_.size() < 3) {
        planned_actions_.push_back(ActionRequest::MoveForward);
        planned_actions_.push_back(ActionRequest::RotateRight45);
        planned_actions_.push_back(ActionRequest::MoveForward);
    }
}

std::vector<ActionRequest> BfsAlgorithm::findPathToTarget(int target_x, int target_y) {
    if (!current_battle_info_) {
        return {};
    }
    
    int start_x = current_battle_info_->getTankPosition().x;
    int start_y = current_battle_info_->getTankPosition().y;
    
    if (start_x == target_x && start_y == target_y) {
        return {}; // Already at target
    }
    
    std::queue<Node> queue;
    std::set<std::pair<int, int>> visited;
    
    queue.emplace(start_x, start_y, 0, ActionRequest::DoNothing);
    visited.insert({start_x, start_y});
    
    while (!queue.empty() && queue.front().steps < MAX_SEARCH_DEPTH) {
        Node current = queue.front();
        queue.pop();
        
        // Check all possible moves from current position
        std::vector<std::pair<std::pair<int, int>, ActionRequest>> moves = {
            {{current.x, current.y - 1}, ActionRequest::MoveForward},  // Assuming north is up
            {{current.x, current.y + 1}, ActionRequest::MoveBackward},
            {{current.x - 1, current.y}, ActionRequest::MoveForward},  // Will need rotation
            {{current.x + 1, current.y}, ActionRequest::MoveForward}   // Will need rotation
        };
        
        for (auto& [new_pos, action] : moves) {
            int new_x = new_pos.first;
            int new_y = new_pos.second;
            
            if (visited.count({new_x, new_y})) {
                continue;
            }
            
            if (!isValidPosition(new_x, new_y) || !isPositionSafe(new_x, new_y)) {
                continue;
            }
            
            // Check if we reached the target
            if (new_x == target_x && new_y == target_y) {
                // Reconstruct path (simplified - just return the first move)
                return {action};
            }
            
            visited.insert({new_x, new_y});
            queue.emplace(new_x, new_y, current.steps + 1, action);
        }
    }
    
    // No path found, return a simple movement
    return {ActionRequest::MoveForward};
}

bool BfsAlgorithm::isValidPosition(int x, int y) const {
    if (!current_battle_info_) {
        return false;
    }
    
    // Simple bounds checking (would need actual map dimensions)
    return x >= 0 && y >= 0 && x < 20 && y < 20; // Assuming 20x20 map for now
}

bool BfsAlgorithm::isPositionSafe(int x, int y) const {
    // In a real implementation, we would check:
    // - No walls at this position
    // - Not in path of enemy shells
    // - Not too close to enemy tanks
    
    // For now, just return true (simplified)
    return true;
}

ActionRequest BfsAlgorithm::getMovementAction(int from_x, int from_y, int to_x, int to_y) const {
    // Determine the direction to move
    if (to_x > from_x) {
        return ActionRequest::MoveForward; // East (assuming tank is facing east)
    } else if (to_x < from_x) {
        return ActionRequest::MoveBackward; // West
    } else if (to_y < from_y) {
        return ActionRequest::MoveForward; // North (assuming tank is facing north)
    } else if (to_y > from_y) {
        return ActionRequest::MoveBackward; // South
    }
    
    return ActionRequest::DoNothing;
}

std::pair<int, int> BfsAlgorithm::findBestTarget() const {
    if (!current_battle_info_) {
        return {10, 10}; // Default center position
    }
    
    // Simple target selection - move towards center if no specific target
    // In a real implementation, this would analyze:
    // - Enemy tank positions
    // - Strategic positions
    // - Resource locations
    // - Safe positions
    
    return {10, 10}; // Move towards center for now
}

bool BfsAlgorithm::shouldShoot() const {
    if (!current_battle_info_) {
        return false;
    }
    
    // Simple shooting logic:
    // - We have shells
    // - We're not on cooldown
    // - Occasionally shoot (every 7 turns)
    
    return (turn_counter_ % 7 == 0);
}
