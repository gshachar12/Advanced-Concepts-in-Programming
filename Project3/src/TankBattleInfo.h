#ifndef TANK_BATTLE_INFO_H
#define TANK_BATTLE_INFO_H

#include "../common/BattleInfo.h"
#include "Position.h"
#include "Direction.h"
#include <vector>

/**
 * Concrete implementation of BattleInfo containing tank-specific information
 */
class TankBattleInfo : public BattleInfo {
private:
    Position tank_position_;
    Direction tank_direction_;
    int shells_remaining_;
    int shoot_cooldown_;
    bool alive_;
    
    // Enemy information (if visible)
    std::vector<Position> visible_enemies_;
    std::vector<Position> visible_shells_;
    
    // Map information
    size_t map_width_;
    size_t map_height_;

public:
    TankBattleInfo(const Position& tank_pos, Direction tank_dir, 
                   int shells, int cooldown, bool alive,
                   size_t map_width, size_t map_height);
    
    // Tank state getters
    const Position& getTankPosition() const { return tank_position_; }
    Direction getTankDirection() const { return tank_direction_; }
    int getShellsRemaining() const { return shells_remaining_; }
    int getShootCooldown() const { return shoot_cooldown_; }
    bool isAlive() const { return alive_; }
    
    // Map information
    size_t getMapWidth() const { return map_width_; }
    size_t getMapHeight() const { return map_height_; }
    
    // Enemy and shell information
    const std::vector<Position>& getVisibleEnemies() const { return visible_enemies_; }
    const std::vector<Position>& getVisibleShells() const { return visible_shells_; }
    
    // Add visible entities
    void addVisibleEnemy(const Position& pos) { visible_enemies_.push_back(pos); }
    void addVisibleShell(const Position& pos) { visible_shells_.push_back(pos); }
    
    // Clear visible entities (for updates)
    void clearVisibleEntities() { 
        visible_enemies_.clear(); 
        visible_shells_.clear(); 
    }
};

#endif // TANK_BATTLE_INFO_H
