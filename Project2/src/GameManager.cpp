#include "GameManager.h"
#include "BoardSatelliteView.h"
#include "MyPlayer.h"
#include "algorithms/Controller.h"
#include "algorithms/Defensive.h"
#include "Shell.h"
#include <algorithm>
#include <iostream>
#include <chrono>
#include <thread>  // For std::this_thread::sleep_for

// Implement MyPlayerFactory
std::unique_ptr<Player> MyPlayerFactory::create(int player_index, 
                                             size_t x, size_t y,
                                             size_t max_steps, 
                                             size_t num_shells) const {
    return std::make_unique<MyPlayer>(player_index, x, y, max_steps, num_shells);
}

// Implement MyTankAlgorithmFactory with a simplified approach to avoid segmentation faults
std::unique_ptr<TankAlgorithm> MyTankAlgorithmFactory::create(
    int player_index, int tank_index) const {
    
    std::cout << "DEBUG: Creating simplified algorithm for player " << player_index 
              << ", tank " << tank_index << std::endl;
    
    // Create a simple algorithm class defined inline that doesn't rely on problematic code
    class SimpleAlgorithm : public TankAlgorithm {
    private:
        int counter = 0;
        int playerIndex;
    public:
        SimpleAlgorithm(int player) : playerIndex(player) {}
        
        ActionRequest getAction() override {
            // Create a simple sequence of actions
            counter++;
            switch (counter % 10) {
                case 1: return ActionRequest::MoveForward;
                case 2: return ActionRequest::RotateRight45;
                case 3: return ActionRequest::MoveForward;
                case 4: return ActionRequest::Shoot;
                case 5: return ActionRequest::RotateLeft90;
                case 6: return ActionRequest::MoveForward;
                case 7: return ActionRequest::MoveForward;
                case 8: return ActionRequest::RotateRight45;
                case 9: return ActionRequest::Shoot;
                case 0: return ActionRequest::RotateLeft45;
                default: return ActionRequest::DoNothing;
            }
        }
        
        void updateBattleInfo(BattleInfo& info) override {
            // Simple stub implementation that does nothing
        }
    };
    
    return std::make_unique<SimpleAlgorithm>(player_index);
}

// GameManager constructor
GameManager::GameManager(const PlayerFactory& playerFactory, 
                         const TankAlgorithmFactory& algorithmFactory,
                         bool visualMode)
    : currentStep(0), 
      gameOver(false),
      visualMode(visualMode),
      noShellsSteps(0),
      playerFactory(dynamic_cast<const MyPlayerFactory&>(playerFactory)),
      algorithmFactory(dynamic_cast<const MyTankAlgorithmFactory&>(algorithmFactory)) {
    
    // Initialize board with default settings
    board.setWrapAround(true);
    
    if (visualMode) {
        std::cout << "Visual mode enabled. Game will display with emojis." << std::endl;
    }
}

bool GameManager::readBoard(const std::string& filename) {
    std::cout << "DEBUG: Starting readBoard for file: " << filename << std::endl;
    
    // Use the new board loading method
    if (!board.loadFromFile(filename)) {
        std::cerr << "Failed to load board from file: " << filename << std::endl;
        return false;
    }
    
    std::cout << "DEBUG: Board loaded successfully" << std::endl;
    
    // Open output file (same name with .out extension)
    std::string outputFilename = filename + ".out";
    std::cout << "DEBUG: Opening output file: " << outputFilename << std::endl;
    
    outputFile.open(outputFilename);
    if (!outputFile.is_open()) {
        std::cerr << "Failed to open output file: " << outputFilename << std::endl;
        return false;
    }
    
    std::cout << "DEBUG: Output file opened successfully" << std::endl;
    
    // Initialize players and tanks
    std::cout << "DEBUG: Initializing players" << std::endl;
    initializePlayers();
    
    std::cout << "DEBUG: Initializing tanks" << std::endl;
    initializeTanks();
    
    std::cout << "DEBUG: Checking for initial game over condition" << std::endl;
    // Check if game is over before it begins (e.g., no tanks for a player)
    checkInitialGameOver();
    
    std::cout << "DEBUG: readBoard completed successfully" << std::endl;
    return true;
}

void GameManager::initializePlayers() {
    // Create players using the factories
    player1 = playerFactory.create(1, board.getWidth(), board.getHeight(), 
                                 board.getMaxSteps(), board.getNumShellsPerTank());
    player2 = playerFactory.create(2, board.getWidth(), board.getHeight(), 
                                 board.getMaxSteps(), board.getNumShellsPerTank());
}

