#include "GameManager.h"
#include "BoardSatelliteView.h"
#include "Player.h"
//#include "SecondPlayer.h"
#include "algorithms/Controller.h"
#include "algorithms/Defensive.h"
#include "Shell.h"  // not in common folder, but needed for Shell class
#include "CellType.h" // not in common folder, but needed for CellType enum
#include <algorithm>
#include <unistd.h>
#include <iostream>
#include <termios.h>
#include <chrono>
#include <thread> // For std::this_thread::sleep_for

// Implement MyPlayerFactory
std::unique_ptr<Player> MyPlayerFactory::create(int player_index,
                                                size_t x, size_t y,
                                                size_t max_steps,
                                                size_t num_shells) const
{
    return std::make_unique<MyPlayer>(player_index, x, y, max_steps, num_shells);
}

std::unique_ptr<TankAlgorithm> MyTankAlgorithmFactory::create(
    int player_index, int tank_index) const
{
    if (player_index == 1) {
        return std::make_unique<Defensive>(player_index, tank_index);
    } else {
        // Define a minimal "Shooter" class inside this block
        class Shooter : public TankAlgorithm {
        public:
            ActionRequest getAction() override {
                return ActionRequest::DoNothing;
            }

            void updateBattleInfo(BattleInfo&) override {
                // This simple algorithm ignores battle info
            }
        };

        return std::make_unique<Shooter>();
    }
}


// GameManager constructor
GameManager::GameManager(const PlayerFactory &playerFactory,
                         const TankAlgorithmFactory &algorithmFactory,
                         bool visualMode)
    : currentStep(0),
      gameOver(false),
      visualMode(visualMode),
      noShellsSteps(0),
      playerFactory(dynamic_cast<const MyPlayerFactory &>(playerFactory)),
      algorithmFactory(dynamic_cast<const MyTankAlgorithmFactory &>(algorithmFactory))
{
    // Initialize board with default settings
    board.setWrapAround(true);
    if (visualMode)
    {
        std::cout << "Visual mode enabled. Game will display with emojis." << std::endl;
    }
}

bool GameManager::readBoard(const std::string &filename)
{

    // Use the new board loading method
    if (!board.loadFromFile(filename))
    {
        std::cerr << "Failed to load board from file: " << filename << std::endl;
        return false;
    }

    // Open output file (same name with .out extension)
    std::string outputFilename = filename + ".out";
    outputFile.open(outputFilename);
    if (!outputFile.is_open())
    {
        std::cerr << "Failed to open output file: " << outputFilename << std::endl;
        return false;
    }

    // Initialize players and tanks
    initializePlayers();
    initializeTanks();
    // Check if game is over before it begins (e.g., no tanks for a player)
    checkInitialGameOver();
    return true;
}

void GameManager::initializePlayers()
{
    // Create players using the factories
    player1 = playerFactory.create(1, board.getWidth(), board.getHeight(),
                                   board.getMaxSteps(), board.getNumShellsPerTank());
    player2 = playerFactory.create(2, board.getWidth(), board.getHeight(),
                                   board.getMaxSteps(), board.getNumShellsPerTank());
}

