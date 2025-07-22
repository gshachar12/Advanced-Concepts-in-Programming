#include "VisualizingGameManager.h"
#include "CellType.h"
#include <iostream>
#include <iomanip>

VisualizingGameManager::VisualizingGameManager(bool verbose) : GameManager(verbose) {}

GameResult VisualizingGameManager::run(
    size_t map_width, 
    size_t map_height,
    SatelliteView& map,
    size_t max_steps, 
    size_t num_shells,
    Player& player1, 
    Player& player2,
    TankAlgorithmFactory player1_tank_algo_factory,
    TankAlgorithmFactory player2_tank_algo_factory) {
    
    std::cout << "\n=== VISUALIZATION MODE ENABLED ===\n";
    std::cout << "Press ENTER after each turn to continue...\n\n";
    
    // Show initial board state using the satellite view
    std::cout << "=== Initial Board State ===\n";
    displayBoardFromSatelliteView(map, map_width, map_height);
    pauseForVisualization();
    
    // Initialize the game using parent's initialization
    if (!initializeGameFromMap(map, map_width, map_height, num_shells)) {
        GameResult failed_result;
        failed_result.winner = 0;
        failed_result.reason = GameResult::MAX_STEPS; // Use existing enum value
        return failed_result;
    }
    
    setupTankAlgorithms(player1_tank_algo_factory, player2_tank_algo_factory);
    updateTanksWithBattleInfo(player1, player2);
    
    // Visual game loop - show state after each turn
    for (size_t turn = 1; turn <= max_steps; ++turn) {
        std::cout << "\n=== Turn " << turn << " ===\n";
        
        // Execute one complete turn
        executeTurn(player1, player2);
        
        // Show rich emoji board state after this turn
        std::cout << "\n=== After Turn " << turn << " ===\n";
        displayRichBoard();
        
        // Show detailed game information like Project 2
        if (game_state_) {
            printTankStatus();
            printShellStatus();
            printGameSummary();
        }
        
        pauseForVisualization();
        
        // Check if game has ended
        if (checkGameEndConditions()) {
            GameResult result = generateGameResult();
            if (verbose_mode_) {
                std::cout << "\n[INFO] Game ended after turn " << turn << "\n";
            }
            std::cout << "\n=== Final Board State ===\n";
            displayRichBoard();
            
            // Show final detailed information
            if (game_state_) {
                printTankStatus();
                printShellStatus();
                printGameSummary();
            }
            
            return result;
        }
    }
    
    // Game ended due to max steps reached
    GameResult result = generateGameResult();
    if (verbose_mode_) {
        std::cout << "\n[INFO] Game ended: Maximum steps (" << max_steps << ") reached\n";
    }
    std::cout << "\n=== Final Board State ===\n";
    displayRichBoard();
    
    // Show final detailed information
    if (game_state_) {
        printTankStatus();
        printShellStatus();
        printGameSummary();
    }
    
    return result;
}

void VisualizingGameManager::executeVisualTurn(Player& player1, Player& player2, size_t turn) {
    if (verbose_mode_) {
        std::cout << "\n=== Turn " << turn << " ===\n";
    }
    
    // Execute the actual turn logic (this would need parent method access)
    // For now, just display the game state
    displayGame(turn);
}

void VisualizingGameManager::displayGame(size_t turn) {
    clearScreen();
    std::cout << "=== Game State (Visual Mode) - Turn: " << turn << " ===\n\n";
    
    if (game_state_) {
        initVisualBoard();
        overlayShells();
        overlayTanks();
        printBoard();
        printTankStatus();
        printShellStatus();
        printGameSummary();
    }
    
    pauseForVisualization();
}

void VisualizingGameManager::initVisualBoard() {
    if (!game_state_) return;
    
    const Board& board = game_state_->getBoard();
    size_t width = board.getWidth();
    size_t height = board.getHeight();
    
    visualBoard = std::vector<std::vector<std::string>>(
        height, std::vector<std::string>(width, "�"));
    
    for (size_t y = 0; y < height; ++y) {
        for (size_t x = 0; x < width; ++x) {
            CellType cell = board.getCellType(x, y);
            visualBoard[y][x] = getCellEmojiRich(cell);
        }
    }
}

void VisualizingGameManager::overlayShells() {
    if (!game_state_) return;
    
    auto shells = game_state_->getAllShells();
    for (const Shell* shell : shells) {
        if (shell && shell->isActive()) {
            Position pos = shell->getPosition();
            if (pos.x >= 0 && pos.x < static_cast<int>(visualBoard[0].size()) &&
                pos.y >= 0 && pos.y < static_cast<int>(visualBoard.size())) {
                visualBoard[pos.y][pos.x] = "🚀";  // Project 2 emoji shell
            }
        }
    }
}

