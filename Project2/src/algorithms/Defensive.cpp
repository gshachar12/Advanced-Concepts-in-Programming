#include "Defensive.h"
#include <iostream>
#include <cmath>
#include <algorithm>

Defensive::Defensive(int player_index, int tank_index)
    : playerIndex(player_index),
      tankIndex(tank_index),
      currentActionIndex(0),
      position({0, 0}),
      direction(Direction::UP),
      needsBattleInfo(true),
      enemyDetected(false),
      consecutiveShots(0)
{
    initActionSequence();
}

void Defensive::initActionSequence() {
    // Clear any existing actions
    actionSequence.clear();
    
    // First action is always to get battle info
    actionSequence.push_back(ActionRequest::GetBattleInfo);
    
    // Create a defensive sequence based on player and tank index
    // This algorithm prioritizes safety and strategic positioning
    
    // Begin with a scanning rotation to get a view of surroundings
    actionSequence.push_back(ActionRequest::RotateRight45);
    actionSequence.push_back(ActionRequest::GetBattleInfo);
    actionSequence.push_back(ActionRequest::RotateRight45);
    actionSequence.push_back(ActionRequest::GetBattleInfo);
    actionSequence.push_back(ActionRequest::RotateRight45);
    actionSequence.push_back(ActionRequest::GetBattleInfo);
    actionSequence.push_back(ActionRequest::RotateRight45);
    
    // Move to find cover
    actionSequence.push_back(ActionRequest::MoveForward);
    actionSequence.push_back(ActionRequest::MoveForward);
    actionSequence.push_back(ActionRequest::GetBattleInfo);
    
    // Offensive actions mixed with defensive positioning
    actionSequence.push_back(ActionRequest::RotateLeft45);
    actionSequence.push_back(ActionRequest::Shoot);
    actionSequence.push_back(ActionRequest::MoveBackward);
    actionSequence.push_back(ActionRequest::GetBattleInfo);
    
    // Reset action index
    currentActionIndex = 0;
}

ActionRequest Defensive::getAction() {
    // Immediately request battle info if needed
    if (needsBattleInfo) {
        needsBattleInfo = false;
        return ActionRequest::GetBattleInfo;
    }
    
    // Get current action or reset sequence if at end
    if (currentActionIndex >= actionSequence.size()) {
        currentActionIndex = 0;
        needsBattleInfo = true;
        return ActionRequest::GetBattleInfo;
    }
    
    ActionRequest action = actionSequence[currentActionIndex++];
    
    // Request battle info every few actions
    if (currentActionIndex % 3 == 0) {
        needsBattleInfo = true;
    }
    
    // Limit consecutive shots to prevent shell depletion
    if (action == ActionRequest::Shoot && consecutiveShots >= 3) {
        consecutiveShots = 0;
        // Instead of shooting, rotate to find a better angle
        return ActionRequest::RotateRight45;
    }
    
    if (action == ActionRequest::Shoot) {
        consecutiveShots++;
    } else {
        consecutiveShots = 0;
    }
    
    return action;
}

