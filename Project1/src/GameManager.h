#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "Board.h"
#include "Tank.h"
#include "Shell.h"
#include "algorithms/Controller.h"
#include "ActionType.h"
#include "Globals.h"
#include <vector>
#include <memory>
#include <string>

class GameManager {
private:
    Tank tank1;
    Tank tank2;
    std::vector<Shell> shells;  // Vector to store all shells in the game

    std::unique_ptr<Controller> alg1;
    std::unique_ptr<Controller> alg2;

    bool gameOver;
    int stepsSinceBothAmmoZero;
    int turnCount;
    bool visualMode;  // if true, the board is displayed each turn

public:
    // Construct tanks with string directions (e.g. "L" for left, "R" for right)
    GameManager(std::unique_ptr<Controller> a1,
                std::unique_ptr<Controller> a2,
                bool visualModeFlag)
            : 
              tank1(0, 0, "L", CellType::TANK1, 1),   // starting positions; they will be set in initializeGame()
              tank2(0, 0, "R", CellType::TANK2,  2),
              alg1(std::move(a1)),
              alg2(std::move(a2)),
              gameOver(false),
              stepsSinceBothAmmoZero(0),
              turnCount(0),
              visualMode(visualModeFlag)
    {}


    GameManager(Tank t1, Tank t2,
                std::unique_ptr<Controller> a1,
                std::unique_ptr<Controller> a2,
                bool visualModeFlag)
            :
              tank1(std::move(t1)),
              tank2(std::move(t2)),
              alg1(std::move(a1)),
              alg2(std::move(a2)),
              gameOver(false),
              stepsSinceBothAmmoZero(0),
              turnCount(0),
              visualMode(visualModeFlag)
    {}

    bool initializeGame(const std::string &boardFile);
    void runGameLoop();

private:
    void applyAction(Tank &tank, ActionType action);
    void updateShells();
    void checkCollisions();
    void checkEndGameConditions();
    void displayGame();
};

#endif // GAMEMANAGER_H
