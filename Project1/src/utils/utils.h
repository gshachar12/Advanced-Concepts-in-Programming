#ifndef TANK_GAME_H
#define TANK_GAME_H

#include <vector>

// Enum for backward movement states
enum class BackwardState {
    NOT_REQUESTED,    // normal state
    WAITING_1,        // first waiting step
    WAITING_2,        // second waiting step
    MOVING_BACKWARD,  // about to execute the backward move
};

#endif // TANK_GAME_H
