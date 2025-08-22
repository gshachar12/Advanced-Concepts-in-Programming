#include "GameManager.h"
#include "TankBattleInfo.h"
#include <iostream>
#include <algorithm>

GameManager::GameManager(bool verbose) 
    : verbose_mode_(verbose) {
    // Note: collision_detector_ and action_processor_ will be initialized after game_state_
}

GameResult GameManager::run(
    size_t map_width, 
    size_t map_height,
    SatelliteView& map,
    size_t max_steps, 
    size_t num_shells,
    Player& player1, 
    Player& player2,
    TankAlgorithmFactory player1_tank_algo_factory,
    TankAlgorithmFactory player2_tank_algo_factory) {
    
    if (verbose_mode_) {
        std::cout << ">> Starting game with " << map_width << "x" << map_height 
                  << " map, max " << max_steps << " steps, " 
                  << num_shells << " shells per tank\n" << std::endl;
    }
    
    // Initialize game state from map
    if (!initializeGameFromMap(map, map_width, map_height, num_shells)) {
        if (verbose_mode_) {
            std::cout << "[ERROR] Failed to initialize game from map\n";
        }
        GameResult result;
        result.winner = 0; // Tie for invalid map
        return result;
    }
    
    // Setup tank algorithms
    setupTankAlgorithms(player1_tank_algo_factory, player2_tank_algo_factory);
    
    // Main game loop
    for (size_t turn = 1; turn <= max_steps; ++turn) {
        if (verbose_mode_) {
            logTurnStart(turn);
        }
        
        // Execute one complete turn
        executeTurn(player1, player2);
        
        // Check if game has ended
        if (checkGameEndConditions()) {
            GameResult result = generateGameResult();
            if (verbose_mode_) {
                logGameEnd(result);
            }
            return result;
        }
    }
    
    // Game ended due to max steps reached
    GameResult result = generateGameResult();
    if (verbose_mode_) {
        std::cout << "[INFO] Game ended: Maximum steps (" << max_steps << ") reached\n";
        logGameEnd(result);
    }
    
    return result;
}

bool GameManager::initializeGameFromMap(SatelliteView& map, size_t map_width, size_t map_height, size_t num_shells) {
    // Create game state (with default values for max_steps and shells_per_tank)
    game_state_ = std::make_unique<GameState>(map_width, map_height, 1000, num_shells);
    
    // Initialize collision detector and action processor now that game_state_ exists
    collision_detector_ = std::make_unique<CollisionDetector>(game_state_.get());
    action_processor_ = std::make_unique<ActionProcessor>(game_state_.get(), collision_detector_.get());
    
    // Find tank positions and setup game board
    Position player1_pos, player2_pos;
    bool found_player1 = false, found_player2 = false;
    
    // Scan the map to find tank starting positions and setup board
    for (size_t y = 0; y < map_height; ++y) {
        for (size_t x = 0; x < map_width; ++x) {
            char cell_char = map.getObjectAt(x, y);
            CellType cell = CellType::EMPTY; // Default
            
            // Convert character to CellType (this mapping might need adjustment based on actual format)
            switch (cell_char) {
                case ' ': cell = CellType::EMPTY; break;
                case '#': cell = CellType::WALL; break;
                case '1': 
                    cell = CellType::EMPTY; // Tank position becomes empty
                    if (found_player1) {
                        if (verbose_mode_) {
                            std::cout << "[ERROR] Multiple Player1 tanks found on map\n";
                        }
                        return false;
                    }
                    player1_pos = Position(x, y);
                    found_player1 = true;
                    break;
                case '2':
                    cell = CellType::EMPTY; // Tank position becomes empty
                    if (found_player2) {
                        if (verbose_mode_) {
                            std::cout << "[ERROR] Multiple Player2 tanks found on map\n";
                        }
                        return false;
                    }
                    player2_pos = Position(x, y);
                    found_player2 = true;
                    break;
                default:
                    cell = CellType::EMPTY; break;
            }
            
            // Set board cell type
            game_state_->getBoard().setCellType(x, y, cell);
        }
    }
    
    // Validate that both tanks were found
    if (!found_player1 || !found_player2) {
        if (verbose_mode_) {
            std::cout << "[ERROR] Missing tank positions - Player1: " 
                      << (found_player1 ? "found" : "missing") 
                      << ", Player2: " << (found_player2 ? "found" : "missing") << "\n";
        }
        return false;
    }
    
    // Create and add tanks to game state
    game_state_->addTank(1, player1_pos, Direction::UP);
    game_state_->addTank(2, player2_pos, Direction::UP);
    
    if (verbose_mode_) {
        std::cout << "[INFO] Game initialized - Player1 at (" << player1_pos.x 
                  << "," << player1_pos.y << "), Player2 at (" 
                  << player2_pos.x << "," << player2_pos.y << ")\n";
    }
    
    return true;
}

