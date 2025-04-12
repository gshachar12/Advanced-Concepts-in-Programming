#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "../ActionType.h"  // now we include the new header
#include <vector>

// Forward declarations for classes we need (so we do not create circular dependencies)
class Board;
class Tank;
class Shell;

class Controller {
public:
    virtual ActionType decideAction(const Board &board, const Tank &myTank,
                                    const Tank &enemyTank, const std::vector<Shell> &shells) {
        return ActionType::MOVE_FORWARD;
    }
    virtual ~Controller() {}
};

#endif // CONTROLLER_H
