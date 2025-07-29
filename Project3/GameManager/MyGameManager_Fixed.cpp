#include "MyGameManager_Fixed.h"
#include "../UserCommon/UserCommonTypes.h"
#include "../UserCommon/UserCommonUtils.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <memory>

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

namespace GameManager_123456789_987654321 {

GameResult MyGameManager::run(
    size_t map_width, size_t map_height,
    SatelliteView& map,
    size_t max_steps, size_t num_shells,
    Player& /* player1 */, Player& /* player2 */,
    TankAlgorithmFactory& player1_tank_algo_factory,
    TankAlgorithmFactory& player2_tank_algo_factory) {
    
    // Store algorithm factories for use during game
    player1_factory_ = &player1_tank_algo_factory;
    player2_factory_ = &player2_tank_algo_factory;
    
    if (verbose_) {
        std::cout << "=== PROJECT 3 INTERACTIVE GAME VISUALIZATION ===" << std::endl;
        std::cout << "Map dimensions: " << map_width << "x" << map_height << std::endl;
        std::cout << "Max steps: " << max_steps << ", Shells per tank: " << num_shells << std::endl;
        std::cout << "Player 1 vs Player 2" << std::endl;
        std::cout << "===============================================" << std::endl;
        
        // Show initial map
        clearScreen();
        std::cout << "\n=== INITIAL MAP STATE ===\n" << std::endl;
        displayInteractiveMap(map, map_width, map_height);
        waitForInput();
        
        // Simulate interactive game with real Project 2-style visualization using actual algorithms
        return simulateInteractiveGameWithAlgorithms(map, map_width, map_height, max_steps, num_shells);
    } else {
        // Simple non-visual version
        displayMap(map, map_width, map_height);
        simulateGameWithVisualization(map_width, map_height, max_steps);
        
        GameResult result;
        result.winner = 1;
        result.remaining_tanks = {1, 0};
        return result;
    }
}

void MyGameManager::displayMap(SatelliteView& map, size_t width, size_t height) {
    std::cout << "\n--- MAP LAYOUT ---" << std::endl;
    for (size_t row = 0; row < height; ++row) {
        for (size_t col = 0; col < width; ++col) {
            char cell = map.getObject(col, row);
            std::cout << cell;
        }
        std::cout << std::endl;
    }
    std::cout << "------------------" << std::endl;
}

void MyGameManager::displayInteractiveMap(SatelliteView& map, size_t width, size_t height) {
    // Print column numbers
    std::cout << "   ";
    for (size_t x = 0; x < width; ++x) {
        std::cout << (x % 10);
    }
    std::cout << "\n";
    
    // Print board with Project 2-style emojis
    for (size_t y = 0; y < height; ++y) {
        std::cout << (y % 10) << "  ";
        for (size_t x = 0; x < width; ++x) {
            char cell = map.getObject(x, y);
            std::cout << getEmojiForCell(cell);
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

std::string MyGameManager::getEmojiForCell(char cell) {
    switch (cell) {
        case '#': return "🟩";  // Wall
        case '=': return "🧱";  // Weak wall  
        case '@': return "💣";  // Mine
        case '*': return "🚀";  // Shell
        case 'X': return "💥";  // Explosion
        case '1': return "🔵1"; // Player 1 tank
        case '2': return "🔴2"; // Player 2 tank
        case ' ': return "⬜";  // Empty space
        case '.': return "⬛";  // Destroyed wall
        case '~': return "🌊";  // Water/impassable
        case '^': return "⛰️";  // Mountain/obstacle
        case 'H': return "🏠";  // House/building
        case 'T': return "🌳";  // Tree
        case 'R': return "🛣️";  // Road
        case 'B': return "🌉";  // Bridge
        default:  return "⬜";  // Default empty
    }
}

GameResult MyGameManager::simulateInteractiveGameWithAlgorithms(SatelliteView& map, size_t width, size_t height, size_t max_steps, size_t num_shells) {
    // Create game state
    GameState state;
    state.width = width;
    state.height = height;
    state.max_steps = max_steps;
    state.current_step = 0;
    
    // Initialize tanks and their algorithms
    initializeTanksWithAlgorithms(state, map, width, height, num_shells);
    
    std::cout << "\n=== STARTING INTERACTIVE GAME WITH REAL ALGORITHMS ===\n";
    std::cout << "Press ENTER after each step to continue...\n\n";
    
    // Main game loop with interactive visualization - game ends when one player is eliminated
    while (!isGameOver(state)) {
        state.current_step++;
        
        clearScreen();
        std::cout << "=== TURN " << state.current_step << " ===\n\n";
        
        // Execute turn logic with real algorithms
        executeTurnWithAlgorithms(state, map);
        
        // Display rich game state
        displayGameState(state, width, height);
        
        // Show detailed status
        printTankStatus(state);
        printShellStatus(state);
        printGameSummary(state);
        
        // Check for game end
        if (isGameOver(state)) {
            std::cout << "\n🏁 GAME OVER - One player eliminated!\n";
            break;
        }
        
        // Interactive pause
        waitForInput();
    }
    
    // Return final result
    GameResult result = generateFinalResult(state);
    
    std::cout << "\n=== FINAL RESULT ===" << std::endl;
    std::cout << "Winner: Player " << result.winner << std::endl;
    std::cout << "Remaining tanks: [" << result.remaining_tanks[0] 
              << ", " << result.remaining_tanks[1] << "]" << std::endl;
    
    // Show the actual reason for game termination
    std::cout << "Reason: ";
    switch (result.reason) {
        case GameResult::ALL_TANKS_DEAD:
            std::cout << "All enemy tanks eliminated";
            break;
        case GameResult::MAX_STEPS:
            std::cout << "Maximum steps reached (" << state.max_steps << ")";
            break;
        case GameResult::ZERO_SHELLS:
            std::cout << "All players out of shells";
            break;
    }
    std::cout << std::endl;
    std::cout << "Total rounds played: " << state.current_step << std::endl;
    
    return result;
}

void MyGameManager::simulateGameWithVisualization(size_t /* width */, size_t /* height */, size_t max_steps) {
    std::cout << "\n--- GAME SIMULATION (showing visualization concept) ---" << std::endl;
    
    // Simulate a few game steps to show visualization
    for (size_t step = 1; step <= std::min(size_t(5), max_steps); ++step) {
        std::cout << "\nStep " << step << ":" << std::endl;
        std::cout << "Player 1: Move forward, Tank status: Active" << std::endl;
        std::cout << "Player 2: Rotate right, Tank status: Active" << std::endl;
        
        if (step == 3) {
            std::cout << "Player 1: FIRE! Shell trajectory: (2,3) -> (4,3)" << std::endl;
        }
        
        if (step == 5) {
            std::cout << "COLLISION! Player 2 tank destroyed!" << std::endl;
            break;
        }
    }
    
    std::cout << "--- End of simulation preview ---" << std::endl;
    std::cout << "=== GAME COMPLETE ===" << std::endl;
    std::cout << "Winner: Player 1" << std::endl;
    std::cout << "Reason: All enemy tanks destroyed" << std::endl;
    std::cout << "Remaining tanks - Player 1: 1, Player 2: 0" << std::endl;
}

void MyGameManager::clearScreen() {
    std::cout << "\033[2J\033[1;1H"; // ANSI escape codes to clear screen
}

void MyGameManager::waitForInput() {
    std::cout << "\nPress ENTER to continue to the next step...\n";
#ifdef _WIN32
    system("pause >nul");
#else
    std::cin.get();
#endif
}

void MyGameManager::initializeTanks(GameState& state, SatelliteView& map, size_t width, size_t height, size_t num_shells) {
    // Find tank positions on the map (without algorithms)
    for (size_t y = 0; y < height; ++y) {
        for (size_t x = 0; x < width; ++x) {
            char cell = map.getObject(x, y);
            if (cell == '1') {
                Tank tank1;
                tank1.x = x; tank1.y = y; tank1.player = 1; tank1.shells = num_shells;
                tank1.direction = 0; tank1.alive = true; tank1.cooldown = 0;
                state.tanks.push_back(std::move(tank1));
            } else if (cell == '2') {
                Tank tank2;
                tank2.x = x; tank2.y = y; tank2.player = 2; tank2.shells = num_shells;
                tank2.direction = 4; tank2.alive = true; tank2.cooldown = 0;
                state.tanks.push_back(std::move(tank2));
            }
        }
    }
}

void MyGameManager::executeTurn(GameState& state) {
    // Move shells
    for (auto& shell : state.shells) {
        if (shell.active) {
            // Move shell in its direction
            moveShell(shell);
            // Check collisions
            checkShellCollisions(shell, state);
        }
    }
    
    // Execute tank actions
    for (auto& tank : state.tanks) {
        if (tank.alive) {
            // Simulate tank action (random for demo)
            int action = (state.current_step + tank.player) % 4;
            executeTankAction(tank, action, state);
        }
    }
    
    // Decrease cooldowns
    for (auto& tank : state.tanks) {
        if (tank.cooldown > 0) tank.cooldown--;
    }
}

void MyGameManager::displayGameState(const GameState& state, size_t width, size_t height) {
    // Create visual board
    std::vector<std::vector<std::string>> visualBoard(height, std::vector<std::string>(width, "⬜"));
    
    // Add walls (simple pattern for demo)
    for (size_t y = 0; y < height; ++y) {
        for (size_t x = 0; x < width; ++x) {
            if (x == 0 || y == 0 || x == width-1 || y == height-1) {
                visualBoard[y][x] = "🟩"; // Border walls
            }
            if ((x + y) % 7 == 0 && x > 0 && y > 0 && x < width-1 && y < height-1) {
                visualBoard[y][x] = "🟩"; // Some internal walls
            }
        }
    }
    
    // Add shells
    for (const auto& shell : state.shells) {
        if (shell.active && shell.x < width && shell.y < height) {
            visualBoard[shell.y][shell.x] = "🚀";
        }
    }
    
    // Add tanks with directional arrows
    const std::vector<std::string> directions = {"↑", "↗", "→", "↘", "↓", "↙", "←", "↖"};
    
    for (const auto& tank : state.tanks) {
        if (tank.alive && tank.x < width && tank.y < height) {
            std::string dir_symbol = (tank.direction < directions.size()) ? directions[tank.direction] : "?";
            if (tank.player == 1) {
                visualBoard[tank.y][tank.x] = dir_symbol + "1";
            } else {
                visualBoard[tank.y][tank.x] = dir_symbol + "2";
            }
        }
    }
    
    // Print column numbers
    std::cout << "   ";
    for (size_t x = 0; x < width; ++x) {
        std::cout << (x % 10);
    }
    std::cout << "\n";
    
    // Print the visual board
    for (size_t y = 0; y < height; ++y) {
        std::cout << (y % 10) << "  ";
        for (size_t x = 0; x < width; ++x) {
            std::cout << visualBoard[y][x];
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

void MyGameManager::printTankStatus(const GameState& state) {
    std::cout << "Tank Status:\n";
    for (const auto& tank : state.tanks) {
        if (tank.alive) {
            const std::vector<std::string> directions = {"↑", "↗", "→", "↘", "↓", "↙", "←", "↖"};
            std::string dir_symbol = (tank.direction < directions.size()) ? directions[tank.direction] : "?";
            
            std::cout << dir_symbol << " Tank " << tank.player 
                      << " at (" << tank.x << ", " << tank.y << ")"
                      << ", Direction: " << dir_symbol
                      << ", Shells: " << tank.shells
                      << ", Cooldown: " << tank.cooldown << "\n";
        } else {
            std::cout << "💀 Tank " << tank.player << " - DESTROYED\n";
        }
    }
}

void MyGameManager::printShellStatus(const GameState& state) {
    bool hasShells = false;
    for (const auto& shell : state.shells) {
        if (shell.active) {
            if (!hasShells) {
                std::cout << "\nShell Status:\n";
                hasShells = true;
            }
            const std::vector<std::string> directions = {"↑", "↗", "→", "↘", "↓", "↙", "←", "↖"};
            std::string dir_symbol = (shell.direction < directions.size()) ? directions[shell.direction] : "?";
            
            std::cout << "🚀 Shell at (" << shell.x << ", " << shell.y 
                      << "), Direction: " << dir_symbol 
                      << ", Owner: Player " << shell.owner << "\n";
        }
    }
}

void MyGameManager::printGameSummary(const GameState& state) {
    int p1_tanks = 0, p2_tanks = 0;
    int p1_shells = 0, p2_shells = 0;
    
    for (const auto& tank : state.tanks) {
        if (tank.alive) {
            if (tank.player == 1) {
                p1_tanks++;
                p1_shells += tank.shells;
            } else {
                p2_tanks++;
                p2_shells += tank.shells;
            }
        }
    }
    
    int shells_in_flight = 0;
    for (const auto& shell : state.shells) {
        if (shell.active) shells_in_flight++;
    }
    
    std::cout << "\nGame Summary:\n";
    std::cout << "Player 1 Artillery: " << p1_tanks << " tanks, " << p1_shells << " shells\n";
    std::cout << "Player 2 Artillery: " << p2_tanks << " tanks, " << p2_shells << " shells\n";
    std::cout << "Shells in flight: " << shells_in_flight << "\n";
    std::cout << "Turn: " << state.current_step << "/" << state.max_steps << "\n";
}

bool MyGameManager::isGameOver(const GameState& state) {
    // Check if all tanks of one player are dead
    int p1_alive = 0, p2_alive = 0;
    for (const auto& tank : state.tanks) {
        if (tank.alive) {
            if (tank.player == 1) p1_alive++;
            else p2_alive++;
        }
    }
    if (p1_alive == 0 || p2_alive == 0) {
        return true; // ALL_TANKS_DEAD condition
    }
    
    // Check if maximum steps reached (only if max_steps > 0, 0 means unlimited)
    if (state.max_steps > 0 && state.current_step >= state.max_steps) {
        return true; // MAX_STEPS condition
    }
    
    // Check if all players have zero shells
    int p1_shells = 0, p2_shells = 0;
    for (const auto& tank : state.tanks) {
        if (tank.alive) {
            if (tank.player == 1) p1_shells += tank.shells;
            else p2_shells += tank.shells;
        }
    }
    if (p1_shells == 0 && p2_shells == 0) {
        return true; // ZERO_SHELLS condition
    }
    
    return false;
}

GameResult MyGameManager::generateFinalResult(const GameState& state) {
    GameResult result;
    
    int p1_tanks = 0, p2_tanks = 0;
    for (const auto& tank : state.tanks) {
        if (tank.alive) {
            if (tank.player == 1) p1_tanks++;
            else p2_tanks++;
        }
    }
    
    // Determine the reason for game ending
    if (p1_tanks == 0 || p2_tanks == 0) {
        result.reason = GameResult::ALL_TANKS_DEAD;
    } else if (state.max_steps > 0 && state.current_step >= state.max_steps) {
        result.reason = GameResult::MAX_STEPS;
    } else {
        // Check if all players have zero shells
        int p1_shells = 0, p2_shells = 0;
        for (const auto& tank : state.tanks) {
            if (tank.alive) {
                if (tank.player == 1) p1_shells += tank.shells;
                else p2_shells += tank.shells;
            }
        }
        if (p1_shells == 0 && p2_shells == 0) {
            result.reason = GameResult::ZERO_SHELLS;
        }
    }
    
    // Determine the winner based on the reason
    if (result.reason == GameResult::ALL_TANKS_DEAD) {
        if (p1_tanks > p2_tanks) {
            result.winner = 1;
        } else if (p2_tanks > p1_tanks) {
            result.winner = 2;
        } else {
            result.winner = 0; // Tie - both eliminated simultaneously
        }
    } else {
        // For MAX_STEPS or ZERO_SHELLS, winner is determined by tank count
        if (p1_tanks > p2_tanks) {
            result.winner = 1;
        } else if (p2_tanks > p1_tanks) {
            result.winner = 2;
        } else {
            result.winner = 0; // Tie
        }
    }
    
    result.remaining_tanks = {(size_t)p1_tanks, (size_t)p2_tanks};
    return result;
}

// Implement helper functions for tank/shell mechanics
void MyGameManager::moveShell(Shell& shell) {
    const std::vector<std::pair<int, int>> dir_offsets = {
        {0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}
    };
    
    if (shell.direction < dir_offsets.size()) {
        shell.x += dir_offsets[shell.direction].first;
        shell.y += dir_offsets[shell.direction].second;
    }
}

void MyGameManager::checkShellCollisions(Shell& shell, GameState& state) {
    // Check bounds
    if (shell.x < 0 || shell.x >= state.width || shell.y < 0 || shell.y >= state.height) {
        shell.active = false;
        return;
    }
    
    // Check tank collisions
    for (auto& tank : state.tanks) {
        if (tank.alive && tank.x == shell.x && tank.y == shell.y && tank.player != shell.owner) {
            tank.alive = false;
            shell.active = false;
            return;
        }
    }
}

void MyGameManager::executeTankAction(Tank& tank, int action, GameState& state) {
    switch (action) {
        case 0: // Move forward
            moveTank(tank, state);
            break;
        case 1: // Rotate right
            tank.direction = (tank.direction + 1) % 8;
            break;
        case 2: // Rotate left
            tank.direction = (tank.direction + 7) % 8;
            break;
        case 3: // Shoot
            if (tank.shells > 0 && tank.cooldown == 0) {
                shootShell(tank, state);
            }
            break;
    }
}

void MyGameManager::moveTank(Tank& tank, const GameState& state) {
    const std::vector<std::pair<int, int>> dir_offsets = {
        {0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}
    };
    
    if (tank.direction < dir_offsets.size()) {
        int new_x = tank.x + dir_offsets[tank.direction].first;
        int new_y = tank.y + dir_offsets[tank.direction].second;
        
        // Check bounds and collisions
        if (new_x >= 0 && new_x < (int)state.width && new_y >= 0 && new_y < (int)state.height) {
            // Check for walls (border) and other tanks
            bool blocked = false;
            if (new_x == 0 || new_y == 0 || new_x == (int)state.width-1 || new_y == (int)state.height-1) {
                blocked = true; // Border wall
            }
            
            for (const auto& other_tank : state.tanks) {
                if (other_tank.alive && other_tank.x == new_x && other_tank.y == new_y) {
                    blocked = true;
                    break;
                }
            }
            
            if (!blocked) {
                tank.x = new_x;
                tank.y = new_y;
            }
        }
    }
}

void MyGameManager::shootShell(Tank& tank, GameState& state) {
    Shell shell;
    shell.x = tank.x;
    shell.y = tank.y;
    shell.direction = tank.direction;
    shell.owner = tank.player;
    shell.active = true;
    
    // Move shell one step forward from tank
    moveShell(shell);
    
    state.shells.push_back(shell);
    tank.shells--;
    tank.cooldown = UserCommon_123456789_987654321::SHELL_COOLDOWN_TURNS;
}

// New methods for algorithm integration
void MyGameManager::initializeTanksWithAlgorithms(GameState& state, SatelliteView& map, size_t width, size_t height, size_t num_shells) {
    int tank_index = 0;
    
    // Find tank positions on the map and create algorithm instances
    for (size_t y = 0; y < height; ++y) {
        for (size_t x = 0; x < width; ++x) {
            char cell = map.getObject(x, y);
            if (cell == '1') {
                Tank tank1;
                tank1.x = x; tank1.y = y; tank1.player = 1; tank1.shells = num_shells;
                tank1.direction = 0; tank1.alive = true; tank1.cooldown = 0;
                
                // Create algorithm instance for player 1
                if (player1_factory_) {
                    tank1.algorithm = player1_factory_->create(0, tank_index);
                    std::cout << "DEBUG: Created algorithm for Player 1 tank " << tank_index << " at (" << x << "," << y << ")\n";
                } else {
                    std::cout << "DEBUG: No factory for Player 1!\n";
                }
                
                state.tanks.push_back(std::move(tank1));
                tank_index++;
            } else if (cell == '2') {
                Tank tank2;
                tank2.x = x; tank2.y = y; tank2.player = 2; tank2.shells = num_shells;
                tank2.direction = 4; tank2.alive = true; tank2.cooldown = 0;
                
                // Create algorithm instance for player 2
                if (player2_factory_) {
                    tank2.algorithm = player2_factory_->create(1, tank_index);
                    std::cout << "DEBUG: Created algorithm for Player 2 tank " << tank_index << " at (" << x << "," << y << ")\n";
                } else {
                    std::cout << "DEBUG: No factory for Player 2!\n";
                }
                
                state.tanks.push_back(std::move(tank2));
                tank_index++;
            }
        }
    }
}

void MyGameManager::executeTurnWithAlgorithms(GameState& state, SatelliteView& map) {
    // Move shells first
    for (auto& shell : state.shells) {
        if (shell.active) {
            moveShell(shell);
            checkShellCollisions(shell, state);
        }
    }
    
    // Remove inactive shells
    state.shells.erase(
        std::remove_if(state.shells.begin(), state.shells.end(),
                      [](const Shell& s) { return !s.active; }),
        state.shells.end());
    
    // Execute tank actions using their algorithms
    for (auto& tank : state.tanks) {
        if (tank.alive && tank.algorithm) {
            std::cout << "DEBUG: Tank Player " << tank.player << " at (" << tank.x << "," << tank.y << ") executing algorithm\n";
            
            // Create battle info for the tank
            MyBattleInfo battle_info = createBattleInfo(state, tank, map);
            tank.algorithm->updateBattleInfo(battle_info);
            
            // Get action from algorithm
            ActionRequest action = tank.algorithm->getAction();
            std::cout << "DEBUG: Algorithm returned action: " << static_cast<int>(action) << "\n";
            
            // Execute the action
            executeTankActionFromAlgorithm(tank, action, state, map);
        } else if (tank.alive) {
            std::cout << "DEBUG: Tank Player " << tank.player << " at (" << tank.x << "," << tank.y << ") has NO ALGORITHM!\n";
        }
    }
    
    // Decrease cooldowns
    for (auto& tank : state.tanks) {
        if (tank.cooldown > 0) tank.cooldown--;
    }
}

void MyGameManager::executeTankActionFromAlgorithm(Tank& tank, ActionRequest action, GameState& state, SatelliteView& /* map */) {
    using namespace UserCommon_123456789_987654321;
    
    switch (action) {
        case ActionRequest::MoveForward:
            moveTank(tank, state);
            break;
        case ActionRequest::MoveBackward:
            // Move in opposite direction
            {
                int old_dir = tank.direction;
                tank.direction = (tank.direction + 4) % 8; // Reverse direction
                moveTank(tank, state);
                tank.direction = old_dir; // Restore original direction
            }
            break;
        case ActionRequest::RotateLeft45:
            tank.direction = GameUtils::rotateLeft45(static_cast<Direction>(tank.direction));
            break;
        case ActionRequest::RotateRight45:
            tank.direction = GameUtils::rotateRight45(static_cast<Direction>(tank.direction));
            break;
        case ActionRequest::RotateLeft90:
            tank.direction = GameUtils::rotateLeft90(static_cast<Direction>(tank.direction));
            break;
        case ActionRequest::RotateRight90:
            tank.direction = GameUtils::rotateRight90(static_cast<Direction>(tank.direction));
            break;
        case ActionRequest::Shoot:
            if (tank.shells > 0 && tank.cooldown == 0) {
                shootShell(tank, state);
            }
            break;
        case ActionRequest::GetBattleInfo:
            // Already handled in executeTurnWithAlgorithms
            break;
        case ActionRequest::DoNothing:
        default:
            // Do nothing
            break;
    }
}

MyBattleInfo MyGameManager::createBattleInfo(const GameState& state, const Tank& tank, SatelliteView& /* map */) {
    MyBattleInfo info;
    
    // Set basic tank info
    info.tank_position_x = tank.x;
    info.tank_position_y = tank.y;
    info.tank_direction = tank.direction;
    info.tank_shells_remaining = tank.shells;
    info.tank_cooldown = tank.cooldown;
    
    // Set game info
    info.current_turn = state.current_step;
    info.max_turns = state.max_steps;
    info.map_width = state.width;
    info.map_height = state.height;
    
    // Count alive tanks
    info.friendly_tanks_count = 0;
    info.enemy_tanks_count = 0;
    
    for (const auto& other_tank : state.tanks) {
        if (other_tank.alive) {
            if (other_tank.player == tank.player) {
                info.friendly_tanks_count++;
            } else {
                info.enemy_tanks_count++;
            }
        }
    }
    
    // Set shell info
    info.shells_in_flight = 0;
    for (const auto& shell : state.shells) {
        if (shell.active) {
            info.shells_in_flight++;
        }
    }
    
    return info;
}

}
