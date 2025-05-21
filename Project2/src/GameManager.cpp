#include "GameManager.h"
#include "BoardSatelliteView.h"
#include "MyPlayer.h"
#include "algorithms/Controller.h"
#include "Shell.h"
#include <algorithm>
#include <iostream>

// Implement MyPlayerFactory
std::unique_ptr<Player> MyPlayerFactory::create(int player_index, 
                                             size_t x, size_t y,
                                             size_t max_steps, 
                                             size_t num_shells) const {
    return std::make_unique<MyPlayer>(player_index, x, y, max_steps, num_shells);
}

// Implement MyTankAlgorithmFactory
std::unique_ptr<TankAlgorithm> MyTankAlgorithmFactory::create(
    int player_index, int tank_index) const {
    return std::make_unique<Controller>(player_index, tank_index);
}

// GameManager constructor
GameManager::GameManager(const PlayerFactory& playerFactory, 
                         const TankAlgorithmFactory& algorithmFactory)
    : currentStep(0), 
      gameOver(false),
      noShellsSteps(0),
      playerFactory(dynamic_cast<const MyPlayerFactory&>(playerFactory)),
      algorithmFactory(dynamic_cast<const MyTankAlgorithmFactory&>(algorithmFactory)) {
    
    // Initialize board with default settings
    board.setWrapAround(true);
}

bool GameManager::readBoard(const std::string& filename) {
    // Use the new board loading method
    if (!board.loadFromFile(filename)) {
        std::cerr << "Failed to load board from file: " << filename << std::endl;
        return false;
    }
    
    // Open output file (same name with .out extension)
    std::string outputFilename = filename + ".out";
    outputFile.open(outputFilename);
    if (!outputFile.is_open()) {
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

void GameManager::initializePlayers() {
    // Create players using the factories
    player1 = playerFactory.create(1, board.getWidth(), board.getHeight(), 
                                 board.getMaxSteps(), board.getNumShellsPerTank());
    player2 = playerFactory.create(2, board.getWidth(), board.getHeight(), 
                                 board.getMaxSteps(), board.getNumShellsPerTank());
}

void GameManager::initializeTanks() {
    // Clear any existing tanks and shells
    tanks.clear();
    shells.clear();
    
    // Scan the board for tank positions and create tanks
    int player1TankCount = 0;
    int player2TankCount = 0;
    
    for (int y = 0; y < board.getHeight(); ++y) {
        for (int x = 0; x < board.getWidth(); ++x) {
            CellType cellType = board.getCellType(x, y);
            
            if (cellType == CellType::TANK1) {
                // Create a Player 1 tank with the algorithm
                auto algorithm = algorithmFactory.create(1, player1TankCount);
                Direction startDir = Direction::LEFT; // Player 1 tanks face left
                
                auto tank = std::make_unique<Tank>(
                    x, y, startDir, CellType::TANK1, 
                    player1TankCount, 1, player1TankCount,
                    board.getNumShellsPerTank(), std::move(algorithm)
                );
                
                tanks.push_back(std::move(tank));
                player1TankCount++;
            }
            else if (cellType == CellType::TANK2) {
                // Create a Player 2 tank with the algorithm
                auto algorithm = algorithmFactory.create(2, player2TankCount);
                Direction startDir = Direction::RIGHT; // Player 2 tanks face right
                
                auto tank = std::make_unique<Tank>(
                    x, y, startDir, CellType::TANK2, 
                    player2TankCount, 2, player2TankCount,
                    board.getNumShellsPerTank(), std::move(algorithm)
                );
                
                tanks.push_back(std::move(tank));
                player2TankCount++;
            }
        }
    }
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
        writeFinalOutput();
        return;
    }
    
    // Main game loop
    while (!gameOver && currentStep < board.getMaxSteps()) {
        // Process a game round
        processRound();
        currentStep++;
        
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
            // Create a satellite view of the current board state
            BoardSatelliteView satelliteView(board, tanks, shells, tank);
            
            // Get the appropriate player for this tank
            Player* player = (tank.getPlayerIndex() == 1) ? player1.get() : player2.get();
            
            // Update the tank with battle info
            player->updateTankWithBattleInfo(*tank.algorithm.get(), satelliteView);
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
            for (auto& tank : tanks) {
                if (tank->isAlive() && shell->collidesWith(*tank)) {
                    // Shell hits tank
                    // Check if shell belongs to opposing player
                    if (shell->getOwnerPlayerIndex() != tank->getPlayerIndex()) {
                        // Enemy shell hits tank
                        tank->destroy();
                        board.setCellType(tank->getX(), tank->getY(), CellType::BOOM);
                        std::cout << "Tank " << tank->getTankID() << " of Player " 
                                  << tank->getPlayerIndex() << " was destroyed!" << std::endl;
                    }
                    
                    // Shell explodes
                    shell->explode(board);
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
            
            // Check if there's a mine at this position
            if (board.getCellType(x, y) == CellType::MINE) {
                // Shell hits mine, both explode
                shell->explode(board);
                shell->setObjectType(CellType::BOOM);
                board.setCellType(x, y, CellType::EMPTY);
            }
        }
    }
    
    // Check for shell collisions with other shells
    for (size_t i = 0; i < shells.size(); ++i) {
        if (shells[i]->getObjectType() == CellType::SHELL) {
            for (size_t j = i + 1; j < shells.size(); ++j) {
                if (shells[j]->getObjectType() == CellType::SHELL && 
                    shells[i]->collidesWith(*shells[j])) {
                    // Shells collide, both explode
                    shells[i]->explode(board);
                    shells[i]->setObjectType(CellType::BOOM);
                    shells[j]->explode(board);
                    shells[j]->setObjectType(CellType::BOOM);
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