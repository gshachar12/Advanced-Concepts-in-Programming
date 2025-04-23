#include "Controller.h"
#include <iostream>

// Do NOT re-declare or re-define the class here.
// Just implement the method:

ActionType Controller::DecideAction(
    const Tank &myTank,
    const Tank &enemyTank,
    const std::vector<Shell> &shells
) {
    static int counter = 0;
    if (counter % 2 == 0)
        return ActionType::MOVE_FORWARD;
    else
        return ActionType::ROTATE_LEFT_1_8;

    ++counter;
}
