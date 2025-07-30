#ifndef MY_BATTLE_INFO_H
#define MY_BATTLE_INFO_H

#include "../common/BattleInfo.h"

namespace GameManager_123456789_987654321 {

/**
 * Custom BattleInfo implementation for our GameManager
 */
class MyBattleInfo : public BattleInfo {
public:
    // Tank position and status
    size_t tank_position_x;
    size_t tank_position_y;
    int tank_direction;
    int tank_shells_remaining;
    int tank_cooldown;
    
    // Game state
    size_t current_turn;
    size_t max_turns;
    size_t map_width;
    size_t map_height;
    
    // Battle statistics
    int friendly_tanks_count;
    int enemy_tanks_count;
    int shells_in_flight;
    
    MyBattleInfo() : 
        tank_position_x(0), tank_position_y(0), tank_direction(0),
        tank_shells_remaining(0), tank_cooldown(0),
        current_turn(0), max_turns(0), map_width(0), map_height(0),
        friendly_tanks_count(0), enemy_tanks_count(0), shells_in_flight(0) {}
        
    virtual ~MyBattleInfo() = default;
};

} // namespace GameManager_123456789_987654321

#endif // MY_BATTLE_INFO_H
