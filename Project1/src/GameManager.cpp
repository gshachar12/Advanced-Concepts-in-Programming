#include "GameManager.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <algorithm>

// Dummy decideAction function.
// Replace this with a call to your Controller's decideAction method when ready.
ActionType dummyDecideAction(const Board &board, const Tank &myTank,
                             const Tank &enemyTank, const std::vector<Shell> &shells) {
    return ActionType::MOVE_FORWARD;
}

bool GameManager::initializeGame(const std::string &boardFile) {
    if (!board.loadFromFile(boardFile)) {
        std::cerr << "Error: Failed to load board from " << boardFile << std::endl;
        return false;
    }
    // For a minimal example, set fixed starting positions.
    tank1.setPosition(2, 2);
    tank2.setPosition(board.getWidth() - 3, board.getHeight() - 3);
    return true;
}

void GameManager::runGameLoop() {
    turnCount = 0;
    // In text mode (visualMode == false) we do not display each turn.
    while (!gameOver && turnCount < 1000) {
        turnCount++;
        std::cout << "Turn " << turnCount << std::endl;

        // Update both tanks' internal statuses (cooldowns, backward movement, etc.)
        tank1.update();
        tank2.update();

        // Get actions from controllers. For now, we use a dummy decision function.
        ActionType action1 = dummyDecideAction(board, tank1, tank2, shells);
        ActionType action2 = dummyDecideAction(board, tank2, tank1, shells);

        applyAction(tank1, action1, 1);
        applyAction(tank2, action2, 2);

        updateShells();
        checkCollisions();
        checkEndGameConditions();

        // Only display intermediate game state if visualMode is enabled.
        if (visualMode) {
            displayGame();
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
        }
    }

    // Final summary regardless of mode.
    std::cout << "=== Game Summary ===" << std::endl;
    if (!tank1.isAlive() && !tank2.isAlive()) {
        std::cout << "Tie: both tanks destroyed." << std::endl;
    } else if (!tank1.isAlive()) {
        std::cout << "Player 2 wins!" << std::endl;
    } else if (!tank2.isAlive()) {
        std::cout << "Player 1 wins!" << std::endl;
    } else {
        std::cout << "Game ended without a winner." << std::endl;
    }
}


void GameManager::applyAction(Tank &tank, ActionType action, int playerID) {
    switch (action) {
        case ActionType::MOVE_FORWARD:
            tank.moveForward();
            break;
        case ActionType::MOVE_BACKWARD:
            tank.requestBackward();
            break;
        case ActionType::SHOOT:
            if (tank.canShoot()) {
                tank.shoot();
                // Create a new shell at the tank's current position using its current Direction.
                // Note: tank.getDirection() returns a Direction.
                Shell newShell(tank.getX(), tank.getY(), tank.getDirection());
                shells.push_back(newShell);
            }
            break;
        default:
            break;
    }
    std::cout << "Player " << playerID << " action applied." << std::endl;
}


void GameManager::updateShells() {
    // Update each active shell.
    for (auto &shell : shells) {
        if (shell.isActive())
            shell.update();
    }
    // Remove shells that have been deactivated.
    shells.erase(std::remove_if(shells.begin(), shells.end(),
                                [](const Shell &s) { return !s.isActive(); }),
                 shells.end());
}