void GameManager::setupTankAlgorithms(TankAlgorithmFactory factory1, TankAlgorithmFactory factory2) {
    // Clear any existing algorithms
    player1_algorithms_.clear();
    player2_algorithms_.clear();
    
    // Create algorithms for each player's tanks
    auto player1_tanks = game_state_->getPlayerTanks(1);
    auto player2_tanks = game_state_->getPlayerTanks(2);
    
    // Create algorithms for Player1 tanks
    for (size_t i = 0; i < player1_tanks.size(); ++i) {
        auto algo = factory1(0, static_cast<int>(i)); // player_index=0, tank_index=i
        if (algo) {
            player1_algorithms_.push_back(std::move(algo));
        }
    }
    
    // Create algorithms for Player2 tanks
    for (size_t i = 0; i < player2_tanks.size(); ++i) {
        auto algo = factory2(1, static_cast<int>(i)); // player_index=1, tank_index=i
        if (algo) {
            player2_algorithms_.push_back(std::move(algo));
        }
    }
    
    if (verbose_mode_) {
        std::cout << "[INFO] Tank algorithms setup - Player1: " << player1_algorithms_.size() 
                  << " algorithms, Player2: " << player2_algorithms_.size() << " algorithms\n";
    }
}

void GameManager::executeTurn(Player& player1, Player& player2) {
    // 1. Update all tanks with current battle information
    updateTanksWithBattleInfo(player1, player2);
    
    // 2. Execute actions for all tanks
    executeAllTankActions();
    
    // 3. Update shell positions
    updateShells();
    
    // 4. Handle all collisions
    handleCollisions();
    
    // 5. Update tank states (cooldowns, etc.)
    updateTankStates();
}

void GameManager::updateTanksWithBattleInfo(Player& player1, Player& player2) {
    auto player1_tanks = game_state_->getPlayerTanks(1);
    auto player2_tanks = game_state_->getPlayerTanks(2);
    
    // Update Player1 tanks
    for (size_t i = 0; i < player1_tanks.size() && i < player1_algorithms_.size(); ++i) {
        if (player1_tanks[i]->isAlive()) {
            auto battle_info = std::make_unique<TankBattleInfo>(
                player1_tanks[i]->getPosition(),
                player1_tanks[i]->getDirection(),
                player1_tanks[i]->getShellsRemaining(),
                player1_tanks[i]->getShootCooldown(),
                player1_tanks[i]->isAlive(),
                game_state_->getBoard().getWidth(),
                game_state_->getBoard().getHeight()
            );
            
            player1_algorithms_[i]->updateBattleInfo(*battle_info);
        }
    }
    
    // Update Player2 tanks
    for (size_t i = 0; i < player2_tanks.size() && i < player2_algorithms_.size(); ++i) {
        if (player2_tanks[i]->isAlive()) {
            auto battle_info = std::make_unique<TankBattleInfo>(
                player2_tanks[i]->getPosition(),
                player2_tanks[i]->getDirection(),
                player2_tanks[i]->getShellsRemaining(),
                player2_tanks[i]->getShootCooldown(),
                player2_tanks[i]->isAlive(),
                game_state_->getBoard().getWidth(),
                game_state_->getBoard().getHeight()
            );
            
            player2_algorithms_[i]->updateBattleInfo(*battle_info);
        }
    }
}