void GameManager::initializeTanks() {
    std::cout << "DEBUG: initializeTanks - Start" << std::endl;
    
    // Clear any existing tanks and shells
    tanks.clear();
    shells.clear();
    
    // Scan the board for tank positions and create tanks
    int player1TankCount = 0;
    int player2TankCount = 0;
    
    std::cout << "DEBUG: Board dimensions: " << board.getWidth() << "x" << board.getHeight() << std::endl;
    
    for (int y = 0; y < board.getHeight(); ++y) {
        for (int x = 0; x < board.getWidth(); ++x) {
            CellType cellType = board.getCellType(x, y);
            
            if (cellType == CellType::TANK1) {
                std::cout << "DEBUG: Found Tank1 at (" << x << "," << y << ")" << std::endl;
                try {
                    // Use our factory to create the algorithm
                    auto algorithm = algorithmFactory.create(1, player1TankCount);
                    
                    Direction startDir = Direction::LEFT; // Player 1 tanks face left
                    
                    std::cout << "DEBUG: Creating Tank1 object" << std::endl;
                    auto tank = std::make_unique<Tank>(
                        x, y, startDir, CellType::TANK1, 
                        player1TankCount, 1, player1TankCount,
                        board.getNumShellsPerTank(), std::move(algorithm)
                    );
                    
                    std::cout << "DEBUG: Adding Tank1 to tanks vector" << std::endl;
                    tanks.push_back(std::move(tank));
                    player1TankCount++;
                } catch (const std::exception& e) {
                    std::cerr << "Exception creating Tank1: " << e.what() << std::endl;
                } catch (...) {
                    std::cerr << "Unknown exception creating Tank1" << std::endl;
                }
            }
            else if (cellType == CellType::TANK2) {
                std::cout << "DEBUG: Found Tank2 at (" << x << "," << y << ")" << std::endl;
                try {
                    // Use our factory to create the algorithm
                    auto algorithm = algorithmFactory.create(2, player2TankCount);
                    
                    Direction startDir = Direction::RIGHT; // Player 2 tanks face right
                    
                    std::cout << "DEBUG: Creating Tank2 object" << std::endl;
                    auto tank = std::make_unique<Tank>(
                        x, y, startDir, CellType::TANK2, 
                        player2TankCount, 2, player2TankCount,
                        board.getNumShellsPerTank(), std::move(algorithm)
                    );
                    
                    std::cout << "DEBUG: Adding Tank2 to tanks vector" << std::endl;
                    tanks.push_back(std::move(tank));
                    player2TankCount++;
                } catch (const std::exception& e) {
                    std::cerr << "Exception creating Tank2: " << e.what() << std::endl;
                } catch (...) {
                    std::cerr << "Unknown exception creating Tank2" << std::endl;
                }
            }
        }
    }
    
    std::cout << "DEBUG: initializeTanks - Complete. Created " << player1TankCount << " player 1 tanks and " << player2TankCount << " player 2 tanks" << std::endl;
}

void GameManager::checkInitialGameOver() {
    // Count tanks for each player
    int player1Tanks = 0;
    int player2Tanks = 0;
    
    for (const auto& tank : tanks) {
        if (tank->getObjectType() == CellType::TANK1) {
            player1Tanks++;
        } else if (tank->getObjectType() == CellType::TANK2) {
            player2Tanks++;
        }
    }
    
    // Check for initial game over conditions
    if (player1Tanks == 0 && player2Tanks == 0) {
        gameOver = true;
        gameResultMessage = "Tie, both players have zero tanks";
    } else if (player1Tanks == 0) {
        gameOver = true;
        gameResultMessage = "Player 2 won with " + std::to_string(player2Tanks) + " tanks still alive";
    } else if (player2Tanks == 0) {
        gameOver = true;
        gameResultMessage = "Player 1 won with " + std::to_string(player1Tanks) + " tanks still alive";
    }
}

