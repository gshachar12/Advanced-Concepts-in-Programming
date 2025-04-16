#include "GameManager.h"
#include "GameObject.h"
#include "Globals.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <algorithm>

ActionType DecideAction(const Tank &myTank,
                        const Tank &enemyTank, const std::vector<Shell> &shells)
{
    static int actionCounter = 0; // Static variable to keep track of the calls

    // Define the sequence of actions you want to cycle through
    ActionType actions[] = {

        ActionType::ROTATE_LEFT_1_4,


       // ActionType::SHOOT
    };

    // Determine the action based on the current value of actionCounter
    ActionType action = actions[actionCounter % (sizeof(actions) / sizeof(actions[0]))];

    // Increment actionCounter for the next call
    actionCounter++;

    return action;
}

bool GameManager::initializeGame(const std::string &boardFile)
{
    if (Global::board == nullptr)
    {
        std::cerr << "Error: Global board is nullptr!" << std::endl;
        return false;
    }

    return true;
}

void GameManager::runGameLoop()
{
    turnCount = 0;

    while (!gameOver && turnCount < 1000)
    {
        turnCount++;
        if (visualMode)
        {
            std::cout << "\033[2J\033[1;1H";
        }
        std::cout << "Turn " << turnCount << std::endl;

        tank1.update();
        tank2.update();

        ActionType action1 = DecideAction(tank1, tank2, shells);
        ActionType action2 = DecideAction(tank2, tank1, shells);

        applyAction(tank1, action1);
        applyAction(tank2, action2);
        tank2.printPosition();
        updateShells();
        checkCollisions();
        checkEndGameConditions();

        if (visualMode)
        {
            displayGame();
        }
    }

    std::cout << "=== Game Summary ===" << std::endl;
    if (!tank1.isAlive() && !tank2.isAlive())
    {
        std::cout << "Tie: both tanks destroyed." << std::endl;
    }
    else if (!tank1.isAlive())
    {
        std::cout << "Player 2 wins!" << std::endl;
    }
    else if (!tank2.isAlive())
    {
        std::cout << "Player 1 wins!" << std::endl;
    }
    else
    {
        std::cout << "Game ended without a winner." << std::endl;
    }
}

void GameManager::applyAction(Tank &tank, ActionType action)
{
    switch (action)
    {
    case ActionType::MOVE_FORWARD:
        std::cout << "MOVE FORWARD: Tank " <<tank.getTankID() << std::endl;
        tank.moveForward();
        break;
    case ActionType::ROTATE_LEFT_1_8:
        std::cout << "ROTATE LEFT 1/8TH " <<tank.getTankID()<<std::endl;
        tank.rotateLeft1_8();
        break;
    case ActionType::ROTATE_RIGHT_1_8:
        std::cout << "ROTATE RIGHT 1/8TH" <<tank.getTankID()<<std::endl;
        tank.rotateRight1_8();
        break;

    case ActionType::ROTATE_LEFT_1_4:
        std::cout << "ROTATE LEFT 1/4TH " <<tank.getTankID()<<std::endl;
        tank.rotateLeft1_4();
        break;
    case ActionType::ROTATE_RIGHT_1_4:
        std::cout << "ROTATE RIGHT 1/4TH" <<tank.getTankID()<<std::endl;
        tank.rotateRight1_4();
        break;
    case ActionType::MOVE_BACKWARD:
        std::cout << "MOVE BACKWARD Tank " <<tank.getTankID()<<std::endl;
        tank.requestBackward();
        break;

    case ActionType::SHOOT:

        if (tank.canShoot())
        {        
            
            std::cout << "TANK "  <<tank.getTankID()<<  " SHOOT" << std::endl;

            tank.shoot();
            Shell newShell(tank.getX(), tank.getY(), tank.getDirection());
            shells.push_back(newShell);
        }

        else         
            std::cout << "TANK "  <<tank.getTankID()<<  " CANNOT SHOOT" << std::endl;


        break;
    default:
                std::cout << "TANK "  <<tank.getTankID()<<  " NO ACTION TAKEN" << std::endl;

        break;
    }
    tank.printStatus();
}

void GameManager::updateShells()
{
    for (auto &shell : shells)
    {
        if (shell.isActive())
            shell.update();
    }
    shells.erase(std::remove_if(shells.begin(), shells.end(),
                                [](const Shell &s)
                                { return !s.isActive(); }),
                 shells.end());
}

