#include "CollisionDetector.h"

CollisionDetector::CollisionDetector(GameState* game_state) 
    : game_state_(game_state) {
}

void CollisionDetector::detectCollisions() {
    collision_events_.clear();
    detectShellCollisions();
    detectTankCollisions();
}

void CollisionDetector::resolveCollisions() {
    for (const CollisionEvent& event : collision_events_) {
        processCollisionEvent(event);
    }
    collision_events_.clear();
}

bool CollisionDetector::wouldTankCollide(const Tank* tank, const Position& new_pos) const {
    // Check bounds
    if (!game_state_->getBoard().isValidPosition(new_pos)) {
        return true;
    }
    
    // Check if position is passable
    if (!game_state_->getBoard().isPassable(new_pos)) {
        return true;
    }
    
    // Check for other tanks at position
    for (const Tank* other_tank : game_state_->getAllTanks()) {
        if (other_tank != tank && other_tank->isAlive() && 
            other_tank->getPosition() == new_pos) {
            return true;
        }
    }
    
    return false;
}

bool CollisionDetector::wouldShellHit(const Shell* shell, const Position& pos) const {
    // Check bounds
    if (!game_state_->getBoard().isValidPosition(pos)) {
        return true;
    }
    
    // Check cell type at position
    CellType cell = game_state_->getBoard().getCellType(pos);
    if (CellTypeUtils::blocksMovement(cell)) {
        return true;
    }
    
    // Check for tanks at position
    for (const Tank* tank : game_state_->getAllTanks()) {
        if (tank->isAlive() && tank->getPosition() == pos) {
            return true;
        }
    }
    
    return false;
}

void CollisionDetector::detectShellCollisions() {
    for (Shell* shell : game_state_->getAllShells()) {
        if (!shell->isActive()) {
            continue;
        }
        
        Position shell_pos = shell->getPosition();
        
        // Check bounds
        if (!game_state_->getBoard().isValidPosition(shell_pos)) {
            CollisionEvent event;
            event.type = CollisionEvent::SHELL_OUT_OF_BOUNDS;
            event.shell = shell;
            event.position = shell_pos;
            collision_events_.push_back(event);
            continue;
        }
        
        // Check wall collision
        CellType cell = game_state_->getBoard().getCellType(shell_pos);
        if (cell == CellType::WALL || cell == CellType::WEAK_WALL) {
            CollisionEvent event;
            event.type = CollisionEvent::SHELL_HITS_WALL;
            event.shell = shell;
            event.position = shell_pos;
            collision_events_.push_back(event);
            continue;
        }
        
        // Check mine collision
        if (cell == CellType::MINE) {
            CollisionEvent event;
            event.type = CollisionEvent::SHELL_HITS_WALL; // Treat mine like wall for shell
            event.shell = shell;
            event.position = shell_pos;
            collision_events_.push_back(event);
            continue;
        }
        
        // Check tank collision
        for (Tank* tank : game_state_->getAllTanks()) {
            if (tank->isAlive() && tank->getPosition() == shell_pos &&
                tank->getPlayerId() != shell->getOwnerPlayerId()) {
                CollisionEvent event;
                event.type = CollisionEvent::SHELL_HITS_TANK;
                event.shell = shell;
                event.tank = tank;
                event.position = shell_pos;
                collision_events_.push_back(event);
                break;
            }
        }
        
        // Check shell-shell collision
        for (Shell* other_shell : game_state_->getAllShells()) {
            if (other_shell != shell && other_shell->isActive() &&
                other_shell->getPosition() == shell_pos) {
                CollisionEvent event;
                event.type = CollisionEvent::SHELL_HITS_SHELL;
                event.shell = shell;
                event.position = shell_pos;
                collision_events_.push_back(event);
                break;
            }
        }
    }
}

void CollisionDetector::detectTankCollisions() {
    for (Tank* tank : game_state_->getAllTanks()) {
        if (!tank->isAlive()) {
            continue;
        }
        
        Position tank_pos = tank->getPosition();
        CellType cell = game_state_->getBoard().getCellType(tank_pos);
        
        // Check mine collision
        if (cell == CellType::MINE) {
            CollisionEvent event;
            event.type = CollisionEvent::TANK_HITS_MINE;
            event.tank = tank;
            event.position = tank_pos;
            collision_events_.push_back(event);
        }
    }
}

void CollisionDetector::processCollisionEvent(const CollisionEvent& event) {
    switch (event.type) {
        case CollisionEvent::SHELL_HITS_WALL:
            if (event.shell) {
                event.shell->deactivate();
                game_state_->getBoard().damageWall(event.position);
            }
            break;
            
        case CollisionEvent::SHELL_HITS_TANK:
            if (event.shell && event.tank) {
                event.shell->deactivate();
                event.tank->takeDamage();
            }
            break;
            
        case CollisionEvent::SHELL_HITS_SHELL:
            if (event.shell) {
                event.shell->deactivate();
                // Find and deactivate the other shell at same position
                for (Shell* other_shell : game_state_->getAllShells()) {
                    if (other_shell != event.shell && other_shell->isActive() &&
                        other_shell->getPosition() == event.position) {
                        other_shell->deactivate();
                        break;
                    }
                }
            }
            break;
            
        case CollisionEvent::TANK_HITS_MINE:
            if (event.tank) {
                event.tank->takeDamage();
                game_state_->getBoard().setCellType(event.position, CellType::EXPLOSION);
            }
            break;
            
        case CollisionEvent::SHELL_OUT_OF_BOUNDS:
            if (event.shell) {
                event.shell->deactivate();
            }
            break;
    }
}