void GameManager::initializeTanks()
{
    // Clear any existing tanks and shells
    tanks.clear();
    shells.clear();
    // Scan the board for tank positions and create tanks
    int player1TankCount = 0;
    int player2TankCount = 0;
    for (int y = 0; y < board.getHeight(); ++y)
    {
        for (int x = 0; x < board.getWidth(); ++x)
        {
            CellType cellType = board.getCellType(x, y);

            if (cellType == CellType::TANK1)
            {
                try
                {
                    // Use our factory to create the algorithm
                    auto algorithm = algorithmFactory.create(1, player1TankCount);

                    Direction startDir = Direction::LEFT; // Player 1 tanks face left

                    auto tank = std::make_unique<Tank>(
                        x, y, startDir, CellType::TANK1,
                        player1TankCount, 1, player1TankCount,
                        board.getNumShellsPerTank(), std::move(algorithm));

                    tanks.push_back(std::move(tank));
                    player1TankCount++;
                }
                catch (const std::exception &e)
                {
                    std::cerr << "Exception creating Tank1: " << e.what() << std::endl;
                }
                catch (...)
                {
                    std::cerr << "Unknown exception creating Tank1" << std::endl;
                }
            }
            else if (cellType == CellType::TANK2)
            {
                try
                {
                    // Use our factory to create the algorithm
                    auto algorithm = algorithmFactory.create(2, player2TankCount);

                    Direction startDir = Direction::RIGHT; // Player 2 tanks face right

                    auto tank = std::make_unique<Tank>(
                        x, y, startDir, CellType::TANK2,
                        player2TankCount, 2, player2TankCount,
                        board.getNumShellsPerTank(), std::move(algorithm));

                    tanks.push_back(std::move(tank));
                    player2TankCount++;
                }
                catch (const std::exception &e)
                {
                    std::cerr << "Exception creating Tank2: " << e.what() << std::endl;
                }
                catch (...)
                {
                    std::cerr << "Unknown exception creating Tank2" << std::endl;
                }
            }
        }
    }
}

void GameManager::checkInitialGameOver()
{
    // Count tanks for each player
    int player1Tanks = 0;
    int player2Tanks = 0;

    for (const auto &tank : tanks)
    {
        if (tank->getObjectType() == CellType::TANK1)
        {
            player1Tanks++;
        }
        else if (tank->getObjectType() == CellType::TANK2)
        {
            player2Tanks++;
        }
    }

    // Check for initial game over conditions
    if (player1Tanks == 0 && player2Tanks == 0)
    {
        gameOver = true;
        gameResultMessage = "Tie, both players have zero tanks";
    }
    else if (player1Tanks == 0)
    {
        gameOver = true;
        gameResultMessage = "Player 2 won with " + std::to_string(player2Tanks) + " tanks still alive";
    }
    else if (player2Tanks == 0)
    {
        gameOver = true;
        gameResultMessage = "Player 1 won with " + std::to_string(player1Tanks) + " tanks still alive";
    }
}

void GameManager::run()
{
    // If game is already over (e.g., from initial conditions), just write the final output

    if (gameOver)
    {
        writeFinalOutput();
        return;
    }


    // Main game loop
    while (!gameOver && currentStep < board.getMaxSteps())
    {
        // Process a game round
        explosions.clear();
        std::cout << "\033[2J\033[1;1H"; // Clear screen
        if (currentStep == 0)
        {
            displayGame(); // Display initial game state
                std::cout << "\033[2J\033[1;1H"; // Clear screen

        }
        processRound();
        currentStep++;
        displayGame();
        // Write this round's output
        writeOutputLine();
        // Clean up dead objects
        cleanupDeadObjects();
        // Check if the game is over
        if (isGameOver())
        {
            writeFinalOutput();
            break;
        }
    }

    // If we reached max steps and game isn't over, determine the result
    if (!gameOver && currentStep >= board.getMaxSteps())
    {
        gameOver = true;
        // Count tanks for each player
        int player1Tanks = 0;
        int player2Tanks = 0;

        for (const auto &tank : tanks)
        {
            if (tank->getObjectType() == CellType::TANK1 && tank->isAlive())
            {
                player1Tanks++;
            }
            else if (tank->getObjectType() == CellType::TANK2 && tank->isAlive())
            {
                player2Tanks++;
            }
        }

        gameResultMessage = "Tie, reached max steps = " + std::to_string(board.getMaxSteps()) +
                            ", player 1 has " + std::to_string(player1Tanks) +
                            " tanks, player 2 has " + std::to_string(player2Tanks) + " tanks";

        writeFinalOutput();
    }
}