void GameManager::checkCollisions()
{
    for (auto &shell : shells)
    {
        if (!shell.isActive())
            continue;
        int sx = shell.getX();
        int sy = shell.getY();
        CellType cell = Global::board->getCellType(sx, sy);
        if (cell == CellType::WALL)
        {
            Global::board->weakenWall(sx, sy);
            shell.deactivate();
            std::cout << "Shell hit a wall at (" << sx << ", " << sy << ") and deactivated." << std::endl;
        }
    }

    for (size_t i = 0; i < shells.size(); ++i)
    {
        for (size_t j = i + 1; j < shells.size(); ++j)
        {
            if (shells[i].isActive() && shells[j].isActive() &&
                shells[i].getX() == shells[j].getX() &&
                shells[i].getY() == shells[j].getY())
            {
                shells[i].deactivate();
                shells[j].deactivate();
                std::cout << "Shell collision at (" << shells[i].getX()
                          << ", " << shells[i].getY() << "). Both shells deactivated."
                          << std::endl;
            }
        }
    }

    for (auto &shell : shells)
    {
        if (!shell.isActive())
            continue;
        int sx = shell.getX();
        int sy = shell.getY();
        if (sx == tank1.getX() && sy == tank1.getY())
        {
            tank1.destroy();
            shell.deactivate();
            std::cout << "Player 1's tank destroyed by a shell at (" << sx << ", " << sy << ")." << std::endl;
        }
        if (sx == tank2.getX() && sy == tank2.getY())
        {
            tank2.destroy();
            shell.deactivate();
            std::cout << "Player 2's tank destroyed by a shell at (" << sx << ", " << sy << ")." << std::endl;
        }
    }

    {
        int tx = tank1.getX();
        int ty = tank1.getY();
        if (Global::board->getCellType(tx, ty) == CellType::MINE)
        {
            tank1.destroy();
            Global::board->setCellType(tx, ty, CellType::EMPTY);
            std::cout << "Player 1's tank hit a mine at (" << tx << ", " << ty << ") and was destroyed." << std::endl;
        }
    }
    {
        int tx = tank2.getX();
        int ty = tank2.getY();
        if (Global::board->getCellType(tx, ty) == CellType::MINE)
        {
            tank2.destroy();
            Global::board->setCellType(tx, ty, CellType::EMPTY);
            std::cout << "Player 2's tank hit a mine at (" << tx << ", " << ty << ") and was destroyed." << std::endl;
        }
    }

    if (tank1.getX() == tank2.getX() && tank1.getY() == tank2.getY())
    {
        tank1.destroy();
        tank2.destroy();
        std::cout << "Tanks collided at (" << tank1.getX() << ", " << tank1.getY() << "). Both destroyed." << std::endl;
    }
}

void GameManager::checkEndGameConditions()
{
    if (!tank1.isAlive() || !tank2.isAlive())
    {
        gameOver = true;
        return;
    }
    if (tank1.getShellCount() == 0 && tank2.getShellCount() == 0)
    {
        stepsSinceBothAmmoZero++;
        if (stepsSinceBothAmmoZero >= 40)
        {
            gameOver = true;
            std::cout << "Tie: 40 steps elapsed with both tanks out of shells." << std::endl;
        }
    }
    else
    {
        stepsSinceBothAmmoZero = 0;
    }
}

void GameManager::displayGame()
{
    for (const auto &shell : shells)
    {
        if (shell.isActive())
        {
            int sx = shell.getX();
            int sy = shell.getY();
            if (sx >= 0 && sx < Global::board->getWidth() && sy >= 0 && sy < Global::board->getHeight())
                Global::board->grid[sy][sx] = CellType::MINE;
        }
    }

    if (visualMode)
    {
        std::cout << "=== Game State (Visual Mode) ===" << std::endl;

        // Render the board with tank directions
        for (const auto &row : Global::board->grid)
        {
            for (const CellType &cell : row)
            {
                if (cell == CellType::TANK1)
                    std::cout << "1 "; // Tank 1 body

                else if (cell == CellType::TANK2)
                    std::cout << "2 "; // Tank 2 body

                else if (cell == CellType::MINE)
                    std::cout << "💣"; // Mines

                else if (cell == CellType::SHELL)
                    std::cout << "💥"; // Shells

                else if (cell == CellType::WALL)
                    std::cout << "🟩"; // Walls

                else
                    std::cout << "⬜"; // Empty spaces
            }
            std::cout << std::endl;
        }

        // Display the cannon direction after the tank body
        for (int i = 0; i < 2; ++i) // Assuming 2 tanks for now
        {
            Tank &tank = (i == 0) ? tank1 : tank2;
            Direction cannonDir = tank.getDirection();

            // Display the tank cannon direction as an arrow
            switch (cannonDir)
            {
            case Direction::U:
                std::cout << "^"; // Up
                break;
            case Direction::UR:
                std::cout << "↗"; // Up-Right
                break;
            case Direction::R:
                std::cout << ">"; // Right
                break;
            case Direction::DR:
                std::cout << "↘"; // Down-Right
                break;
            case Direction::D:
                std::cout << "v"; // Down
                break;
            case Direction::DL:
                std::cout << "↙"; // Down-Left
                break;
            case Direction::L:
                std::cout << "<"; // Left
                break;
            case Direction::UL:
                std::cout << "↖"; // Up-Left
                break;
            }

            std::cout << " Tank " << (i + 1) << " Cannon" << std::endl;
        }
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << std::endl;
}