void Defensive::updateBattleInfo(BattleInfo& info) {
    // Cast to concrete TankBattleInfo class
    TankBattleInfo* battleInfo = dynamic_cast<TankBattleInfo*>(&info);
    if (!battleInfo) {
        std::cerr << "Error: BattleInfo is not a TankBattleInfo" << std::endl;
        return;
    }
    
    // Update our local state from the battle info
    position = battleInfo->getPosition();
    direction = battleInfo->getDirection();
    
    // Check for enemy tanks
    const auto& tanks = battleInfo->getTanks();
    enemyDetected = false;
    
    // Plan defensive actions based on battlefield information
    for (const auto& tank : tanks) {
        if (!tank.isMyTank) {
            enemyDetected = true;
            
            // Clear the current action sequence and replace with defensive tactics
            actionSequence.clear();
            currentActionIndex = 0;
            
            // If under threat, execute defensive maneuvers
            if (shouldRetreat(*battleInfo)) {
                // Retreat sequence: rotate away, move back, then shoot
                actionSequence.push_back(ActionRequest::RotateRight90);
                actionSequence.push_back(ActionRequest::MoveBackward);
                actionSequence.push_back(ActionRequest::MoveBackward);
                actionSequence.push_back(ActionRequest::GetBattleInfo);
                actionSequence.push_back(ActionRequest::RotateLeft90);
                actionSequence.push_back(ActionRequest::Shoot);
            } else {
                // Attack sequence: shoot then move to avoid counterfire
                actionSequence.push_back(ActionRequest::Shoot);
                actionSequence.push_back(ActionRequest::RotateRight45);
                actionSequence.push_back(ActionRequest::MoveForward);
                actionSequence.push_back(ActionRequest::GetBattleInfo);
            }
            
            break;
        }
    }
    
    // Check for nearby walls and use them for cover
    const auto& walls = battleInfo->getWalls();
    if (!walls.empty() && !enemyDetected) {
        // Find closest wall
        int closestDist = 999;
        Position closestWall{0, 0};
        
        for (const auto& wall : walls) {
            int dx = wall.x - position.x;
            int dy = wall.y - position.y;
            int dist = std::abs(dx) + std::abs(dy);
            
            if (dist < closestDist && dist > 1) { // Don't move directly next to walls
                closestDist = dist;
                closestWall = wall;
            }
        }
        
        // If found a suitable wall for cover, move toward it
        if (closestDist < 6 && closestDist > 1) {
            actionSequence.clear();
            currentActionIndex = 0;
            
            // Determine direction to wall
            int dx = closestWall.x - position.x;
            int dy = closestWall.y - position.y;
            
            // Rotate toward the wall
            if (std::abs(dx) > std::abs(dy)) {
                // Wall is more horizontal than vertical
                if (dx > 0) {
                    // Wall is to the right
                    actionSequence.push_back(ActionRequest::RotateRight45);
                    actionSequence.push_back(ActionRequest::RotateRight45);
                } else {
                    // Wall is to the left
                    actionSequence.push_back(ActionRequest::RotateLeft45);
                    actionSequence.push_back(ActionRequest::RotateLeft45);
                }
            } else {
                // Wall is more vertical than horizontal
                if (dy > 0) {
                    // Wall is below
                    actionSequence.push_back(ActionRequest::RotateRight90);
                } else {
                    // Wall is above
                    actionSequence.push_back(ActionRequest::RotateLeft90);
                }
            }
            
            // Move toward the wall, but not too close
            actionSequence.push_back(ActionRequest::MoveForward);
            actionSequence.push_back(ActionRequest::GetBattleInfo);
        }
    }
    
    // Always add GetBattleInfo periodically
    bool hasBattleInfoRequest = false;
    for (const auto& action : actionSequence) {
        if (action == ActionRequest::GetBattleInfo) {
            hasBattleInfoRequest = true;
            break;
        }
    }
    
    if (!hasBattleInfoRequest) {
        actionSequence.push_back(ActionRequest::GetBattleInfo);
    }
}

bool Defensive::shouldRetreat(const TankBattleInfo& info) const {
    // Count nearby enemy tanks
    int nearbyEnemies = 0;
    for (const auto& tank : info.getTanks()) {
        if (!tank.isMyTank) {
            int dx = tank.position.x - position.x;
            int dy = tank.position.y - position.y;
            int distance = std::abs(dx) + std::abs(dy);
            
            if (distance < 4) {
                nearbyEnemies++;
            }
        }
    }
    
    // Retreat if outnumbered or low on shells
    return (nearbyEnemies > 1) || (info.getRemainingShells() <= 2);
}

ActionRequest Defensive::planDefensiveAction(const TankBattleInfo& /* info */) {
    // This method would implement more sophisticated defensive logic
    // For now, we'll keep it simple
    return ActionRequest::MoveBackward;
}