void GameManager::processRound()
{
    // Reset tank round state
    for (auto &tank : tanks)
    {
        if (tank->isAlive())
        {
            tank->resetRoundState();
        }
    }

    // Process each tank's action
    for (auto &tank : tanks)
    {
        if (tank->isAlive())
        {
            processTankAction(*tank);
        }
    }

    // Update all shells 

    for (int i = 0; i < 2; i++) // shell updates twice
    {
        for (int j=0;j<shells.size();j++)
        {
            auto &shell = shells[j];

            if (shell->getObjectType() == CellType::SHELL)
            {
                Shell *shellPtr = dynamic_cast<Shell *>(shell.get());
                if (shellPtr)
                {
                    shellPtr->update(board, tanks, explosions);
                    // Handle collisions between objects
                    handleShellCollisions(shellPtr);
                }
            }
        }
    }
    shells.erase(
        std::remove_if(shells.begin(), shells.end(),
                       [](const std::unique_ptr<GameObject> &shell)
                       {
                           Shell *shellPtr = dynamic_cast<Shell *>(shell.get());
                           return shellPtr && shellPtr->hasExploded();
                       }),
        shells.end());


    // Update all tanks (cooldowns, etc.)
    for (auto& tank : tanks)
    {
        // Only proceed if the tank is alive
        if (tank->isAlive())
        {
            // Create satellite view
            BoardSatelliteView view(board, tanks, shells, *tank);

            // Route to the appropriate player
            if (tank->getPlayerIndex() == 1 && player1)
            {

                player1->updateTankWithBattleInfo(*(tank->getAlgorithm()), view);
            }
            else if (tank->getPlayerIndex() == 2 && player2)
            {
                player2->updateTankWithBattleInfo(*(tank->getAlgorithm()), view);
            }
        }

        tank->update();
    }

    // Check for the no-shells condition
    bool anyTankHasShells = false;
    for (const auto &tank : tanks)
    {
        if (tank->isAlive() && tank->getShellCount() > 0)
        {
            anyTankHasShells = true;
            break;
        }
    }

    if (!anyTankHasShells)
    {
        noShellsSteps++;
        if (noShellsSteps >= MAX_NO_SHELLS_STEPS)
        {
            gameOver = true;
            gameResultMessage = "Tie, both players have zero shells for " +
                                std::to_string(MAX_NO_SHELLS_STEPS) + " steps";
        }
    }
    else
    {
        noShellsSteps = 0; // Reset counter if any tank has shells
    }
}

