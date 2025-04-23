#pragma once
#include "ActionType.h"
#include "Tank.h"
#include "Shell.h"
#include <vector>

class Controller {
public:
    virtual ~Controller() = default;

    virtual ActionType DecideAction(
        const Tank &myTank,
        const Tank &enemyTank,
        const std::vector<Shell> &shells
    );
};
