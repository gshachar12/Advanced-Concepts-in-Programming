#include "GameState.h"
#include <algorithm>

GameState::GameState(size_t map_width, size_t map_height, size_t max_steps, size_t shells_per_tank)
    : board_(std::make_unique<Board>(map_width, map_height)),
      max_steps_(max_steps), current_step_(0), 
      initial_shells_per_tank_(shells_per_tank), game_over_(false) {
    result_.winner = 0;
    result_.reason = GameResult::MAX_STEPS;
    result_.remaining_tanks.resize(2, 0);
}

void GameState::addTank(int player_id, const Position& pos, Direction dir) {
    if (player_id == 1) {
        int tank_id = static_cast<int>(player1_tanks_.size());
        player1_tanks_.push_back(std::make_unique<Tank>(pos, dir, player_id, tank_id));
    } else if (player_id == 2) {
        int tank_id = static_cast<int>(player2_tanks_.size());
        player2_tanks_.push_back(std::make_unique<Tank>(pos, dir, player_id, tank_id));
    }
}

std::vector<Tank*> GameState::getPlayerTanks(int player_id) {
    std::vector<Tank*> tanks;
    if (player_id == 1) {
        for (auto& tank : player1_tanks_) {
            tanks.push_back(tank.get());
        }
    } else if (player_id == 2) {
        for (auto& tank : player2_tanks_) {
            tanks.push_back(tank.get());
        }
    }
    return tanks;
}

std::vector<const Tank*> GameState::getPlayerTanks(int player_id) const {
    std::vector<const Tank*> tanks;
    if (player_id == 1) {
        for (const auto& tank : player1_tanks_) {
            tanks.push_back(tank.get());
        }
    } else if (player_id == 2) {
        for (const auto& tank : player2_tanks_) {
            tanks.push_back(tank.get());
        }
    }
    return tanks;
}

std::vector<Tank*> GameState::getAllTanks() {
    std::vector<Tank*> all_tanks;
    for (auto& tank : player1_tanks_) {
        all_tanks.push_back(tank.get());
    }
    for (auto& tank : player2_tanks_) {
        all_tanks.push_back(tank.get());
    }
    return all_tanks;
}

std::vector<const Tank*> GameState::getAllTanks() const {
    std::vector<const Tank*> all_tanks;
    for (const auto& tank : player1_tanks_) {
        all_tanks.push_back(tank.get());
    }
    for (const auto& tank : player2_tanks_) {
        all_tanks.push_back(tank.get());
    }
    return all_tanks;
}

void GameState::addShell(const Position& pos, Direction dir, int owner_player_id, int owner_tank_id) {
    active_shells_.push_back(std::make_unique<Shell>(pos, dir, owner_player_id, owner_tank_id));
}

std::vector<Shell*> GameState::getAllShells() {
    std::vector<Shell*> shells;
    for (auto& shell : active_shells_) {
        shells.push_back(shell.get());
    }
    return shells;
}

std::vector<const Shell*> GameState::getAllShells() const {
    std::vector<const Shell*> shells;
    for (const auto& shell : active_shells_) {
        shells.push_back(shell.get());
    }
    return shells;
}

void GameState::removeInactiveShells() {
    active_shells_.erase(
        std::remove_if(active_shells_.begin(), active_shells_.end(),
            [](const std::unique_ptr<Shell>& shell) {
                return !shell->isActive();
            }),
        active_shells_.end());
}

void GameState::setGameOver(int winner, GameResult::Reason reason) {
    game_over_ = true;
    result_.winner = winner;
    result_.reason = reason;
    result_.remaining_tanks[0] = getAlivePlayerTanks(1);
    result_.remaining_tanks[1] = getAlivePlayerTanks(2);
}

size_t GameState::getAlivePlayerTanks(int player_id) const {
    size_t count = 0;
    auto tanks = getPlayerTanks(player_id);
    for (const Tank* tank : tanks) {
        if (tank->isAlive()) {
            count++;
        }
    }
    return count;
}

size_t GameState::getTotalPlayerShells(int player_id) const {
    size_t total = 0;
    auto tanks = getPlayerTanks(player_id);
    for (const Tank* tank : tanks) {
        if (tank->isAlive()) {
            total += tank->getShellsRemaining();
        }
    }
    return total;
}

bool GameState::hasPlayerLost(int player_id) const {
    return getAlivePlayerTanks(player_id) == 0;
}

void GameState::updateBoardRepresentation() {
    // Clear previous tank and shell positions
    for (size_t y = 0; y < board_->getHeight(); ++y) {
        for (size_t x = 0; x < board_->getWidth(); ++x) {
            CellType cell = board_->getCellType(x, y);
            if (cell == CellType::TANK1 || cell == CellType::TANK2 || 
                cell == CellType::SHELL || cell == CellType::EXPLOSION) {
                board_->setCellType(x, y, CellType::EMPTY);
            }
        }
    }
    
    // Place tanks
    for (const Tank* tank : getAllTanks()) {
        if (tank->isAlive()) {
            board_->setCellType(tank->getPosition(), tank->getCellType());
        } else {
            board_->setCellType(tank->getPosition(), CellType::EXPLOSION);
        }
    }
    
    // Place shells
    for (const Shell* shell : getAllShells()) {
        if (shell->isActive()) {
            board_->setCellType(shell->getPosition(), CellType::SHELL);
        }
    }
}