void GameManager::processTankAction(Tank &tank)
{
    // Get the action from the tank's algorithm
    // Get battle info and update the tank's algorithm with current game state

    ActionRequest action = tank.getAction();
    std::string actionName;
    bool actionIgnored = false;

    // Process the action
    switch (action)
    {
    case ActionRequest::MoveForward:
    {
        actionName = "MoveForward";
        auto [newX, newY] = tank.moveForward(board);
        if (newX == tank.getX() && newY == tank.getY() || isTankAboutToHitWall(newX, newY))
        {
            // Movement blocked
            actionIgnored = true;
        }
        else
        {
            // Update board
            board.setCellType(tank.getX(), tank.getY(), CellType::EMPTY);
            tank.setPosition(newX, newY);
            if (!checkTankOnMine(tank))
            {
                board.setCellType(newX, newY, tank.getObjectType());
            }
        }
        break;
    }

    case ActionRequest::MoveBackward:
    {
        actionName = "MoveBackward";
        tank.requestBackward();
        if (tank.getBackwardState() != BackwardState::MOVING_BACKWARD)
        {
            // Just requested, not moving yet
            actionIgnored = true;
        }
        else
        {
            auto [newX, newY] = tank.moveBackward(board);
            if (isTankAboutToHitWall(newX, newY))
            {
                actionIgnored = true;
                break;
            }
            if (newX == tank.getX() && newY == tank.getY())
            {
                // Movement blocked
                actionIgnored = true;
            }
            else
            {
                // Update board
                if (!checkTankOnMine(tank))
                {
                    board.setCellType(tank.getX(), tank.getY(), CellType::EMPTY);
                    tank.setPosition(newX, newY);
                    if (!checkTankOnMine(tank))
                    {
                        board.setCellType(newX, newY, tank.getObjectType());
                    }
                }
            }
        }
        break;
    }

    case ActionRequest::RotateLeft90:
    {
        actionName = "RotateLeft90";
        tank.rotateLeft90();
        break;
    }

    case ActionRequest::RotateRight90:
    {
        actionName = "RotateRight90";
        tank.rotateRight90();
        break;
    }

    case ActionRequest::RotateLeft45:
    {
        actionName = "RotateLeft45";
        tank.rotateLeft45();
        break;
    }

    case ActionRequest::RotateRight45:
    {
        actionName = "RotateRight45";
        tank.rotateRight45();
        break;
    }

    case ActionRequest::Shoot:
    {
        actionName = "Shoot";
        if (!tank.canShoot())
        {
            actionIgnored = true;
        }
        else
        {
            tank.shoot();
            // Get the direction offset to position the shell
            auto [dx, dy] = Directions::directionToOffset(tank.getDirection());

            // Create shell position in front of tank
            int shellX = tank.getX();
            int shellY = tank.getY();

            // Create and add the shell
            auto shell = std::make_unique<Shell>(
                shellX, shellY,
                tank.getDirection(),
                tank.getPlayerIndex());

            board.setCellType(shellX, shellY, CellType::SHELL);
            shells.push_back(std::move(shell));
        }
        break;
    }

    case ActionRequest::GetBattleInfo:
    {
        actionName = "GetBattleInfo";

        // Create a satellite view for this tank
        BoardSatelliteView satelliteView(board, tanks, shells, tank);
        // Route the request to the appropriate player based on the tank's player index
        if (tank.getPlayerIndex() == 1 && player1)
        {
            player1->updateTankWithBattleInfo(*(tank.getAlgorithm()), satelliteView);
        }
        else if (tank.getPlayerIndex() == 2 && player2)
        {
            player2->updateTankWithBattleInfo(*(tank.getAlgorithm()), satelliteView);
        }
        else
        {
            actionIgnored = true;
        }
        break;

        // Print the current battle info for debugging
 
    }

    case ActionRequest::DoNothing:
    {
        actionName = "DoNothing";
        // Do nothing
        break;
    }
    }

    // Print the action name to screen
    std::cout << "Tank " << tank.getPlayerIndex() << " took action: " << actionName << std::endl;
    // Print the tank's action and whether it was ignored

    // check collisions
    isTankCollidingWithOthers(tank, board);
}

bool GameManager::isTankCollidingWithOthers(const Tank &tank, Board &board)
{
    // Check if this tank's position overlaps with any other tank
    for (const auto &otherTank : tanks)
    {
        // Skip if same tank or either tank is dead
        if (&tank == otherTank.get() || !tank.isAlive() || !otherTank->isAlive())
        {
            continue;
        }
        // Check for position overlap
        if (tank.getX() == otherTank->getX() && tank.getY() == otherTank->getY())
        {
            // Both tanks are destroyed when they collide
            const_cast<Tank &>(tank).destroy();
            otherTank->destroy();
            std::cout << "Collision detected between Tank "
                      << const_cast<Tank &>(tank).getPlayerIndex() << ") and Tank "
                      << otherTank->getPlayerIndex()
                      << ") at (" << const_cast<Tank &>(tank).getX() << ", " << const_cast<Tank &>(tank).getY() << ")\n";

            // Mark explosion on board
            board.setCellType(tank.getX(), tank.getY(), CellType::BOOM);
            return true;
        }
    }
// Check if tank has stepped on a shell
for (const auto& shell : shells) {
    if (shell->getObjectType() == CellType::SHELL && 
        shell->getX() == tank.getX() && 
        shell->getY() == tank.getY() && 
        tank.getShootCooldown() != SHOOT_COOLDOWN)
    {
        Shell* shellPtr = dynamic_cast<Shell*>(shell.get());
        if (shellPtr) {
            // Explode the shell at the tank's position
            shellPtr->explode(tank.getX(), tank.getY(), explosions);

            // Destroy the tank
            const_cast<Tank&>(tank).destroy();

            return true;
        }
    }
}

    return false;
}