void GameManager::run() {
    // If game is already over (e.g., from initial conditions), just write the final output
    if (gameOver) {
        displayGame(); // Display initial state
        writeFinalOutput();
        return;
    }
    
    // Display initial state of the game
    displayGame();
    
    // Main game loop
    while (!gameOver && currentStep < board.getMaxSteps()) {
        // Process a game round
        processRound();
        currentStep++;
        
        // Display the game state after this round
        displayGame();
        
        // Check if the game is over
        if (isGameOver()) {
            writeFinalOutput();
            break;
        }
        
        // Write this round's output
        writeOutputLine();
        
        // Clean up dead objects
        cleanupDeadObjects();
    }
    
    // If we reached max steps and game isn't over, determine the result
    if (!gameOver && currentStep >= board.getMaxSteps()) {
        gameOver = true;
        
        // Count tanks for each player
        int player1Tanks = 0;
        int player2Tanks = 0;
        
        for (const auto& tank : tanks) {
            if (tank->getObjectType() == CellType::TANK1 && tank->isAlive()) {
                player1Tanks++;
            } else if (tank->getObjectType() == CellType::TANK2 && tank->isAlive()) {
                player2Tanks++;
            }
        }
        
        gameResultMessage = "Tie, reached max steps = " + std::to_string(board.getMaxSteps()) + 
                         ", player 1 has " + std::to_string(player1Tanks) + 
                         " tanks, player 2 has " + std::to_string(player2Tanks) + " tanks";
        
        // Display final state
        displayGame();
        writeFinalOutput();
    }
}

void GameManager::processRound() {
    // Reset tank round state
    for (auto& tank : tanks) {
        if (tank->isAlive()) {
            tank->resetRoundState();
        }
    }
    
    // Process each tank's action
    for (auto& tank : tanks) {
        if (tank->isAlive()) {
            processTankAction(*tank);
        }
    }
    
    // Update all shells (make them move)
    for (auto& shell : shells) {
        if (shell->getObjectType() == CellType::SHELL) {
            Shell* shellPtr = dynamic_cast<Shell*>(shell.get());
            if (shellPtr) {
                shellPtr->update(board);
            }
        }
    }
    
    // Handle collisions between objects
    handleCollisions();
    
    // Update all tanks (cooldowns, etc.)
    for (auto& tank : tanks) {
        tank->update();
    }
    
    // Check for the no-shells condition
    bool anyTankHasShells = false;
    for (const auto& tank : tanks) {
        if (tank->isAlive() && tank->getShellCount() > 0) {
            anyTankHasShells = true;
            break;
        }
    }
    
    if (!anyTankHasShells) {
        noShellsSteps++;
        if (noShellsSteps >= MAX_NO_SHELLS_STEPS) {
            gameOver = true;
            gameResultMessage = "Tie, both players have zero shells for " + 
                               std::to_string(MAX_NO_SHELLS_STEPS) + " steps";
        }
    } else {
        noShellsSteps = 0;  // Reset counter if any tank has shells
    }
}