void VisualizingGameManager::overlayTanks() {
    if (!game_state_) return;
    
    auto tanks = game_state_->getAllTanks();
    for (const Tank* tank : tanks) {
        if (tank && tank->isAlive()) {
            Position pos = tank->getPosition();
            if (pos.x >= 0 && pos.x < static_cast<int>(visualBoard[0].size()) &&
                pos.y >= 0 && pos.y < static_cast<int>(visualBoard.size())) {
                
                // Use Project 2-style tank emojis with direction
                Direction dir = tank->getDirection();
                std::string tankEmoji = getTankEmojiWithDirection(tank->getPlayerId(), dir);
                
                visualBoard[pos.y][pos.x] = tankEmoji;
            }
        }
    }
}

void VisualizingGameManager::printBoard() {
    for (const auto &row : visualBoard) {
        for (const auto &cell : row) {
            std::cout << cell;
        }
        std::cout << '\n';
    }
}

void VisualizingGameManager::printTankStatus() {
    if (!game_state_) return;
    
    std::cout << "\nTank Status:\n";
    auto tanks = game_state_->getAllTanks();
    
    for (const Tank* tank : tanks) {
        if (!tank->isAlive()) {
            std::cout << "X Tank " << tank->getPlayerId() 
                      << " (ID: " << tank->getTankId() << ") DESTROYED\n";
            continue;
        }
        
        Position pos = tank->getPosition();
        Direction dir = tank->getDirection();
        std::string symbol = getDirectionSymbol(dir);
        
        std::cout << getDirectionSymbol(dir) << " Tank " << tank->getPlayerId()
                  << " (ID: " << tank->getTankId() << ") at (" << pos.x << ", " << pos.y
                  << "), Direction: " << symbol
                  << ", Shells: " << tank->getShellsRemaining()
                  << ", Cooldown: " << tank->getShootCooldown() << "\n";
    }
}

void VisualizingGameManager::printShellStatus() {
    if (!game_state_) return;
    
    auto shells = game_state_->getAllShells();
    if (shells.empty()) {
        return;
    }
    
    std::cout << "\nShell Status:\n";
    
    for (const Shell* shell : shells) {
        if (!shell || !shell->isActive()) continue;
        
        Position pos = shell->getPosition();
        Direction dir = shell->getDirection();
        std::string symbol = getDirectionSymbolASCII(dir);
        
        std::cout << "🚀 Shell at (" << pos.x << ", " << pos.y
                  << "), Direction: " << getDirectionSymbol(dir) 
                  << ", Owner: Player " << shell->getOwnerPlayerId() << "\n";
    }
}

void VisualizingGameManager::printGameSummary() {
    if (!game_state_) return;
    
    auto p1_tanks = game_state_->getPlayerTanks(1);
    auto p2_tanks = game_state_->getPlayerTanks(2);
    
    int p1_alive = 0, p2_alive = 0;
    int p1_shells = 0, p2_shells = 0;
    
    for (const Tank* tank : p1_tanks) {
        if (tank->isAlive()) {
            p1_alive++;
            p1_shells += tank->getShellsRemaining();
        }
    }
    
    for (const Tank* tank : p2_tanks) {
        if (tank->isAlive()) {
            p2_alive++;
            p2_shells += tank->getShellsRemaining();
        }
    }
    
    auto active_shells = game_state_->getAllShells();
    int shells_in_flight = 0;
    for (const Shell* shell : active_shells) {
        if (shell && shell->isActive()) shells_in_flight++;
    }
    
    std::cout << "\nGame Summary:\n";
    std::cout << "Player 1: " << p1_alive << " tanks, " << p1_shells << " shells\n";
    std::cout << "Player 2: " << p2_alive << " tanks, " << p2_shells << " shells\n";
    std::cout << "Shells in flight: " << shells_in_flight << "\n";
    std::cout << "Turn: " << game_state_->getCurrentStep() << "/" << game_state_->getMaxSteps() << "\n";
}

void VisualizingGameManager::pauseForVisualization() {
    std::cout << "\nPress ENTER to continue...";
    std::cin.get();
}