// Enhanced collision checking.
void GameManager::checkCollisions() {
    // 1. Shell vs. Wall Collision:
    // For each active shell, if its current cell on the board is a wall, weaken the wall and deactivate the shell.
    for (auto &shell : shells) {
        if (!shell.isActive())
            continue;
        int sx = shell.getX();
        int sy = shell.getY();
        CellType cell = board.getCellType(sx, sy);
        if (cell == CellType::WALL) {
            board.weakenWall(sx, sy);
            shell.deactivate();
            std::cout << "Shell hit a wall at (" << sx << ", " << sy << ") and deactivated." << std::endl;
        }
        // Note: Shells do not interact with mines.
    }

    // 2. Shell vs. Shell Collision:
    // Check all unique pairs of shells for collision.
    for (size_t i = 0; i < shells.size(); ++i) {
        for (size_t j = i + 1; j < shells.size(); ++j) {
            if (shells[i].isActive() && shells[j].isActive() &&
                shells[i].getX() == shells[j].getX() &&
                shells[i].getY() == shells[j].getY()) {
                shells[i].deactivate();
                shells[j].deactivate();
                std::cout << "Shell collision at (" << shells[i].getX()
                          << ", " << shells[i].getY() << "). Both shells deactivated."
                          << std::endl;
            }
        }
    }

    // 3. Shell vs. Tank Collision:
    // For each active shell, if it occupies the same cell as a tank, destroy that tank and deactivate the shell.
    for (auto &shell : shells) {
        if (!shell.isActive())
            continue;
        int sx = shell.getX();
        int sy = shell.getY();
        if (sx == tank1.getX() && sy == tank1.getY()) {
            tank1.destroy();
            shell.deactivate();
            std::cout << "Player 1's tank destroyed by a shell at (" << sx << ", " << sy << ")." << std::endl;
        }
        if (sx == tank2.getX() && sy == tank2.getY()) {
            tank2.destroy();
            shell.deactivate();
            std::cout << "Player 2's tank destroyed by a shell at (" << sx << ", " << sy << ")." << std::endl;
        }
    }

    // 4. Tank vs. Mine Collision:
    // For each tank, check the board cell. If it's a mine, destroy the tank and remove the mine.
    {
        int tx = tank1.getX();
        int ty = tank1.getY();
        if (board.getCellType(tx, ty) == CellType::MINE) {
            tank1.destroy();
            board.setCellType(tx, ty, CellType::EMPTY);  // Remove the mine.
            std::cout << "Player 1's tank hit a mine at (" << tx << ", " << ty << ") and was destroyed." << std::endl;
        }
    }
    {
        int tx = tank2.getX();
        int ty = tank2.getY();
        if (board.getCellType(tx, ty) == CellType::MINE) {
            tank2.destroy();
            board.setCellType(tx, ty, CellType::EMPTY);  // Remove the mine.
            std::cout << "Player 2's tank hit a mine at (" << tx << ", " << ty << ") and was destroyed." << std::endl;
        }
    }

    // 5. Tank vs. Tank Collision:
    // If both tanks are in the same cell, destroy both.
    if (tank1.getX() == tank2.getX() && tank1.getY() == tank2.getY()) {
        tank1.destroy();
        tank2.destroy();
        std::cout << "Tanks collided at (" << tank1.getX() << ", " << tank1.getY() << "). Both destroyed." << std::endl;
    }
}

void GameManager::checkEndGameConditions() {
    if (!tank1.isAlive() || !tank2.isAlive()) {
        gameOver = true;
        return;
    }
    // If both tanks are out of shells, continue for 40 steps then end in a tie.
    if (tank1.getShellCount() == 0 && tank2.getShellCount() == 0) {
        stepsSinceBothAmmoZero++;
        if (stepsSinceBothAmmoZero >= 40) {
            gameOver = true;
            std::cout << "Tie: 40 steps elapsed with both tanks out of shells." << std::endl;
        }
    } else {
        stepsSinceBothAmmoZero = 0;
    }
}

void GameManager::displayGame() {
    // Prepare a temporary grid based on the board dimensions.
    std::vector<std::vector<char>> displayGrid(board.getHeight(), std::vector<char>(board.getWidth(), ' '));
    for (int y = 0; y < board.getHeight(); ++y) {
        for (int x = 0; x < board.getWidth(); ++x)
            displayGrid[y][x] = ' ';  // default empty cell placeholder.
    }
    // Overlay tanks if they are alive.
    if (tank1.isAlive())
        displayGrid[tank1.getY()][tank1.getX()] = '1';
    if (tank2.isAlive())
        displayGrid[tank2.getY()][tank2.getX()] = '2';
    // Overlay shells (using '*' to represent a shell).
    for (const auto &shell : shells) {
        if (shell.isActive()) {
            int sx = shell.getX();
            int sy = shell.getY();
            if (sx >= 0 && sx < board.getWidth() && sy >= 0 && sy < board.getHeight())
                displayGrid[sy][sx] = '*';
        }
    }

    // Print the grid using emojis if visualMode is enabled.
    if (visualMode) {
        std::cout << "=== Game State (Visual Mode) ===" << std::endl;
        for (const auto &row : displayGrid) {
            for (const auto &cell : row) {
                if (cell == '1')
                    std::cout << "🚗 ";
                else if (cell == '2')
                    std::cout << "🚙 ";
                else if (cell == '*')
                    std::cout << "💥 ";
                else
                    std::cout << "⬜ ";
            }
            std::cout << std::endl;
        }
    } else {
        // If not in visual mode, don't print intermediate states.
    }

    std::cout << std::endl;
}