void GameManager::processTankAction(Tank& tank) {
    // Get the action from the tank's algorithm
    ActionRequest action = tank.getAction();
    std::string actionName;
    bool actionIgnored = false;
    
    // Process the action
    switch (action) {
        case ActionRequest::MoveForward: {
            actionName = "MoveForward";
            auto [newX, newY] = tank.moveForward(board);
            if (newX == tank.getX() && newY == tank.getY()) {
                // Movement blocked
                actionIgnored = true;
            } else {
                // Update board
                board.setCellType(tank.getX(), tank.getY(), CellType::EMPTY);
                board.setCellType(newX, newY, tank.getObjectType());
                tank.setPosition(newX, newY);
            }
            break;
        }
        
        case ActionRequest::MoveBackward: {
            actionName = "MoveBackward";
            tank.requestBackward();
            if (tank.getBackwardState() != BackwardState::MOVING_BACKWARD) {
                // Just requested, not moving yet
                actionIgnored = true;
            } else {
                auto [newX, newY] = tank.moveBackward(board);
                if (newX == tank.getX() && newY == tank.getY()) {
                    // Movement blocked
                    actionIgnored = true;
                } else {
                    // Update board
                    board.setCellType(tank.getX(), tank.getY(), CellType::EMPTY);
                    board.setCellType(newX, newY, tank.getObjectType());
                    tank.setPosition(newX, newY);
                }
            }
            break;
        }
        
        case ActionRequest::RotateLeft90: {
            actionName = "RotateLeft90";
            tank.rotateLeft90();
            break;
        }
        
        case ActionRequest::RotateRight90: {
            actionName = "RotateRight90";
            tank.rotateRight90();
            break;
        }
        
        case ActionRequest::RotateLeft45: {
            actionName = "RotateLeft45";
            tank.rotateLeft45();
            break;
        }
        
        case ActionRequest::RotateRight45: {
            actionName = "RotateRight45";
            tank.rotateRight45();
            break;
        }
        
        case ActionRequest::Shoot: {
            actionName = "Shoot";
            if (!tank.canShoot()) {
                actionIgnored = true;
            } else {
                tank.shoot();
                
                // Get the direction offset to position the shell
                auto [dx, dy] = Directions::directionToOffset(tank.getDirection());
                
                // Create shell position in front of tank
                int shellX = tank.getX() + dx;
                int shellY = tank.getY() + dy;
                
                // Check if shell position is valid (not out of bounds or a wall)
                CellType targetCell = board.getCellType(shellX, shellY);
                if (targetCell == CellType::EMPTY) {
                    // Create and add the shell
                    auto shell = std::make_unique<Shell>(
                        shellX, shellY, 
                        tank.getDirection(), 
                        tank.getPlayerIndex()
                    );
                    
                    board.setCellType(shellX, shellY, CellType::SHELL);
                    shells.push_back(std::move(shell));
                } else {
                    // Shell position blocked, it explodes immediately
                    std::cout << "Shell blocked and exploded immediately at (" 
                              << shellX << ", " << shellY << ")" << std::endl;
                    
                    if (targetCell == CellType::WALL || targetCell == CellType::WEAK_WALL) {
                        board.weakenWall(shellX, shellY);
                    }
                }
            }
            break;
        }
        
        case ActionRequest::GetBattleInfo: {
            actionName = "GetBattleInfo";
            
            // Create a satellite view for this tank
            BoardSatelliteView satelliteView(board, tanks, shells, tank);
            
            // Route the request to the appropriate player based on the tank's player index
            if (tank.getPlayerIndex() == 1 && player1) {
                player1->updateTankWithBattleInfo(*(tank.getAlgorithm()), satelliteView);
            } else if (tank.getPlayerIndex() == 2 && player2) {
                player2->updateTankWithBattleInfo(*(tank.getAlgorithm()), satelliteView);
            } else {
                actionIgnored = true;
            }
            break;
        }
        
        case ActionRequest::DoNothing: {
            actionName = "DoNothing";
            // Do nothing
            break;
        }
    }
    
    // Record the action for output
    tank.setLastAction(actionName, actionIgnored);
}

void GameManager::handleCollisions() {
    // Check for shell collisions with tanks
    for (auto& shell : shells) {
        if (shell->getObjectType() == CellType::SHELL) {
            // Cast to Shell to use Shell-specific methods
            Shell* shellPtr = dynamic_cast<Shell*>(shell.get());
            if (!shellPtr) continue;  // Skip if cast fails
            
            for (auto& tank : tanks) {
                if (tank->isAlive() && shellPtr->collidesWith(*tank)) {
                    // Shell hits tank
                    // Check if shell belongs to opposing player
                    if (shellPtr->getOwnerPlayerIndex() != tank->getPlayerIndex()) {
                        // Enemy shell hits tank
                        tank->destroy();
                        board.setCellType(tank->getX(), tank->getY(), CellType::BOOM);
                        std::cout << "Tank " << tank->getTankID() << " of Player " 
                                  << tank->getPlayerIndex() << " was destroyed!" << std::endl;
                    }
                    
                    // Shell explodes
                    shellPtr->explode(board);
                    shell->setObjectType(CellType::BOOM);
                }
            }
        }
    }
    
    // Check for shell collisions with mines
    for (auto& shell : shells) {
        if (shell->getObjectType() == CellType::SHELL) {
            // Get the shell's position
            int x = shell->getX();
            int y = shell->getY();
            
            // Cast to Shell to use Shell-specific methods
            Shell* shellPtr = dynamic_cast<Shell*>(shell.get());
            if (!shellPtr) continue;  // Skip if cast fails
            
            // Check if there's a mine at this position
            if (board.getCellType(x, y) == CellType::MINE) {
                // Shell hits mine, both explode
                shellPtr->explode(board);
                shell->setObjectType(CellType::BOOM);
                board.setCellType(x, y, CellType::EMPTY);
            }
        }
    }
    
    // Check for shell collisions with other shells
    for (size_t i = 0; i < shells.size(); ++i) {
        if (shells[i]->getObjectType() == CellType::SHELL) {
            Shell* shellPtr1 = dynamic_cast<Shell*>(shells[i].get());
            if (!shellPtr1) continue;  // Skip if cast fails
            
            for (size_t j = i + 1; j < shells.size(); ++j) {
                if (shells[j]->getObjectType() == CellType::SHELL) {
                    Shell* shellPtr2 = dynamic_cast<Shell*>(shells[j].get());
                    if (!shellPtr2) continue;  // Skip if cast fails
                    
                    if (shellPtr1->collidesWith(*shellPtr2)) {
                        // Shells collide, both explode
                        shellPtr1->explode(board);
                        shells[i]->setObjectType(CellType::BOOM);
                        shellPtr2->explode(board);
                        shells[j]->setObjectType(CellType::BOOM);
                    }
                }
            }
        }
    }
}