void VisualizingGameManager::displayBoardFromSatelliteView(const SatelliteView& map, size_t width, size_t height) {
    // Print column numbers
    std::cout << "   ";
    for (size_t x = 0; x < width; ++x) {
        std::cout << (x % 10);
    }
    std::cout << "\n";
    
    // Print board with emojis
    for (size_t y = 0; y < height; ++y) {
        std::cout << (y % 10) << "  ";
        for (size_t x = 0; x < width; ++x) {
            char cell = map.getObjectAt(x, y);
            std::cout << getEmojiForChar(cell);
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

void VisualizingGameManager::displayRichBoard() {
    if (!game_state_) {
        std::cout << "[ERROR] Game state not available for rich visualization\n";
        return;
    }
    
    const Board& board = game_state_->getBoard();
    size_t width = board.getWidth();
    size_t height = board.getHeight();
    
    // Print column numbers
    std::cout << "   ";
    for (size_t x = 0; x < width; ++x) {
        std::cout << (x % 10);
    }
    std::cout << "\n";
    
    // Create visual board with Project 2-style emojis
    initVisualBoard();
    overlayShells();
    overlayTanks();
    
    // Print the rich emoji board
    for (size_t y = 0; y < height; ++y) {
        std::cout << (y % 10) << "  ";
        for (size_t x = 0; x < width; ++x) {
            if (y < visualBoard.size() && x < visualBoard[y].size()) {
                std::cout << visualBoard[y][x];
            } else {
                std::cout << "⬜";
            }
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

void VisualizingGameManager::clearScreen() {
    std::cout << "\033[2J\033[1;1H"; // ANSI escape codes to clear screen
}

std::string VisualizingGameManager::getDirectionSymbol(Direction dir) {
    switch (dir) {
        case Direction::UP: return "↑";
        case Direction::UP_RIGHT: return "↗";
        case Direction::RIGHT: return "→";
        case Direction::DOWN_RIGHT: return "↘";
        case Direction::DOWN: return "↓";
        case Direction::DOWN_LEFT: return "↙";
        case Direction::LEFT: return "←";
        case Direction::UP_LEFT: return "↖";
        default: return "?";
    }
}

std::string VisualizingGameManager::getDirectionSymbolASCII(Direction dir) {
    switch (dir) {
        case Direction::UP: return "^";
        case Direction::UP_RIGHT: return "/";
        case Direction::RIGHT: return ">";
        case Direction::DOWN_RIGHT: return "\\";
        case Direction::DOWN: return "v";
        case Direction::DOWN_LEFT: return "/";
        case Direction::LEFT: return "<";
        case Direction::UP_LEFT: return "\\";
        default: return "?";
    }
}

std::string VisualizingGameManager::getCellEmojiRich(CellType cell) {
    switch (cell) {
        case CellType::WALL: return "🧱";  // Brick wall emoji
        case CellType::WEAK_WALL: return "🟫";  // Brown square for weak wall  
        case CellType::MINE: return "💣";  // Bomb emoji for mine
        case CellType::EXPLOSION: return "💥";  // Explosion emoji
        case CellType::SHELL: return "🚀";  // Rocket emoji for shell
        case CellType::EMPTY: return "🟩";  // Green square for empty
        case CellType::TANK1: return "🟦";  // Blue square (will be overlaid)
        case CellType::TANK2: return "🟦";  // Blue square (will be overlaid)
        default: return "🟩";   // Default green
    }
}

std::string VisualizingGameManager::getTankEmojiWithDirection(int playerId, Direction dir) {
    // Tank base emoji depends on player
    std::string tankBase = (playerId == 1) ? "🟦" : "🟥";  // Blue for P1, Red for P2
    
    // Add directional arrow
    std::string arrow = getDirectionSymbol(dir);
    
    // For Project 2 style, we can use tank emoji with direction
    switch (dir) {
        case Direction::UP: return (playerId == 1) ? "⬆️" : "⬆️";
        case Direction::UP_RIGHT: return (playerId == 1) ? "↗️" : "↗️";
        case Direction::RIGHT: return (playerId == 1) ? "➡️" : "➡️";
        case Direction::DOWN_RIGHT: return (playerId == 1) ? "↘️" : "↘️";
        case Direction::DOWN: return (playerId == 1) ? "⬇️" : "⬇️";
        case Direction::DOWN_LEFT: return (playerId == 1) ? "↙️" : "↙️";
        case Direction::LEFT: return (playerId == 1) ? "⬅️" : "⬅️";
        case Direction::UP_LEFT: return (playerId == 1) ? "↖️" : "↖️";
        default: return (playerId == 1) ? "🔵" : "🔴";
    }
}

std::string VisualizingGameManager::getEmojiForChar(char cell) {
    switch (cell) {
        case '#': return "🧱";  // Brick wall emoji 
        case '=': return "🟫";  // Brown square for weak wall
        case '@': return "💣";  // Bomb emoji for mine
        case 'X': return "💥";  // Explosion emoji
        case '*': return "🚀";  // Rocket emoji for shell
        case '1': return "⬆️";  // Player 1 tank (default up direction)
        case '2': return "⬆️";  // Player 2 tank (default up direction)
        case ' ': return "🟩";  // Green square for empty
        default: return "🟩";   // Default green
    }
}
