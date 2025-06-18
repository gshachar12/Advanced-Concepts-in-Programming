#include "GameManager.h"

#include <iostream>
#include <thread>
#include <chrono>
#include <map>
#include <unistd.h>
#include <termios.h>

#include "Logger.h"
#include "Mine.h"
#include "Shell.h"
#include "ActionRequest.h"
#include "InputParser.h"
#include "MySatelliteView.h"
#include "Direction.h"

using namespace std::chrono_literals;

void GameManager::readBoard(const std::string &file_name) {
    auto input_parser = InputParser();
    board = input_parser.parseInputFile(file_name);

    if (board == nullptr) {
        board = std::make_unique<Board>();
        std::cerr << "Can't parse file " << file_name << std::endl;
    }

    // create 2 players
    for (int i = 1; i <= 2; i++) {
        players.emplace_back(player_factory.create(i, board->getWidth(), board->getHeight(), board->getMaxSteps(),
                                                   board->getNumShells()));
    }

    for (auto [player_i, tank_i]: input_parser.getTanks()) {
        tanks.emplace_back(tank_algorithm_factory.create(player_i, tank_i));
        tank_status.push_back({false, ActionRequest::DoNothing, true, false});
    }
}

void GameManager::run() {
    checkDeaths(); //check if one of the player doesn't have any tanks
    
    // Display initial game state if in visual mode
    if (visual) {
        std::cout << "\033[2J\033[1;1H"; // Clear screen
        displayGame();
    }
    
    while (!isGameOver()) {
        processStep();
    }

    Logger::getInstance().logResult(getGameResult());
    if (visual) {
        std::cout << "\n" << getGameResult() << std::endl;
    }
}

void GameManager::updateCounters(Tank &tank, const ActionRequest action) {
    tank.decreaseShootingCooldown();
    const int back_counter = tank.getBackwardsCounter();

    if (back_counter == 2) {
        if (action == ActionRequest::MoveForward) {
            tank.setBackwardsCounter(3);
        } else {
            tank.setBackwardsCounter(back_counter - 1);
        }
    }

    if (back_counter == 1) {
        if (action == ActionRequest::MoveForward) {
            tank.setBackwardsCounter(3);
        } else {
            tank.setBackwardsCounter(back_counter - 1);
        }
    }
}

bool GameManager::tankAction(Tank &tank, const ActionRequest action) {
    bool result = false;
    const int back_counter = tank.getBackwardsCounter();
    updateCounters(tank, action);
    if (action == ActionRequest::DoNothing) return true;
    if (back_counter == 1 && action != ActionRequest::MoveForward) return moveBackward(tank);

    switch (action) {
        case ActionRequest::MoveForward:
            result = moveForward(tank);
            if (back_counter == 0) tank.setBackwardsCounter(3);
            break;
        case ActionRequest::MoveBackward:
            if (back_counter == 0) result = moveBackward(tank);
            else tank.setBackwardsCounter(back_counter - 1);
            break;
        case ActionRequest::RotateLeft45:
            result = rotate(tank, -45);
            break;
        case ActionRequest::RotateRight45:
            result = rotate(tank, 45);
            break;
        case ActionRequest::RotateLeft90:
            result = rotate(tank, -90);
            break;
        case ActionRequest::RotateRight90:
            result = rotate(tank, 90);
            break;
        case ActionRequest::Shoot:
            result = shoot(tank);
            break;
        case ActionRequest::GetBattleInfo:
            result = getBattleInfo(tank, tank.getPlayerIndex());
            break;
        default: ;
    }

    return result;
}

void GameManager::checkDeaths() {
    const bool firstDead = board->getPlayerAliveTanks(1).empty();
    const bool secondDead = board->getPlayerAliveTanks(2).empty();

    if (firstDead && secondDead) {
        winner = TIE;
        game_over = true;
        return;
    }
    if (firstDead) {
        winner = PLAYER_2;
        game_over = true;
        return;
    }
    if (secondDead) {
        winner = PLAYER_1;
        game_over = true;
        return;
    }

    if (empty_countdown == 0) {
        winner = TIE_AMMO;
        game_over = true;
        return;
    }

    if (game_step == board->getMaxSteps()) {
        winner = TIE_STEPS;
        game_over = true;
    }
}

