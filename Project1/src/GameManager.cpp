#include "GameManager.h"
#include "GameObject.h"
#include "Globals.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <algorithm>


bool GameManager::initializeGame()
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
    std::vector<Tank *> tanks;
    tanks.push_back(&tank1);
    tanks.push_back(&tank2);
    int currentTank = 0;

    while (!gameOver && turnCount < 1000)
    {
        turnCount++;
        std::cout << "\033[2J\033[1;1H"; // clear screen

        std::cout << "Tank " << currentTank + 1 << " Turn " << turnCount << std::endl;

        ActionType action = alg1->DecideAction(*tanks[currentTank], *tanks[1 - currentTank], shells);

        applyAction(*tanks[currentTank], action);
        updateShells(); // advance shells twice
        updateShells();

        if (visualMode)
        {
            displayGame();
        }
        tank1.printStatus();
        tank2.printStatus();
        for (const auto &shell : shells)
            {
              
                shell.printStatus(); 
                

            }
        
        checkEndGameConditions();
        currentTank = 1 - currentTank; // switch to the other player
        std::this_thread::sleep_for(std::chrono::seconds(1));

    }
}

void GameManager::applyAction(Tank &tank, ActionType action)
{
    switch (action)
    {
    case ActionType::MOVE_FORWARD:
    {
        std::cout << "MOVE FORWARD: Tank " << tank.getTankID() << std::endl;
        auto [dx, dy] = tank.moveForward();
        moveTank(dx, dy, tank);
    }
    break;
    case ActionType::ROTATE_LEFT_1_8:
        std::cout << "ROTATE LEFT 1/8TH Tank " << tank.getTankID() << std::endl;
        tank.rotateLeft1_8();
        break;
    case ActionType::ROTATE_RIGHT_1_8:
        std::cout << "ROTATE RIGHT 1/8TH Tank " << tank.getTankID() << std::endl;
        tank.rotateRight1_8();
        break;
    case ActionType::ROTATE_LEFT_1_4:
        std::cout << "ROTATE LEFT 1/4TH Tank " << tank.getTankID() << std::endl;
        tank.rotateLeft1_4();
        break;
    case ActionType::ROTATE_RIGHT_1_4:
        std::cout << "ROTATE RIGHT 1/4TH Tank " << tank.getTankID() << std::endl;
        tank.rotateRight1_4();
        break;
    case ActionType::MOVE_BACKWARD:
        std::cout << "MOVE BACKWARD Tank Tank " << tank.getTankID() << std::endl;
        tank.requestBackward();
        break;
    case ActionType::SHOOT:
        if (tank.canShoot())
        {
            std::cout << "TANK " << tank.getTankID() << " SHOOT" << std::endl;
            tank.shoot();
            Shell newShell(tank.getX(), tank.getY(), tank.getDirection());
            shells.push_back(newShell);
        }
        else
            std::cout << "TANK " << tank.getTankID() << " CANNOT SHOOT" << std::endl;
        break;
    default:
        std::cout << "TANK " << tank.getTankID() << " NO ACTION TAKEN" << std::endl;

        break;
    }

    if (tank.getBackwardState() == BackwardState::MOVING_BACKWARD) // moving backward approved
    {
        auto [dx, dy] = tank.moveBackward();
        moveTank(dx, dy, tank);
    }
    tank.update();
}

void GameManager::moveTank(int dx, int dy, Tank &tank)
{
    int new_pos_x = (tank.getX() + dx + Global::width) % Global::width;
    int new_pos_y = (tank.getY() + dy + Global::height) % Global::height;
    if (!hitWall(new_pos_x, new_pos_y, tank)) // move only if there is no collision
    {

        Global::board->setCellType(tank.getX(), tank.getY(), CellType::EMPTY);
        tank.setPosition(new_pos_x, new_pos_y);
        Global::board->setCellType(new_pos_x, new_pos_y, tank.getObjectType());
    }
        if (tank2.getX()== tank1.getX() && tank1.getY() == tank2.getY())
    {
        // Both tanks are destroyed if they collide
        Global::board->setCellType(tank1.getX(), tank1.getY(), CellType::BOOM);
        std::cout << "Tank 1 collided with Tank 2 at (" << tank1.getX()<< ", " << tank1.getY()<< "). Both destroyed. RIP." << std::endl;

        tank1.destroy();
        tank2.destroy();

    }
}

