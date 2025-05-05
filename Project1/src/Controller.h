#pragma once
#include "ActionType.h"
#include "Tank.h"
#include "Shell.h"
#include <vector>

class Controller {
public:
    virtual ~Controller() = default;


    // Main action decision function
    virtual ActionType DecideAction(
        Tank &myTank,
        Tank &enemyTank,
        const std::vector<Shell> &shells
    );
    ActionType pickEvadeDirection(Tank &myTank, Tank &enemyTank);

protected:
    // Helper methods
    bool isValidPosition(int x, int y);

private:
    bool CanShoot(const Tank &enemyTank);
    ActionType chaseTank(Position chaserStart, Position target);
    Direction angleToClosestDirection(double angle);
    bool IsShellChasingTank(const Shell &shell, const Tank &myTank);
    bool IsSafeToMoveForward( Tank &myTank);
    bool IsMineNearby( Tank &myTank);
    bool IsTankNearby( Tank &myTank);
    bool IsInLineOfSight(const Tank &enemyTank);
    bool IsInRange(const Tank &enemyTank);
    bool IsObstacleAhead(const Tank &myTank);
};