bool GameManager::moveForward(Tank &tank) {
    if (const auto obj = board->getObjectAt(tank.getPosition() + tank.getDirection())) {
        if (obj->getSymbol() == '#') return false;
    }
    return board->moveObject(tank.getPosition(), tank.getDirection());
}

bool GameManager::moveBackward(Tank &tank) {
    if (const auto obj = board->getObjectAt(tank.getPosition() + tank.getDirection())) {
        if (obj->getSymbol() == '#') return false;
    }
    return board->moveObject(tank.getPosition(), -tank.getDirection());
}

bool GameManager::rotate(Tank &tank, const int turn) {
    const int new_direction = tank.getDirection() + turn;
    tank.setDirection(Direction::getDirection(new_direction));
    return true;
}

bool GameManager::shoot(Tank &tank) {
    if (tank.getAmmunition() == 0) {
        return false;
    }
    if (tank.getCooldown() != 0) {
        return false;
    }

    tank.decrementAmmunition();
    tank.setCooldown(3);
    board->placeObject(std::make_unique<Shell>(tank.getPosition() + tank.getDirection(), tank.getDirection(),
                                               tank.getId()));
    return true;
}

bool GameManager::getBattleInfo(const Tank &tank, const size_t player_i) {
    const int tank_algo_i = tank.getTankAlgoIndex();
    auto [x,y] = tank.getPosition();
    MySatelliteView satellite_view = this->satellite_view;
    satellite_view.setObjectAt(x, y, '%');
    players[player_i - 1]->updateTankWithBattleInfo(*tanks[tank_algo_i], satellite_view);
    return true;
}

void GameManager::updateSatelliteView() {
    satellite_view.setDimensions(board->getWidth(), board->getHeight());
    board->fillSatelliteView(satellite_view);
}

bool GameManager::allEmptyAmmo() const {
    if (!board) return true;
    for (const auto tank: board->getAliveTanks()) {
        if (tank->getAmmunition() != 0) return false;
    }
    return true;
}

void GameManager::tanksTurn() {
    for (const auto tank: board->getAliveTanks()) {
        const int i = tank->getTankAlgoIndex();
        const ActionRequest action = tanks[i]->getAction();
        const bool res = tankAction(*tank, action);
        tank_status[i] = {false, action, res, false};
    }

    if (empty_countdown == -1 && allEmptyAmmo()) {
        empty_countdown = max_steps_empty_ammo;
    }

    if (empty_countdown != -1) {
        empty_countdown--;
    }
}

void GameManager::shellsTurn() const {
    for (auto [id, shell]: board->getShells()) {
        board->moveObject(shell->getPosition(), shell->getDirection());
    }
}

void GameManager::processStep() {
    if (game_over) return;
    game_step++;

    updateSatelliteView();

    shellsTurn();
    board->finishMove();
    
    // Collect explosions for display
    for (auto [id, pos] : board->getCollisions()) {
        explosions.push_back(pos);
    }

    shellsTurn();
    tanksTurn();
    board->finishMove();
    
    // Collect new explosions after tank turn
    for (auto [id, pos] : board->getCollisions()) {
        explosions.push_back(pos);
    }

    if (visual) {
        displayGame();
    }

    checkDeaths();
    logStep();
}

std::string GameManager::getGameResult() const {
    std::string p1_tanks = std::to_string(board->getPlayerAliveTanks(1).size());
    std::string p2_tanks = std::to_string(board->getPlayerAliveTanks(2).size());
    std::string max_steps = std::to_string(board->getMaxSteps());
    std::string empty_ammo = std::to_string(max_steps_empty_ammo);

    switch (winner) {
        case TIE:
            return "Tie, both players have zero tanks";
        case TIE_STEPS:
            return "Tie, reached max steps = " + max_steps + ", player 1 has " + p1_tanks + " tanks, player 2 has " +
                   p2_tanks + " tanks";
        case TIE_AMMO:
            return "Tie, both players have zero shells for " + empty_ammo + " steps";
        case PLAYER_1:
            return "Player 1 won with " + p1_tanks + " tanks still alive";
        case PLAYER_2:
            return "Player 2 won with " + p2_tanks + " tanks still alive";
        default:
            return "Bad result";
    }
}