void GameManager::GameSummary()
{
    std::cout << "=== Game Summary ===" << std::endl;

    if (tank1.getX() == tank2.getX() && tank1.getY() == tank2.getY())
    {
        tank1.destroy();
        tank2.destroy();
        std::cout << "Tanks collided at (" << tank1.getX() << ", " << tank1.getY() << "). Both destroyed." << std::endl;
    }
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

void GameManager::updateShells()
{
    for (auto &shell : shells)
    {
        if (shell.isActive())
        {
            auto [dx, dy] = shell.advance();
            shell.move(dx,dy);
            ShellHit(shell); 

        }
    }
    shells.erase(std::remove_if(shells.begin(), shells.end(),
                                [](const Shell &s)
                                { return !s.isActive(); }),
                 shells.end());
}

bool GameManager::ShellHit(Shell &shell)
{
    int sx = shell.getX();
    int sy = shell.getY();
    CellType cell = Global::board->getCellType(sx, sy);

    // shell hit wall    
    if (cell == CellType::WALL ||cell == CellType::WEAK_WALL)
    {
        Global::board->weakenWall(sx, sy);
        shell.deactivate();
        std::cout << "Shell hit a wall at (" << sx << ", " << sy << ") and deactivated." << std::endl;
        return true;
    }
    
    // shell hit another shell
    for (size_t i = 0; i < shells.size(); ++i)
    {
        // Avoid self-collision
        if (&shells[i] == &shell) continue;

        if (shells[i].isActive() && shell.isActive() &&
            shells[i].getX() == sx&&
            shells[i].getY() == sy)
        {
            shells[i].deactivate();
            shell.deactivate();
            std::cout << "Shell collision at (" << shells[i].getX()
                        << ", " << shells[i].getY() << "). Both shells deactivated."
                        << std::endl;
        }
        return true; 
    }


    // shell hit a tank  

    if (sx == tank1.getX() && sy == tank1.getY())
    {
        tank1.destroy();
        shell.deactivate();
        std::cout << "Player 1's tank destroyed by a shell at (" << sx << ", " << sy << ")." << std::endl;
        return true; 
    }
    if (sx == tank2.getX() && sy == tank2.getY())
    {
        tank2.destroy();
        shell.deactivate();
        std::cout << "Player 2's tank destroyed by a shell at (" << sx << ", " << sy << ")." << std::endl;
        return true; 
    }

    return false; 
    
}


bool GameManager::hitWall(int x, int y, Tank &tank)
{
    CellType cell = Global::board->getCellType(tank.getX(), tank.getY());

    if (cell == CellType::WALL || cell == CellType::WEAK_WALL) // tank hit the wall
        return true;

    if (Global::board->getCellType(x, y) == CellType::MINE)
    {
        tank.destroy();
        tank.setObjectType(CellType::BOOM);
        std::cout << "Tank " << tank.getTankID() << " stepped on a mine at (" 
                  << x << ", " << y << "). and was destroyed. RIP." << std::endl;
        return true; // added return true here since tank is destroyed
    }

    return false;
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
    std::cout << "=== Game State (Visual Mode) ===" << std::endl;

    for (int y = 0; y < Global::board->getHeight(); ++y)
    {
        for (int x = 0; x < Global::board->getWidth(); ++x)
        {
            // Check if there's an active shell at (x, y)
            bool hasShell = false;
            for (const auto &shell : shells)
            {
                if ( shell.getX() == x && shell.getY() == y)
                {
                    if(shell.isActive())
                    {
                        hasShell = true;
                        break;
                    }

                }

            }

            if (hasShell)
            {
                std::cout << "🚀";
            }
            else
            {
                // Otherwise, display the cell content from the board
                CellType cell = Global::board->grid[y][x];
                switch (cell)
                {
                case CellType::TANK1:
                    std::cout << "1 ";
                    break;
                case CellType::TANK2:
                    std::cout << "2 ";
                    break;
                case CellType::MINE:
                    std::cout << "💣";
                    break;
                case CellType::BOOM:
                    std::cout << "💥";
                    break;
                case CellType::WALL:
                    std::cout << "🟩";
                    break;
                case CellType::WEAK_WALL:
                    std::cout << "🧱";
                    break;
                default:
                    std::cout << "⬜";
                    break;
                }
            }
        }
        std::cout << std::endl;
    }

    // Display the cannon direction for both tanks
    for (int i = 0; i < 2; ++i)
    {
        Tank &tank = (i == 0) ? tank1 : tank2;
        Direction cannonDir = tank.getDirection();
        switch (cannonDir)
        {
        case Direction::U:
            std::cout << "↑";
            break;
        case Direction::UR:
            std::cout << "↗";
            break;
        case Direction::R:
            std::cout << "→";
            break;
        case Direction::DR:
            std::cout << "↘";
            break;
        case Direction::D:
            std::cout << "↓";
            break;
        case Direction::DL:
            std::cout << "↙";
            break;
        case Direction::L:
            std::cout << "←";
            break;
        case Direction::UL:
            std::cout << "↖";
            break;
        }
        std::cout << " Tank " << (i + 1) << " Cannon" << std::endl;
    }

    std::cout << std::endl;
}
