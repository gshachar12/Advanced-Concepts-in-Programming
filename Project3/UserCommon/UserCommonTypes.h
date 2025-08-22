#ifndef USER_COMMON_TYPES_H
#define USER_COMMON_TYPES_H

#include "../common/ActionRequest.h"
#include "../common/BattleInfo.h"
#include "../common/TankAlgorithm.h"

/**
 * UserCommon namespace for shared user definitions
 * This namespace should contain any common types, constants, or utilities
 * that might be shared between different student implementations
 */
namespace UserCommon_123456789_987654321 {

// Common game constants
const int MAX_SHELLS_PER_TANK = 10;
const int SHELL_COOLDOWN_TURNS = 3;
const int MAX_TURNS = 100;

// Direction constants
enum Direction {
    NORTH = 0,
    NORTHEAST = 1,
    EAST = 2,
    SOUTHEAST = 3,
    SOUTH = 4,
    SOUTHWEST = 5,
    WEST = 6,
    NORTHWEST = 7
};

// Cell types
enum CellType {
    EMPTY = ' ',
    WALL = '#',
    WEAK_WALL = '=',
    MINE = '@',
    SHELL = '*',
    EXPLOSION = 'X',
    TANK_PLAYER1 = '1',
    TANK_PLAYER2 = '2'
};

// Game result reasons
enum GameEndReason {
    ALL_ENEMY_TANKS_DESTROYED,
    MAX_TURNS_REACHED,
    PLAYER_DISCONNECTED,
    TECHNICAL_FAILURE
};

} // namespace UserCommon_123456789_987654321

#endif // USER_COMMON_TYPES_H