void GameManager::logStep() {
    for (const auto tank: board->getTanks()) {
        if (tank->isDestroyed()) {
            std::get<3>(tank_status[tank->getTankAlgoIndex()]) = true;
        }
    }

    Logger::getInstance().logActions(tank_status);

    // Update deaths
    for (size_t i = 0; i < tank_status.size(); i++) {
        if (std::get<3>(tank_status[i])) {
            std::get<0>(tank_status[i]) = true;
        }
    }
}



/**
 * Displays the current game state in a visual format with emojis and symbols.
 * This is the main visualization method that calls other display helper methods.
 */
void GameManager::displayGame() {
    if (!visual)
        return;

    std::cout << "\033[2J\033[1;1H"; // Clear screen
    std::cout << "=== Game State (Visual Mode) - Turn: " << game_step << " ===\n" << std::endl;

    initVisualBoard();
    overlayShells();
    overlayTanks();
    printBoard();
    printTankStatus();
    printShellStatus();
    printGameSummary();
    waitForVisualPause();
}

/**
 * Initializes the visual representation of the board with emojis for walls, mines, and empty spaces.
 * Also adds any explosion markers from the previous turn.
 */
void GameManager::initVisualBoard() {
    visual_board = std::vector<std::vector<std::string>>(
        board->getHeight(), std::vector<std::string>(board->getWidth(), "🟦"));

    for (int y = 0; y < board->getHeight(); ++y) {
        for (int x = 0; x < board->getWidth(); ++x) {
            Position pos{x, y};
            GameObject* obj = board->getObjectAt(pos);
            if (obj) {
                char symbol = obj->getSymbol();
                switch (symbol) {
                    case '#':
                        visual_board[y][x] = "🟩";
                        break;
                    case '=':
                        visual_board[y][x] = "🧱";
                        break;
                    case '@':
                        visual_board[y][x] = "💣";
                        break;
                }
            }
        }
    }
}

/**
 * Adds shell emojis to the visual board at the positions of active shells.
 */
void GameManager::overlayShells() {
    for (auto [id, shell_ptr] : board->getShells()) {
        Position pos = shell_ptr->getPosition();
        GameObject* obj = board->getObjectAt(pos);
        if (obj && obj->getSymbol() == '*') {
            visual_board[pos.y][pos.x] = "🚀";
        }
    }
}

/**
 * Adds tank representation to the visual board with directional arrows and player numbers.
 * Each tank is represented as a directional arrow followed by the player number.
 */
void GameManager::overlayTanks() {
    // Create a mapping of directions to symbols
    std::map<Direction::DirectionType, std::string> dirSymbols;
    dirSymbols[Direction::UP] = "↑";
    dirSymbols[Direction::UP_RIGHT] = "↗";
    dirSymbols[Direction::RIGHT] = "→";
    dirSymbols[Direction::DOWN_RIGHT] = "↘";
    dirSymbols[Direction::DOWN] = "↓";
    dirSymbols[Direction::DOWN_LEFT] = "↙";
    dirSymbols[Direction::LEFT] = "←";
    dirSymbols[Direction::UP_LEFT] = "↖";

    for (const auto tank : board->getTanks()) {
        if (tank->isDestroyed())
            continue;

        Position pos = tank->getPosition();
        int x = pos.x, y = pos.y;
        if (x >= 0 && x < board->getWidth() && y >= 0 && y < board->getHeight()) {
            Direction::DirectionType dir = tank->getDirection();
            std::string arrow = dirSymbols.count(dir) ? dirSymbols.at(dir) : "?";

            std::string playerNum = std::to_string(tank->getPlayerIndex());
            std::string coloredArrow;

            if (tank->getPlayerIndex() == 1) {
                coloredArrow = "\033[34m" + arrow + playerNum + "\033[0m";  // Blue
            } else {
                coloredArrow = "\033[31m" + arrow + playerNum + "\033[0m";  // Red
            }
            visual_board[y][x] = arrow + playerNum;
        }
    }
}

/**
 * Prints the visual board to the console.
 */
void GameManager::printBoard() {
    for (const auto &row : visual_board) {
        for (const auto &cell : row)
            std::cout << cell;
        std::cout << '\n';
    }
}

/**
 * Prints detailed status for each tank, including position, direction, ammo, and cooldown.
 * Destroyed tanks are marked with an X.
 */