bool GameManager::isTankAboutToHitWall(const int nextX, int nextY) const
{
    // Check if the position is within bounds
    if (nextX < 0 || nextX >= board.getWidth() || nextY < 0 || nextY >= board.getHeight())
    {
        return false; // Out of bounds, not hitting a wall
    }

    // Check the cell type at the next position
    CellType nextCellType = board.getCellType(nextX, nextY);

    return (nextCellType == CellType::WALL || nextCellType == CellType::WEAK_WALL);
}

bool GameManager::checkTankOnMine(Tank &tank)
{
    if (tank.isAlive())
    {
        int x = tank.getX();
        int y = tank.getY();

        // Check if the tank is on a mine
        if (board.getCellType(x, y) == CellType::MINE)
        {
            // Destroy the tank
            tank.destroy();
            board.setCellType(x, y, CellType::BOOM); // Mark explosion on the board
            std::cout << "Tank " << tank.getTankID() << " of Player "
                      << tank.getPlayerIndex() << " was destroyed by a mine at ("
                      << x << ", " << y << ")" << std::endl;
            return true;
        }

        else
            return false;
    }
    return false;
}
void GameManager::handleShellCollisions(Shell* shellPtr)
{
        for (const auto& tank : tanks) {
            if (!tank->isAlive()) continue;
            if (shellPtr->getX() == tank->getX() && shellPtr->getY() == tank->getY()) {
                // Destroy both shell and tank

            
                shellPtr->explode(explosions);
                tank->destroy();
                board.setCellType(tank->getX(), tank->getY(), CellType::BOOM);
            }
        }
    
    // Check for shell collisions with other shells
    for (size_t i = 0; i < shells.size(); ++i)
    {

        Shell *shellPtr1 = dynamic_cast<Shell *>(shells[i].get());
        if (!shellPtr1)
            continue; // Skip if cast fails

        for (size_t j = i + 1; j < shells.size(); ++j)
        {

            if (shells[j]->getObjectType() == CellType::SHELL)
            {
                Shell *shellPtr2 = dynamic_cast<Shell *>(shells[j].get());
                if (shellPtr1->collidesWith(*shellPtr2))
                {
                    // Shells collide, both explode
                    shellPtr1->explode(explosions);
                    shellPtr2->explode(explosions);
                }
            }
        }
    }
}

bool GameManager::isGameOver()
{
    // Count tanks for each player
    int player1Tanks = 0;
    int player2Tanks = 0;

    for (const auto &tank : tanks)
    {
        if (tank->isAlive())
        {
            if (tank->getObjectType() == CellType::TANK1)
            {
                player1Tanks++;
            }
            else if (tank->getObjectType() == CellType::TANK2)
            {
                player2Tanks++;
            }
        }
    }

    // Check if a player has no tanks left
    if (player1Tanks == 0 && player2Tanks == 0)
    {
        gameOver = true;
        gameResultMessage = "Tie, both players have zero tanks";
        return true;
    }
    else if (player1Tanks == 0)
    {
        gameOver = true;
        gameResultMessage = "Player 2 won with " + std::to_string(player2Tanks) + " tanks still alive";
        return true;
    }
    else if (player2Tanks == 0)
    {
        gameOver = true;
        gameResultMessage = "Player 1 won with " + std::to_string(player1Tanks) + " tanks still alive";
        return true;
    }

    return false;
}

