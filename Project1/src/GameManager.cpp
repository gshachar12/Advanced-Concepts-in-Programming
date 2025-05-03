#include "GameManager.h"
#include "GameObject.h"
#include "Globals.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <algorithm>

void GameManager::printToBoth(const std::string &message){
    // Print to the console (std::cout)
    std::cout << message << std::endl;

    // Print to the file
    outFile << message << std::endl;
}


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
    //int currentTank = 0;
    outFile.open("output.txt");

    // Check if the file is open
    if (!outFile) {
        std::cerr << "Error opening the file!" << std::endl;
    }

    while (!gameOver && turnCount < 1000)
    {
        turnCount++;
        std::cout << "\033[2J\033[1;1H"; // clear screen

        printToBoth("Turn " + std::to_string(turnCount));

        ActionType action1 = alg1->DecideAction(tank1, tank2, shells);
        ActionType action2 = ActionType::ROTATE_LEFT_1_4; //alg2->DecideAction(tank2, tank1, shells);
        printToBoth("Tank 1 status:");
        applyAction(tank1, action1);
        printToBoth("Tank 2 status:");
        applyAction(tank2, action2);
        for(int i=0; i<2; i++)
        {
        updateShells(); // advance shells twice
        checkEndGameConditions();
        }
        if (visualMode)
        {
            displayGame();
        }


        tank1.printStatus();
        tank2.printStatus();

        for (const auto &shell : shells)
        {
            printToBoth("Shell status:");
            shell.printStatus();

        }

        //currentTank = 1 - currentTank; // switch to the other player
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void GameManager::applyAction( Tank &tank, ActionType action)
{
    if(!tank.isAlive())
        return; 
    std::string actionMessage;  // To store the action message

    switch (action)
    {
    case ActionType::MOVE_FORWARD:
    {
        actionMessage = "MOVE FORWARD: Tank " + std::to_string(tank.getTankID());
        printToBoth(actionMessage);
        auto [new_x, new_y] = tank.moveForward();
        moveTank(new_x, new_y, tank);
    }
    break;
    case ActionType::ROTATE_LEFT_1_8:

        actionMessage = "ROTATE LEFT 1/8TH Tank " + std::to_string(tank.getTankID());
        printToBoth(actionMessage);
        tank.rotateLeft1_8();
        break;  
    case ActionType::ROTATE_RIGHT_1_8:
        actionMessage = "ROTATE RIGHT 1/8TH Tank " + std::to_string(tank.getTankID());
        printToBoth(actionMessage);
        tank.rotateRight1_8();
        break;
    case ActionType::ROTATE_LEFT_1_4:
        actionMessage = "ROTATE LEFT 1/4TH Tank " + std::to_string(tank.getTankID());
        printToBoth(actionMessage);
        tank.rotateLeft1_4();
        break;
    case ActionType::ROTATE_RIGHT_1_4:
        actionMessage = "ROTATE RIGHT 1/4TH Tank " + std::to_string(tank.getTankID());
        printToBoth(actionMessage);
        tank.rotateRight1_4();
        break;
    case ActionType::MOVE_BACKWARD:
        actionMessage = "MOVE BACKWARD Tank Tank " + std::to_string(tank.getTankID());
        printToBoth(actionMessage);
        tank.requestBackward();
        break;
    case ActionType::SHOOT:
        if (tank.canShoot())
        {
            actionMessage = "TANK " + std::to_string(tank.getTankID()) + " SHOOT";
            printToBoth(actionMessage);
            tank.shoot();
            Shell newShell(tank.getX(), tank.getY(), tank.getDirection());
            shells.push_back(newShell);
        }
        else
        {
            actionMessage = "TANK " + std::to_string(tank.getTankID()) + " CANNOT SHOOT";
            printToBoth(actionMessage); 
        }
            break;
    default:
        actionMessage = "TANK " + std::to_string(tank.getTankID()) + " NO ACTION TAKEN";
        printToBoth(actionMessage);

        break;
    }

    if (tank.getBackwardState() == BackwardState::MOVING_BACKWARD) // moving backward approved
    {
        auto [new_x, new_y] = tank.moveBackward();
        moveTank(new_x, new_y, tank);
    }

    actionMessage = "stepped on " + tank.getPosition().toString() + ": "+ cellTypeToString(tank.getObjectType());
    printToBoth(actionMessage);

    tanksCollided(); // check if tanks collided
    tank.update();
}

void GameManager::moveTank(int new_pos_x, int new_pos_y, Tank &tank)
{

    if (!hitWall(new_pos_x, new_pos_y, tank)) // move only if there is no collision
    {

        Global::board->setCellType(tank.getX(), tank.getY(), CellType::EMPTY);
        tank.setPosition(new_pos_x, new_pos_y);
        Global::board->setCellType(new_pos_x, new_pos_y, tank.getObjectType());
    }

}

void GameManager::tanksCollided()
{
    if (tank2.getX() == tank1.getX() && tank1.getY() == tank2.getY())
    {
        // Both tanks are destroyed if they collide
        Global::board->setCellType(tank1.getX(), tank1.getY(), CellType::BOOM);
        printToBoth("Tank 1 collided with Tank 2 at " + tank1.getPosition(). toString()+ ". Both destroyed. RIP.");

        tank1.destroy();
        tank2.destroy();
    }
}

void GameManager::GameSummary()
{
    printToBoth("=== Game Summary ===");

    if (tank1.getPosition() == tank2.getPosition())
    {
        tank1.destroy();
        tank2.destroy();
        printToBoth("Tanks collided at " +tank1.getPosition().toString()+ " Both destroyed.");
    }
    if (!tank1.isAlive() && !tank2.isAlive())
    {
        printToBoth("Tie: both tanks destroyed.");
    }
    else if (!tank1.isAlive())
    {
        printToBoth("Player 2 wins!");
    }
    else if (!tank2.isAlive())
    {
        printToBoth("Player 1 wins!");
    }
    else
    {
        printToBoth("Game ended without a winner.");
    }
}
void GameManager::updateShells()
{
    for (auto &shell : shells)
    {
        if (shell.isActive())
        {
            auto [dx, dy] = shell.advance();
            shell.move(dx, dy);
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
    if (cell == CellType::WALL || cell == CellType::WEAK_WALL)
    {
        Global::board->weakenWall(sx, sy);
        shell.deactivate();
        printToBoth( "Shell hit a wall at "+shell.getPosition().toString()+" and deactivated.");

    }

    // shell hit another shell
    for (size_t i = 0; i < shells.size(); ++i)
    {
        // Avoid self-collision
        if (&shells[i] == &shell)
            continue;

        if (shells[i].isActive() && shell.isActive() &&
            shells[i].getX() == sx &&
            shells[i].getY() == sy)
        {
            shells[i].deactivate();
            shell.deactivate();
            printToBoth( "Shell collision at " + shells[i].getPosition().toString()+
                      ". Both shells deactivated."
                      );
            Global::board->setCellType(sx, sy, CellType::BOOM);

        }
    }

    // shell hit a tank

    if (shell.getPosition()== tank1.getPosition())
    {
        Global::board->setCellType(tank1.getX(), tank1.getY(), CellType::BOOM);
        tank1.destroy();
        shell.deactivate();
        printToBoth("Player 1's tank destroyed by a shell at " +tank1.getPosition().toString()+ "." );
    }
    if (shell.getPosition()== tank2.getPosition())
    {
        Global::board->setCellType(tank2.getX(), tank2.getY(), CellType::BOOM);
        tank2.destroy();
        shell.deactivate();
        printToBoth("Player 2's tank destroyed by a shell at " +tank2.getPosition().toString()+ "." );
    }

    return false;
}

bool GameManager::hitWall(int x, int y, Tank &tank)
{
    CellType cell = Global::board->getCellType(x, y);
    if (cell == CellType::WALL || cell == CellType::WEAK_WALL) // tank hit the wall
    {
        return true;
    }
    if (Global::board->getCellType(x, y) == CellType::MINE)
    {
        tank.destroy();
        tank.setObjectType(CellType::BOOM);
        std::string message = "Tank " + std::to_string(tank.getTankID()) + " stepped on a mine at (" 
                            + std::to_string(x) + ", " + std::to_string(y) 
                            + "). and was destroyed. RIP.";

        // Call printToBoth to print the message to both the console and the file
        printToBoth(message);
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
            printToBoth("Tie: 40 steps elapsed with both tanks out of shells." );
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
                if (shell.getX() == x && shell.getY() == y)
                {
                    if (shell.isActive())
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
        default:
            break;
        }
        std::cout << " Tank " << (i + 1) << " Cannon" << std::endl;
    }

    std::cout << std::endl;
}