void GameManager::executeAllTankActions() {
    auto player1_tanks = game_state_->getPlayerTanks(1);
    auto player2_tanks = game_state_->getPlayerTanks(2);
    
    // Execute Player1 tank actions
    for (size_t i = 0; i < player1_tanks.size() && i < player1_algorithms_.size(); ++i) {
        if (player1_tanks[i]->isAlive()) {
            ActionRequest action = player1_algorithms_[i]->getAction();
            bool success = action_processor_->processAction(player1_tanks[i], action);
            
            if (verbose_mode_) {
                logTankAction(player1_tanks[i], action, success);
            }
        }
    }
    
    // Execute Player2 tank actions
    for (size_t i = 0; i < player2_tanks.size() && i < player2_algorithms_.size(); ++i) {
        if (player2_tanks[i]->isAlive()) {
            ActionRequest action = player2_algorithms_[i]->getAction();
            bool success = action_processor_->processAction(player2_tanks[i], action);
            
            if (verbose_mode_) {
                logTankAction(player2_tanks[i], action, success);
            }
        }
    }
}

void GameManager::updateShells() {
    auto shells = game_state_->getAllShells();
    
    for (Shell* shell : shells) {
        if (shell->isActive()) {
            if (verbose_mode_) {
                Position old_pos = shell->getPosition();
                shell->advance();  // Move the shell
                std::cout << "[INFO] Shell moved from (" << old_pos.x << "," << old_pos.y 
                         << ") to (" << shell->getPosition().x << "," << shell->getPosition().y << ")\n";
            } else {
                shell->advance();  // Move the shell
            }
        }
    }
    
    // Remove inactive shells
    game_state_->removeInactiveShells();
}

void GameManager::handleCollisions() {
    collision_detector_->detectCollisions();
}

void GameManager::updateTankStates() {
    auto all_tanks = game_state_->getAllTanks();
    
    for (Tank* tank : all_tanks) {
        if (tank->isAlive()) {
            tank->update();  // Update cooldowns
        }
    }
}

bool GameManager::checkGameEndConditions() {
    auto player1_tanks = game_state_->getPlayerTanks(1);
    auto player2_tanks = game_state_->getPlayerTanks(2);
    
    bool player1_has_alive_tank = false;
    bool player2_has_alive_tank = false;
    
    for (Tank* tank : player1_tanks) {
        if (tank->isAlive()) {
            player1_has_alive_tank = true;
            break;
        }
    }
    
    for (Tank* tank : player2_tanks) {
        if (tank->isAlive()) {
            player2_has_alive_tank = true;
            break;
        }
    }
    
    // Game ends if at least one player has no alive tanks
    return !player1_has_alive_tank || !player2_has_alive_tank;
}

GameResult GameManager::generateGameResult() {
    auto player1_tanks = game_state_->getPlayerTanks(1);
    auto player2_tanks = game_state_->getPlayerTanks(2);
    
    bool player1_has_alive_tank = false;
    bool player2_has_alive_tank = false;
    
    for (Tank* tank : player1_tanks) {
        if (tank->isAlive()) {
            player1_has_alive_tank = true;
            break;
        }
    }
    
    for (Tank* tank : player2_tanks) {
        if (tank->isAlive()) {
            player2_has_alive_tank = true;
            break;
        }
    }
    
    GameResult result;
    if (player1_has_alive_tank && player2_has_alive_tank) {
        result.winner = 0; // Tie
    } else if (player1_has_alive_tank) {
        result.winner = 1; // Player 1 wins
    } else if (player2_has_alive_tank) {
        result.winner = 2; // Player 2 wins
    } else {
        result.winner = 0; // Tie - both eliminated
    }
    
    return result;
}

void GameManager::logTurnStart(size_t turn) {
    std::cout << "\n=== Turn " << turn << " ===================================\n";
}