void GameManager::printTankStatus() {
    std::cout << "\nTank Status:\n";
    for (const auto tank : board->getTanks()) {
        if (tank->isDestroyed()) {
            std::cout << "✖️ Tank " << tank->getPlayerIndex() << " (ID: " << tank->getTankIndex() << ") DESTROYED\n";
            continue;
        }

        std::map<Direction::DirectionType, std::string> dirSymbols;
        dirSymbols[Direction::UP] = "↑";
        dirSymbols[Direction::UP_RIGHT] = "↗";
        dirSymbols[Direction::RIGHT] = "→";
        dirSymbols[Direction::DOWN_RIGHT] = "↘";
        dirSymbols[Direction::DOWN] = "↓";
        dirSymbols[Direction::DOWN_LEFT] = "↙";
        dirSymbols[Direction::LEFT] = "←";
        dirSymbols[Direction::UP_LEFT] = "↖";

        Direction::DirectionType dir = tank->getDirection();
        std::string symbol = dirSymbols.count(dir) ? dirSymbols.at(dir) : "?";
        Position pos = tank->getPosition();

        std::cout << symbol << " Tank " << tank->getPlayerIndex()
                  << " (ID: " << tank->getTankIndex() << ") at (" << pos.x << ", " << pos.y
                  << "), Direction: " << symbol
                  << ", Shells: " << tank->getAmmunition()
                  << ", Cooldown: " << tank->getCooldown() << "\n";
    }
}

/**
 * Prints information about the active shells in the game, including their positions and directions.
 */
void GameManager::printShellStatus() {
    auto shells = board->getShells();
    if (shells.empty())
        return;

    std::cout << "\nShell Status:\n";

    std::map<Direction::DirectionType, std::string> dirSymbols;
    dirSymbols[Direction::UP] = "↑";
    dirSymbols[Direction::UP_RIGHT] = "↗";
    dirSymbols[Direction::RIGHT] = "→";
    dirSymbols[Direction::DOWN_RIGHT] = "↘";
    dirSymbols[Direction::DOWN] = "↓";
    dirSymbols[Direction::DOWN_LEFT] = "↙";
    dirSymbols[Direction::LEFT] = "←";
    dirSymbols[Direction::UP_LEFT] = "↖";

    for (auto [id, shell_ptr] : shells) {
        if (!shell_ptr || shell_ptr->getSymbol() != '*')
            continue;

        Position pos = shell_ptr->getPosition();
        Direction::DirectionType dir = shell_ptr->getDirection();
        std::string symbol = dirSymbols.count(dir) ? dirSymbols.at(dir) : "?";

        // We don't have a direct way to get owner like in Project2, so we just display the direction

        std::cout << "🚀 Shell at (" << pos.x << ", " << pos.y
                  << "), Direction: " << symbol << "\n";
    }
}

/**
 * Prints a summary of the current game state, including the number of tanks and shells 
 * for each player, as well as steps without ammunition if applicable.
 */
void GameManager::printGameSummary() {
    int p1Tanks = board->getPlayerAliveTanks(1).size();
    int p2Tanks = board->getPlayerAliveTanks(2).size();
    
    // Count shells
    int p1Shells = 0, p2Shells = 0;
    for (auto tank : board->getPlayerAliveTanks(1)) {
        p1Shells += tank->getAmmunition();
    }
    
    for (auto tank : board->getPlayerAliveTanks(2)) {
        p2Shells += tank->getAmmunition();
    }

    std::cout << "\nGame Summary:\n";
    std::cout << "Player 1 Artillery: " << p1Tanks << " tanks, " << p1Shells << " shells\n";
    std::cout << "Player 2 Artillery: " << p2Tanks << " tanks, " << p2Shells << " shells\n";

    if (empty_countdown > 0)
        std::cout << "Steps without shells: " << (max_steps_empty_ammo - empty_countdown) << "/" << max_steps_empty_ammo << "\n";

    if (game_over)
        std::cout << "\n🏁 GAME OVER: " << getGameResult() << "\n";
}

/**
 * Pauses the game until the user presses a key.
 * This allows the user to review the current state before proceeding to the next step.
 * Uses terminal settings to capture a single keypress without requiring Enter.
 */
void GameManager::waitForVisualPause() {
    std::cout << "\nPress any key to continue to the next step...\n";

    termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt); // save old settings
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);        // disable buffering and echo
    tcsetattr(STDIN_FILENO, TCSANOW, &newt); // apply new settings

    getchar(); // wait for key

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // restore old settings
}