bool GameManager::isGameOver() {
    // Count tanks for each player
    int player1Tanks = 0;
    int player2Tanks = 0;
    
    for (const auto& tank : tanks) {
        if (tank->isAlive()) {
            if (tank->getObjectType() == CellType::TANK1) {
                player1Tanks++;
            } else if (tank->getObjectType() == CellType::TANK2) {
                player2Tanks++;
            }
        }
    }
    
    // Check if a player has no tanks left
    if (player1Tanks == 0 && player2Tanks == 0) {
        gameOver = true;
        gameResultMessage = "Tie, both players have zero tanks";
        return true;
    } else if (player1Tanks == 0) {
        gameOver = true;
        gameResultMessage = "Player 2 won with " + std::to_string(player2Tanks) + " tanks still alive";
        return true;
    } else if (player2Tanks == 0) {
        gameOver = true;
        gameResultMessage = "Player 1 won with " + std::to_string(player1Tanks) + " tanks still alive";
        return true;
    }
    
    return false;
}

void GameManager::writeOutputLine() {
    // Write the actions of each tank to the output file
    if (!outputFile.is_open()) return;
    
    bool first = true;
    for (const auto& tank : tanks) {
        if (!first) {
            outputFile << ", ";
        }
        outputFile << tank->getActionString();
        first = false;
    }
    outputFile << std::endl;
}

void GameManager::writeFinalOutput() {
    // Write the final game result to the output file
    if (!outputFile.is_open()) return;
    
    outputFile << gameResultMessage << std::endl;
}

void GameManager::cleanupDeadObjects() {
    // Remove dead shells
    auto shellIt = std::remove_if(shells.begin(), shells.end(),
        [](const auto& shell) {
            // Remove if the shell is not SHELL type (e.g., turned into BOOM)
            return shell->getObjectType() != CellType::SHELL;
        });
    
    shells.erase(shellIt, shells.end());
    
    // We don't remove dead tanks since we need them for output formatting
}

