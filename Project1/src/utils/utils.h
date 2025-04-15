#ifndef TANK_GAME_H
#define TANK_GAME_H

#include <vector>


// // Enum for the cell type
// enum class CellType {
//     EMPTY,
//     WALL,
//     MINE,
// };

// Enum for the direction of the tank's cannon
enum class CannonDirection {
    UP,
    DOWN,
    RIGHT,
    LEFT,
    UR, // up-right
    DR, // down-right
    DL, // down-left
    UL // up-left
};

// Enum for possible tank actions
enum class TankActionType {
    MOVE_FORWARD,
    MOVE_BACKWARD,
    ROTATE_LEFT_1_8,
    ROTATE_RIGHT_1_8,
    ROTATE_LEFT_1_4,
    ROTATE_RIGHT_1_4,
    SHOOT,
    NONE // in case no valid or no desired action
};

// Enum for backward movement states
enum class BackwardState {
    NOT_REQUESTED,    // normal state
    WAITING_1,        // first waiting step
    WAITING_2,        // second waiting step
    MOVING_BACKWARD,  // about to execute the backward move
};


#endif // TANK_GAME_H
