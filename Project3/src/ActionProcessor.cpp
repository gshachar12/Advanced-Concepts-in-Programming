#include "ActionProcessor.h"
#include <cmath>

ActionProcessor::ActionProcessor(GameState* game_state, CollisionDetector* collision_detector)
    : game_state_(game_state), collision_detector_(collision_detector) {
}

bool ActionProcessor::processAction(Tank* tank, ActionRequest action) {
    if (!tank || !tank->isAlive()) {
        return false;
    }
    
    switch (action) {
        case ActionRequest::MoveForward:
            return processMoveForward(tank);
        case ActionRequest::MoveBackward:
            return processMoveBackward(tank);
        case ActionRequest::RotateLeft90:
            return processRotateLeft90(tank);
        case ActionRequest::RotateRight90:
            return processRotateRight90(tank);
        case ActionRequest::RotateLeft45:
            return processRotateLeft45(tank);
        case ActionRequest::RotateRight45:
            return processRotateRight45(tank);
        case ActionRequest::Shoot:
            return processShoot(tank);
        case ActionRequest::GetBattleInfo:
            // Battle info is provided separately, this is a no-op
            return true;
        case ActionRequest::DoNothing:
            return true;
        default:
            return false;
    }
}

std::unique_ptr<TankBattleInfo> ActionProcessor::createBattleInfo(const Tank* tank) const {
    auto battle_info = std::make_unique<TankBattleInfo>(
        tank->getPosition(),
        tank->getDirection(),
        tank->getShellsRemaining(),
        tank->getShootCooldown(),
        tank->isAlive(),
        game_state_->getBoard().getWidth(),
        game_state_->getBoard().getHeight()
    );
    
    // Add visible enemies
    std::vector<Position> visible_enemies = getVisibleEnemies(tank);
    for (const Position& pos : visible_enemies) {
        battle_info->addVisibleEnemy(pos);
    }
    
    // Add visible shells
    std::vector<Position> visible_shells = getVisibleShells(tank);
    for (const Position& pos : visible_shells) {
        battle_info->addVisibleShell(pos);
    }
    
    return battle_info;
}

bool ActionProcessor::processMoveForward(Tank* tank) {
    Position new_pos = tank->getForwardPosition();
    
    if (!collision_detector_->wouldTankCollide(tank, new_pos)) {
        tank->moveForward();
        return true;
    }
    
    return false;
}

bool ActionProcessor::processMoveBackward(Tank* tank) {
    Position new_pos = tank->getBackwardPosition();
    
    if (!collision_detector_->wouldTankCollide(tank, new_pos)) {
        tank->moveBackward();
        return true;
    }
    
    return false;
}

bool ActionProcessor::processRotateLeft90(Tank* tank) {
    tank->rotateLeft90();
    return true;
}

bool ActionProcessor::processRotateRight90(Tank* tank) {
    tank->rotateRight90();
    return true;
}

bool ActionProcessor::processRotateLeft45(Tank* tank) {
    tank->rotateLeft45();
    return true;
}

bool ActionProcessor::processRotateRight45(Tank* tank) {
    tank->rotateRight45();
    return true;
}

bool ActionProcessor::processShoot(Tank* tank) {
    if (!tank->canShoot()) {
        return false;
    }
    
    tank->shoot();
    
    // Create shell at tank's position
    game_state_->addShell(
        tank->getPosition(),
        tank->getDirection(),
        tank->getPlayerId(),
        tank->getTankId()
    );
    
    return true;
}

std::vector<Position> ActionProcessor::getVisibleEnemies(const Tank* tank) const {
    std::vector<Position> visible;
    
    for (const Tank* enemy : game_state_->getAllTanks()) {
        if (enemy->getPlayerId() != tank->getPlayerId() && 
            enemy->isAlive() &&
            isPositionVisible(tank->getPosition(), enemy->getPosition())) {
            visible.push_back(enemy->getPosition());
        }
    }
    
    return visible;
}

std::vector<Position> ActionProcessor::getVisibleShells(const Tank* tank) const {
    std::vector<Position> visible;
    
    for (const Shell* shell : game_state_->getAllShells()) {
        if (shell->isActive() &&
            isPositionVisible(tank->getPosition(), shell->getPosition())) {
            visible.push_back(shell->getPosition());
        }
    }
    
    return visible;
}

bool ActionProcessor::isPositionVisible(const Position& from, const Position& to) const {
    // Simple line-of-sight check - no obstacles blocking
    // For simplicity, we'll use a basic implementation
    // In a more sophisticated version, we could implement ray casting
    
    int dx = abs(to.x - from.x);
    int dy = abs(to.y - from.y);
    
    // For now, consider positions visible if they're within a reasonable distance
    // and not blocked by walls
    const int MAX_VISION_DISTANCE = 10;
    
    if (dx + dy > MAX_VISION_DISTANCE) {
        return false;
    }
    
    // Simple check: if there's a direct line without major obstacles
    // This is a simplified implementation
    return true;
}