void GameManager::logTankAction(const Tank* tank, ActionRequest action, bool success) {
    std::string action_name;
    switch (action) {
        case ActionRequest::MoveForward: action_name = "MoveForward"; break;
        case ActionRequest::MoveBackward: action_name = "MoveBackward"; break;
        case ActionRequest::RotateLeft90: action_name = "RotateLeft90"; break;
        case ActionRequest::RotateRight90: action_name = "RotateRight90"; break;
        case ActionRequest::RotateLeft45: action_name = "RotateLeft45"; break;
        case ActionRequest::RotateRight45: action_name = "RotateRight45"; break;
        case ActionRequest::Shoot: action_name = "Shoot"; break;
        case ActionRequest::GetBattleInfo: action_name = "GetBattleInfo"; break;
        case ActionRequest::DoNothing: action_name = "DoNothing"; break;
        default: action_name = "Unknown"; break;
    }
    
    std::cout << "Player" << tank->getPlayerId() << " tank at (" 
              << tank->getPosition().x << "," << tank->getPosition().y 
              << ") performed " << action_name << ": " 
              << (success ? "[SUCCESS]" : "[FAILED]") << "\n";
}

void GameManager::logGameEnd(const GameResult& result) {
    std::cout << "\nGAME ENDED ==========================================\n";
    
    if (result.winner == 1) {
        std::cout << "WINNER: Player 1\n";
    } else if (result.winner == 2) {
        std::cout << "WINNER: Player 2\n";
    } else {
        std::cout << "RESULT: Game ended in a TIE\n";
    }
    
    std::cout << "====================================================\n";
}

void GameManager::displayBoard() {
    if (!game_state_) return;
    
    const Board& board = game_state_->getBoard();
    size_t width = board.getWidth();
    size_t height = board.getHeight();
    
    std::cout << "\n  Current Board State:\n";
    std::cout << "  ";
    for (size_t x = 0; x < width; ++x) {
        std::cout << (x % 10);
    }
    std::cout << "\n";
    
    for (size_t y = 0; y < height; ++y) {
        std::cout << (y % 10) << " ";
        for (size_t x = 0; x < width; ++x) {
            Position pos(x, y);
            char display_char = ' ';
            
            // Check for tanks first
            bool found_tank = false;
            const auto& tanks = game_state_->getAllTanks();
            for (const auto& tank : tanks) {
                if (tank->getPosition() == pos && tank->isAlive()) {
                    display_char = (tank->getPlayerId() == 1) ? '1' : '2';
                    found_tank = true;
                    break;
                }
            }
            
            // Check for shells
            if (!found_tank) {
                const auto& shells = game_state_->getAllShells();
                for (const auto& shell : shells) {
                    if (shell->getPosition() == pos) {
                        display_char = '*';
                        found_tank = true;
                        break;
                    }
                }
            }
            
            // Default to board character
            if (!found_tank) {
                display_char = board.getObjectAt(x, y);
            }
            
            std::cout << display_char;
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

void GameManager::displayGameState() {
    if (!game_state_) return;
    
    std::cout << "  Game Status:\n";
    const auto& tanks = game_state_->getAllTanks();
    for (const auto& tank : tanks) {
        if (tank->isAlive()) {
            std::cout << "  Player" << tank->getPlayerId() 
                      << " Tank: (" << tank->getPosition().x << "," << tank->getPosition().y << ") "
                      << "Shells: " << tank->getShellsRemaining()
                      << " Dir: " << DirectionUtils::toString(tank->getDirection()) << "\n";
        } else {
            std::cout << "  Player" << tank->getPlayerId() << " Tank: DESTROYED\n";
        }
    }
    
    const auto& shells = game_state_->getAllShells();
    if (!shells.empty()) {
        std::cout << "  Active Shells: " << shells.size() << "\n";
        for (const auto& shell : shells) {
            std::cout << "    Shell at (" << shell->getPosition().x << "," << shell->getPosition().y << ") "
                      << "Owner: Player" << shell->getOwnerPlayerId() << "\n";
        }
    }
}