void GameManager::writeOutputLine()
{
    // Write the actions of each tank to the output file
    if (!outputFile.is_open())
    {
        std::cout << "Output file is not open, cannot write actions." << std::endl;
        return;
    }
    bool first = true;
    for (const auto &tank : tanks)
    {
        if (!first)
        {
            outputFile << ", ";
        }
        outputFile << tank->getActionString();
        first = false;
    }
    outputFile << std::endl;
}

void GameManager::writeFinalOutput()
{
    // Write the final game result to the output file
    if (!outputFile.is_open())
        return;

    outputFile << gameResultMessage << std::endl;
}

void GameManager::cleanupDeadObjects()
{
    // Remove dead shells
    auto shellIt = std::remove_if(shells.begin(), shells.end(),
                                  [](const auto &shell)
                                  {
                                      // Remove if the shell is not SHELL type (e.g., turned into BOOM)
                                      return shell->getObjectType() != CellType::SHELL;
                                  });

    shells.erase(shellIt, shells.end());

    // We don't remove dead tanks since we need them for output formatting
}

void waitForKeyPress()
{
    std::cout << "\nPress any key to get the next step\n"<< std::endl;

    termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt); // save old settings
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);        // disable buffering and echo
    tcsetattr(STDIN_FILENO, TCSANOW, &newt); // apply new settings

    getchar(); // wait for key

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // restore old settings
}
void GameManager::displayGame()
{
    if (!visualMode)
        return;

    std::cout << "=== Game State (Visual Mode) - Turn: " << currentStep << " ===\n" << std::endl;

    initVisualBoard();
    overlayShells();
    overlayTanks();
    printBoard();
    printTankStatus();
    printShellStatus();
    printGameSummary();
    waitForVisualPause();
}

void GameManager::initVisualBoard()
{
    visualBoard = std::vector<std::vector<std::string>>(
        board.getHeight(), std::vector<std::string>(board.getWidth(), "⬜"));

    for (int y = 0; y < board.getHeight(); ++y)
    {
        for (int x = 0; x < board.getWidth(); ++x)
        {
            switch (board.getCellType(x, y))
            {
            case CellType::WALL:
                visualBoard[y][x] = "🟩";
                break;
            case CellType::WEAK_WALL:
                visualBoard[y][x] = "🟨";
                break;
            case CellType::MINE:
                visualBoard[y][x] = "💣";
                break;
            case CellType::BOOM:
                visualBoard[y][x] = "💥";
                break;
            default:
                visualBoard[y][x] = "⬜";
                break;
            }
        }
    }

    for (const auto &[x, y] : explosions)
    {
        if (x >= 0 && x < board.getWidth() && y >= 0 && y < board.getHeight())
        {
            visualBoard[y][x] = "💥";
        }
    }
}

void GameManager::overlayShells()
{
    for (const auto &shell : shells)
    {
        if (shell->getObjectType() == CellType::SHELL)
        {
            visualBoard[shell->getY()][shell->getX()] = "🚀";
        }
    }
}

void GameManager::overlayTanks()
{// Create a mapping of directions to symbols
    static const std::map<Direction, std::string> dirSymbols{
        {Direction::UP, "↑"}, {Direction::UP_RIGHT, "↗"}, {Direction::RIGHT, "→"}, {Direction::DOWN_RIGHT, "↘"}, {Direction::DOWN, "↓"}, {Direction::DOWN_LEFT, "↙"}, {Direction::LEFT, "←"}, {Direction::UP_LEFT, "↖"}};

    for (const auto &tank : tanks)
    {
        if (!tank->isAlive())
            continue;

        int x = tank->getX(), y = tank->getY();
        if (x >= 0 && x < board.getWidth() && y >= 0 && y < board.getHeight())
        {
            Direction dir = tank->getDirection();
            std::string arrow = dirSymbols.count(dir) ? dirSymbols.at(dir) : "?";

            std::string playerNum = std::to_string(tank->getPlayerIndex());
            visualBoard[y][x] = arrow + playerNum;
        }
    }
}

