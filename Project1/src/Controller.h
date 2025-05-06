#pragma once
#include "ActionType.h"
#include "Tank.h"
#include "Shell.h"
#include <vector>

class Controller {

public:
    virtual ~Controller() = default;
    virtual ActionType ChaseTank( Board board, Tank &myTank,Tank &enemyTank, const std::vector<Shell> &shells);
    ActionType EvadeTank(Board board, Tank &myTank, Tank &enemyTank);
    ActionType AvoidShells(Board board, Tank &myTank, const std::vector<Shell> &shells);

protected:
    // Helper methods
    bool isValidPosition(Board board, int x, int y);

private:
    static int count; 
    bool CanShoot(Tank &myTank, Tank &enemyTank);
    Position BFS(Board board, Position chaserStart, Position target);
    bool IsShellChasingTank(const Shell &shell, const Tank &myTank);
    bool IsSafeToMoveForward( Board board,Tank &myTank);
    bool IsMineNearby(Board board, Tank &myTank);
    bool IsTankNearby( Tank &myTank, Tank &enemyTank);
    bool IsInLineOfSight( Tank &myTank, Tank &enemyTank);
   // bool IsInRange(const Tank &enemyTank);
    //bool IsObstacleAhead(const Tank &myTank);
    
    // New helper methods for pickEvadeDirection
    ActionType handleCloseEvade(Board board, Tank &myTank, Tank &enemyTank, const Position &myPos, const Position &enemyPos, double distance);
    ActionType handleFacingEnemyEvade(Board board, Tank &myTank);
    ActionType handleFacingAwayEvade(Board board, Tank &myTank);
    ActionType calculateRotationDirection(Direction current, Direction desired);
    ActionType handleDistantEvade(Board board, Tank &myTank, Tank &enemyTank, const Position &myPos, const Position &enemyPos, double distance);
    Direction findDirectionMaximizingDistance(Board board,Tank &myTank, const Position &enemyPos, double currentDistance);
};
