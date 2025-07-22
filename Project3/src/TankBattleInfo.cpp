#include "TankBattleInfo.h"

TankBattleInfo::TankBattleInfo(const Position& tank_pos, Direction tank_dir, 
                               int shells, int cooldown, bool alive,
                               size_t map_width, size_t map_height)
    : tank_position_(tank_pos), tank_direction_(tank_dir), 
      shells_remaining_(shells), shoot_cooldown_(cooldown), alive_(alive),
      map_width_(map_width), map_height_(map_height) {
}
