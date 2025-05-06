#pragma once
#include "ActionType.h"
#include "Tank.h"
#include "Shell.h"
#include <vector>

class Controller {

public:
    virtual ~Controller() = default;
    virtual ActionType ChaseTank( Board board, Tank &myTank,Tank &enemyTank, const std::vector<Shell> &shells);
    ActionType pickEvadeDirection(Board board, Tank &myTank, Tank &enemyTank);
    ActionType AvoidShells(Board board, Tank &myTank, const std::vector<Shell> &shells);

protected:
    // Helper methods
    bool isValidPosition(Board board, int x, int y);

private:
    static int count; 
    bool CanShoot(Tank &myTank, Tank &enemyTank);
    Position BFS(Board board, Position chaserStart, Position target);
    bool IsShellChasingTank(const Shell &shell, const Tank &myTank);
    bool IsSafeToMoveForward( Tank &myTank);
    bool IsMineNearby( Tank &myTank);
    bool IsTankNearby( Tank &myTank);
    bool IsInLineOfSight(const Tank &enemyTank);
    bool IsInRange(const Tank &enemyTank);
    bool IsObstacleAhead(const Tank &myTank);
    
    // New helper methods for pickEvadeDirection
    ActionType handleCloseEvade(Tank &myTank, Tank &enemyTank, const Position &myPos, const Position &enemyPos, double distance);
    ActionType handleFacingEnemyEvade(Tank &myTank);
    ActionType handleFacingAwayEvade(Tank &myTank);
    ActionType calculateRotationDirection(Direction current, Direction desired);
    ActionType handleDistantEvade(Tank &myTank, Tank &enemyTank, const Position &myPos, const Position &enemyPos, double distance);
    Direction findDirectionMaximizingDistance(Tank &myTank, const Position &enemyPos, double currentDistance);
};