void GameManager::displayGame() {
    if (!visualMode) {
        return;
    }
    
    // Clear screen (platform-independent)
    std::cout << "\033[2J\033[1;1H";
    
    std::cout << "=== Game State (Visual Mode) - Turn: " << currentStep << " ===" << std::endl;
    
    // Create a 2D map of the board
    std::vector<std::vector<std::string>> visualBoard(
        board.getHeight(), 
        std::vector<std::string>(board.getWidth(), "⬜")  // Default to empty space
    );
    
    // Fill in the board with cell types
    for (int y = 0; y < board.getHeight(); ++y) {
        for (int x = 0; x < board.getWidth(); ++x) {
            CellType cellType = board.getCellType(x, y);
            
            switch (cellType) {
                case CellType::WALL:
                    visualBoard[y][x] = "🟩";  // Wall
                    break;
                case CellType::WEAK_WALL:
                    visualBoard[y][x] = "🟨";  // Weak wall
                    break;
                case CellType::MINE:
                    visualBoard[y][x] = "💣";  // Mine (changed to bomb emoji for better visibility)
                    break;
                case CellType::BOOM:
                    visualBoard[y][x] = "💥";  // Explosion
                    break;
                case CellType::EMPTY:
                default:
                    visualBoard[y][x] = "⬜";  // Empty space
                    break;
            }
        }
    }
    
    // Overlay shells
    for (const auto& shell : shells) {
        if (shell->getObjectType() == CellType::SHELL) {
            int x = shell->getX();
            int y = shell->getY();
            
            if (x >= 0 && x < board.getWidth() && y >= 0 && y < board.getHeight()) {
                visualBoard[y][x] = "🚀";  // Changed to rocket emoji like in Project1
            }
        }
    }
    
    // Overlay tanks
    for (const auto& tank : tanks) {
        if (tank->isAlive()) {
            int x = tank->getX();
            int y = tank->getY();
            
            if (x >= 0 && x < board.getWidth() && y >= 0 && y < board.getHeight()) {
                if (tank->getObjectType() == CellType::TANK1) {
                    visualBoard[y][x] = "🚗";  // Tank 1
                } else if (tank->getObjectType() == CellType::TANK2) {
                    visualBoard[y][x] = "🚙";  // Tank 2
                }
            }
        }
    }
    
    // Display the board
    for (const auto& row : visualBoard) {
        for (const auto& cell : row) {
            std::cout << cell;
        }
        std::cout << std::endl;
    }
    
    // Display the cannon direction for both tanks like in Project1
    std::cout << "\nTank Status:" << std::endl;
    for (const auto& tank : tanks) {
        if (tank->isAlive()) {
            std::string directionSymbol;
            Direction cannonDir = tank->getDirection();
            
            switch (cannonDir) {
                case Direction::UP: directionSymbol = "↑"; break;
                case Direction::UP_RIGHT: directionSymbol = "↗"; break;
                case Direction::RIGHT: directionSymbol = "→"; break;
                case Direction::DOWN_RIGHT: directionSymbol = "↘"; break;
                case Direction::DOWN: directionSymbol = "↓"; break;
                case Direction::DOWN_LEFT: directionSymbol = "↙"; break;
                case Direction::LEFT: directionSymbol = "←"; break;
                case Direction::UP_LEFT: directionSymbol = "↖"; break;
                default: directionSymbol = "?"; break;
            }
            
            std::cout << directionSymbol << " Tank " << tank->getPlayerIndex() << " (ID: " << tank->getTankID() 
                      << ") at position (" << tank->getX() << ", " << tank->getY() 
                      << "), Direction: " << directionSymbol
                      << ", Shells remaining: " << tank->getShellCount() 
                      << ", Cooldown: " << tank->getShootCooldown() << std::endl;
        } else {
            int playerIdx = (tank->getObjectType() == CellType::TANK1) ? 1 : 2;
            std::cout << "✖️ Tank " << playerIdx << " (ID: " << tank->getTankID() 
                      << ") DESTROYED" << std::endl;
        }
    }
    
    // Display shell information
    if (!shells.empty()) {
        std::cout << "\nShell Status:" << std::endl;
        for (const auto& shell : shells) {
            if (shell->getObjectType() == CellType::SHELL) {
                std::cout << "🚀 Shell owned by Player " << dynamic_cast<Shell*>(shell.get())->getOwnerPlayerIndex()
                          << " at position (" << shell->getX() << ", " << shell->getY() << ")"
                          << ", Direction: ";
                          
                Direction shellDir = shell->getDirection();
                switch (shellDir) {
                    case Direction::UP: std::cout << "↑"; break;
                    case Direction::UP_RIGHT: std::cout << "↗"; break;
                    case Direction::RIGHT: std::cout << "→"; break;
                    case Direction::DOWN_RIGHT: std::cout << "↘"; break;
                    case Direction::DOWN: std::cout << "↓"; break;
                    case Direction::DOWN_LEFT: std::cout << "↙"; break;
                    case Direction::LEFT: std::cout << "←"; break;
                    case Direction::UP_LEFT: std::cout << "↖"; break;
                    default: std::cout << "?"; break;
                }
                std::cout << std::endl;
            }
        }
    }
    
    // Display game state
    int player1Tanks = 0;
    int player2Tanks = 0;
    int player1Shells = 0;
    int player2Shells = 0;
    
    for (const auto& tank : tanks) {
        if (tank->isAlive()) {
            if (tank->getObjectType() == CellType::TANK1) {
                player1Tanks++;
                player1Shells += tank->getShellCount();
            } else if (tank->getObjectType() == CellType::TANK2) {
                player2Tanks++;
                player2Shells += tank->getShellCount();
            }
        }
    }
    
    std::cout << "\nGame Summary:" << std::endl;
    std::cout << "Player 1: " << player1Tanks << " tanks, " << player1Shells << " shells" << std::endl;
    std::cout << "Player 2: " << player2Tanks << " tanks, " << player2Shells << " shells" << std::endl;
    
    if (noShellsSteps > 0) {
        std::cout << "Steps without shells: " << noShellsSteps << "/" << MAX_NO_SHELLS_STEPS << std::endl;
    }
    
    if (gameOver) {
        std::cout << "\n🏁 GAME OVER: " << gameResultMessage << std::endl;
    }
    
    // Pause between turns for better visualization
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
}