void GameManager::printBoard()
{
    for (const auto &row : visualBoard)
    {
        for (const auto &cell : row)
            std::cout << cell;
        std::cout << '\n';
    }
}

void GameManager::printTankStatus()
{
    std::cout << "\nTank Status:\n";
    for (const auto &tank : tanks)
    {
        if (!tank->isAlive())
        {
            int idx = tank->getObjectType() == CellType::TANK1 ? 1 : 2;
            std::cout << "✖️ Tank " << idx << " (ID: " << tank->getTankID() << ") DESTROYED\n";
            continue;
        }

        static const std::map<Direction, std::string> dirSymbols{
            {Direction::UP, "↑"}, {Direction::UP_RIGHT, "↗"}, {Direction::RIGHT, "→"}, {Direction::DOWN_RIGHT, "↘"}, {Direction::DOWN, "↓"}, {Direction::DOWN_LEFT, "↙"}, {Direction::LEFT, "←"}, {Direction::UP_LEFT, "↖"}};

        Direction dir = tank->getDirection();
        std::string symbol = dirSymbols.count(dir) ? dirSymbols.at(dir) : "?";

        std::cout << symbol << " Tank " << tank->getPlayerIndex()
                  << " (ID: " << tank->getTankID() << ") at (" << tank->getX() << ", " << tank->getY()
                  << "), Direction: " << symbol
                  << ", Shells: " << tank->getShellCount()
                  << ", Cooldown: " << tank->getShootCooldown() << "\n";
    }
}

void GameManager::printShellStatus()
{
    if (shells.empty())
        return;

    std::cout << "\nShell Status:\n";

    static const std::map<Direction, std::string> dirSymbols{
        {Direction::UP, "↑"}, {Direction::UP_RIGHT, "↗"}, {Direction::RIGHT, "→"}, {Direction::DOWN_RIGHT, "↘"}, {Direction::DOWN, "↓"}, {Direction::DOWN_LEFT, "↙"}, {Direction::LEFT, "←"}, {Direction::UP_LEFT, "↖"}};

    for (const auto &shell : shells)
    {
        if (shell->getObjectType() != CellType::SHELL)
            continue;

        Direction dir = shell->getDirection();
        std::string symbol = dirSymbols.count(dir) ? dirSymbols.at(dir) : "?";

        std::cout << "🚀 Shell from Player " << dynamic_cast<Shell *>(shell.get())->getOwnerPlayerIndex()
                  << " at (" << shell->getX() << ", " << shell->getY()
                  << "), Direction: " << symbol << "\n";
    }
}

void GameManager::printGameSummary()
{
    int p1Tanks = 0, p2Tanks = 0, p1Shells = 0, p2Shells = 0;

    for (const auto &tank : tanks)
    {
        if (!tank->isAlive())
            continue;

        if (tank->getObjectType() == CellType::TANK1)
        {
            p1Tanks++;
            p1Shells += tank->getShellCount();
        }
        else if (tank->getObjectType() == CellType::TANK2)
        {
            p2Tanks++;
            p2Shells += tank->getShellCount();
        }
    }

    std::cout << "\nGame Summary:\n";
    std::cout << "Player 1 Artillery: " << p1Tanks << " tanks, " << p1Shells << " shells\n";
    std::cout << "Player 2 Artillery: " << p2Tanks << " tanks, " << p2Shells << " shells\n";

    if (noShellsSteps > 0)
        std::cout << "Steps without shells: " << noShellsSteps << "/" << MAX_NO_SHELLS_STEPS << "\n";

    if (gameOver)
        std::cout << "\n🏁 GAME OVER: " << gameResultMessage << "\n";
}

void GameManager::waitForVisualPause()
{
    waitForKeyPress(); // Custom function to pause between turns
}
