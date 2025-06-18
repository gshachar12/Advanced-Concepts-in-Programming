#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "Board.h"
#include "Tank.h"
#include "Shell.h"
#include "Controller.h"
#include "ActionType.h"
#include <vector>
#include <memory>
#include <string>
#include <fstream>

// Constants for game rules and settings
extern const int MAX_TURNS;
extern const int MAX_STEPS_WITHOUT_AMMO;
extern const int DISPLAY_DELAY_MS;

class GameManager {
private:
    Tank tank1;
    Tank tank2;
    std::vector<Shell> shells;  // Vector to store all shells in the game
    std::ofstream outFile;
    std::unique_ptr<Controller> alg1;
    std::unique_ptr<Controller> alg2;
    std::shared_ptr<Board> board;
    bool gameOver;
    int stepsSinceBothAmmoZero;
    int turnCount;
    bool visualMode;  // if true, the board is displayed each turn

public:
    // Construct tanks with string directions (e.g. "L" for left, "R" for right)
    GameManager(std::unique_ptr<Controller> a1,
                std::unique_ptr<Controller> a2,
                std::unique_ptr<Board> b,
                bool visualModeFlag
                )
            : 
              tank1(0, 0, "L", CellType::TANK1, 1),   // starting positions; they will be set in initializeGame()
              tank2(0, 0, "R", CellType::TANK2,  2),
              alg1(std::move(a1)),
              alg2(std::move(a2)),
              board(std::move(b)),
              gameOver(false),
              stepsSinceBothAmmoZero(0),
              turnCount(0),
              visualMode(visualModeFlag)
    {}


    GameManager(Tank t1, Tank t2,
                std::unique_ptr<Controller> a1,
                std::unique_ptr<Controller> a2,
                std::unique_ptr<Board> b,
                bool visualModeFlag)
            :
              tank1(std::move(t1)),
              tank2(std::move(t2)),
              alg1(std::move(a1)),
              alg2(std::move(a2)),
              board(std::move(b)),
              gameOver(false),
              stepsSinceBothAmmoZero(0),
              turnCount(0),
              visualMode(visualModeFlag)
    {}

    bool initializeGame();
    void runGameLoop(const std::string& boardFile);

private:
    void applyAction( Tank &tank, ActionType action);
    void moveTank(int dx, int dy, Tank &tank);
    void updateShells();
    void tanksCollided();
    bool ShellHit(Shell &shell);
    void GameSummary();
    bool hitWall(int x, int y, Tank &tank);
    void checkEndGameConditions();
    void printToBoth(const std::string &message); 
    void displayGame();
};

#endif // GAMEMANAGER_H
