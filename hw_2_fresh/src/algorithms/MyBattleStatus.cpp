#include "MyBattleStatus.h"

#include "Direction.h"

/**
 * @brief Constructor for MyBattleStatus
 * 
 * Initializes a battle status object with the player ID and tank index.
 * Sets the initial tank direction based on the player ID:
 * - Player 1 starts facing LEFT
 * - Player 2 starts facing RIGHT
 * 
 * @param player_id ID of the player (1 or 2)
 * @param tank_index Index of the tank for this player
 */
MyBattleStatus::MyBattleStatus(int player_id, int tank_index): tank_direction(
                                                                   player_id == 1
                                                                       ? Direction::LEFT
                                                                       : Direction::RIGHT),
                                                               player_id(player_id),
                                                               tank_index(tank_index) {
    updateTanksPosition();
}

/**
 * @brief Updates the game board information
 * 
 * Updates the internal board state with the new provided board,
 * recalculates the board dimensions, and refreshes tank positions.
 * 
 * @param updated_board The new board state to update with
 */
void MyBattleStatus::updateBoard(const std::vector<std::vector<char> > &updated_board) {
    board = updated_board;
    board_x = board.size();
    board_y = board[0].size();
    updateTanksPosition();
}

/**
 * @brief Returns the current tank position
 * 
 * @return Position The current position of the tank
 */
Position MyBattleStatus::getTankPosition() const {
    return tank_position;
}

/**
 * @brief Checks if the tank has ammunition left
 * 
 * @return true If the tank has at least one shell remaining
 * @return false If the tank has no shells left
 */
bool MyBattleStatus::hasTankAmmo() const {
    return shells_count > 0;
}

/**
 * @brief Checks if the tank can shoot at this moment
 * 
 * The tank must have ammunition AND its cooldown must be 0
 * 
 * @return true If the tank can shoot
 * @return false If the tank cannot shoot (no ammo or on cooldown)
 */
bool MyBattleStatus::canTankShoot() const {
    return hasTankAmmo() && cool_down == 0;
}

/**
 * @brief Gets the character at the specified board coordinates
 * 
 * @param x X-coordinate on the board
 * @param y Y-coordinate on the board
 * @return char The character at the specified position
 */
char MyBattleStatus::getBoardItem(int x, int y) const {
    return board[x][y];
}

/**
 * @brief Gets the character at the specified position
 * 
 * @param p Position object containing x and y coordinates
 * @return char The character at the specified position
 */
char MyBattleStatus::getBoardItem(Position p) const {
    return getBoardItem(p.x, p.y);
}

/**
 * @brief Wraps a position around the board edges (handles board wraparound)
 * 
 * Ensures that coordinates stay within the board boundaries.
 * If a position goes beyond the board edges, it wraps around to the other side.
 * 
 * @param p Position to update/wrap
 * @return Position The updated position after wrapping around the board
 */
Position MyBattleStatus::updatePosition(Position p) const {
    return {p.x % board_x, p.y % board_y};
}

/**
 * @brief Calculates the shortest distance between two positions on the board
 * 
 * Accounts for board wraparound, calculating the shortest path between
 * two positions (which might go around the edges of the board).
 * Uses the max norm (Chebyshev distance) - the maximum of dx and dy.
 * 
 * @param a First position
 * @param b Second position
 * @return size_t The shortest distance between positions
 */
size_t MyBattleStatus::getDistance(const Position &a, const Position &b) const {
    int raw_dx = std::abs(static_cast<int>(a.x) - static_cast<int>(b.x));
    int raw_dy = std::abs(static_cast<int>(a.y) - static_cast<int>(b.y));

    size_t dx = std::min(static_cast<size_t>(raw_dx), board_x - static_cast<size_t>(raw_dx));
    size_t dy = std::min(static_cast<size_t>(raw_dy), board_y - static_cast<size_t>(raw_dy));

    return std::max(dx, dy);
}

/**
 * @brief Checks if the tank can hit an enemy when facing a specific direction
 * 
 * Determines if there's a clear line of sight to any enemy or shell (if include_shells is true)
 * when the tank is facing the specified direction.
 * 
 * @param dir The direction to check for targets
 * @param include_shells Whether to consider shells as valid targets
 * @return true If an enemy or shell can be hit when facing the given direction
 * @return false If no enemy or shell can be hit when facing the given direction
 */
bool MyBattleStatus::canTankHitEnemy(Direction::DirectionType dir, bool include_shells) const {
    if (enemy_positions.empty() || !canTankShoot()) {
        return false;
    }

    for (Position enemy: enemy_positions) {
        if (isTargetOnSight(dir, enemy)) {
            return true;
        }
    }
    if (include_shells) {
        for (Position shell: shells_position) {
            if (isTargetOnSight(dir, shell)) {
                return true;
            }
        }
    }
    return false;
}

/**
 * @brief Checks if the tank can hit a specific enemy position
 * 
 * Determines if there's a clear line of sight to the specified enemy position
 * when the tank is facing its current direction.
 * 
 * @param enemy The position of the enemy to check
 * @return true If the specific enemy can be hit
 * @return false If the specific enemy cannot be hit
 */
bool MyBattleStatus::canTankHitEnemy(Position enemy) const {
    if (!canTankShoot()) {
        return false;
    }
    return isTargetOnSight(tank_direction, enemy);
}

/**
 * @brief Checks if the tank can hit any enemy in its current direction
 * 
 * Convenience method that checks if any enemy (or shell if include_shells is true)
 * is in the line of fire of the tank's current direction.
 * 
 * @param include_shells Whether to consider shells as valid targets
 * @return true If an enemy or shell can be hit in the current direction
 * @return false If no enemy or shell can be hit in the current direction
 */
bool MyBattleStatus::canTankHitEnemy(bool include_shells) const {
    return canTankHitEnemy(tank_direction, include_shells);
}

/**
 * @brief Checks if a target is in the line of sight from the tank's position
 * 
 * Traces a line from the tank's position in the specified direction and checks
 * if the target is reached before any obstacle (wall or ally).
 * 
 * @param dir Direction to look in
 * @param target Position of the target to check
 * @return true If the target can be seen and hit from the tank's position
 * @return false If an obstacle blocks the line of sight or the target is not in that direction
 */
bool MyBattleStatus::isTargetOnSight(Direction::DirectionType dir, Position target) const {
    Position p = tank_position;
    for (size_t i = 1; i <= std::max(board_x, board_y); i++) {
        p = updatePosition(p + dir);
        if (target == p) return true;
        if (board[p.x][p.y] == boardItemToChar(BoardItem::WALL) || board[p.x][p.y] == getAllyName()) {
            return false;
        }
    }
    return false;
}


/**
 * @brief Determines if a position is safe for the tank to occupy
 * 
 * A position is considered safe if:
 * 1. The position is empty (no wall, tank, etc.)
 * 2. If immediate_safe is false, also checks there are no nearby shells or enemies
 * 
 * @param p Position to check for safety
 * @param immediate_safe If true, only checks if the position is empty
 * @return true If the position is considered safe
 * @return false If the position is not safe
 */
bool MyBattleStatus::isSafePosition(Position p, const bool immediate_safe) const {
    if (board[p.x][p.y] != boardItemToChar(BoardItem::EMPTY)) {
        return false;
    }

    if (immediate_safe) {
        return true;
    }

    return !(isShellClose(p) || isEnemyClose(p));
}

/**
 * @brief Gets a list of safe directions to move from a position
 * 
 * Checks all possible directions and returns those that lead to safe positions.
 * A direction is considered safe if moving one step in that direction
 * results in a safe position (as determined by isSafePosition).
 * 
 * @param position The starting position
 * @return std::vector<Direction::DirectionType> List of directions that lead to safe positions
 */
std::vector<Direction::DirectionType> MyBattleStatus::getSafeDirections(const Position position) const {
    std::vector<Direction::DirectionType> safe_directions;

    for (int i = 0; i < Direction::getDirectionSize(); i++) {
        auto direction = Direction::getDirectionFromIndex(i);
        Position next_position = updatePosition(position + direction);
        if (isSafePosition(next_position)) {
            safe_directions.push_back(direction);
        }
    }

    return safe_directions;
}

/**
 * @brief Determines the action needed to rotate the tank to face a specific direction
 * 
 * Calculates the shortest rotation path (clockwise or counter-clockwise)
 * to turn from the current direction to the desired direction.
 * Returns the appropriate rotation action for the shortest path.
 * 
 * @param to_direction The target direction to face
 * @return ActionRequest The rotation action to take (or DoNothing if already facing that direction)
 */
ActionRequest MyBattleStatus::rotateTowards(Direction::DirectionType to_direction) const {
    if (tank_direction == to_direction) return ActionRequest::DoNothing;

    // Find the shortest rotation path
    if (int diff = (tank_direction - to_direction + 360) % 360; diff > 180) {
        diff = 360 - diff;
        // Clockwise is shorter
        if (diff == 45) return ActionRequest::RotateRight45;
        if (diff == 90) return ActionRequest::RotateRight90;
        if (diff == 135) return ActionRequest::RotateRight90;
        if (diff == 180) return ActionRequest::RotateRight90;
    } else {
        // Counter-clockwise is shorter or equal
        if (diff == 45) return ActionRequest::RotateLeft45;
        if (diff == 90) return ActionRequest::RotateLeft90;
        if (diff == 135) return ActionRequest::RotateLeft90;
        if (diff == 180) return ActionRequest::RotateLeft90;
    }

    return ActionRequest::DoNothing;
}


/**
 * @brief Updates the positions of all tanks and shells on the board
 * 
 * Scans the entire board to find and track:
 * - The position of the current tank
 * - The positions of all allied tanks
 * - The positions of all enemy tanks
 * - The positions of all shells
 * 
 * This information is stored for quick access when making decisions.
 */
void MyBattleStatus::updateTanksPosition() {
    std::vector<Position> enemy;
    std::vector<Position> ally;
    std::vector<Position> shells;
    for (size_t i{0}; i < board.size(); i++) {
        for (size_t j{0}; j < board.front().size(); j++) {
            Position p = {i, j};

            if (board[i][j] == getAllyName()) {
                ally.push_back(p);
            } else if (board[i][j] == boardItemToChar(BoardItem::CURRENT_TANK)) {
                tank_position = p;
            } else if (board[i][j] == getEnemyName()) {
                enemy.push_back(p);
            } else if (board[i][j] == boardItemToChar(BoardItem::SHELL)) {
                shells.push_back(p);
            }
        }
    }
    ally_positions = ally;
    enemy_positions = enemy;
    shells_position = shells;
}

/**
 * @brief Gets the character representing enemy tanks on the board
 * 
 * @return char '2' for player 1's enemies, '1' for player 2's enemies
 */
char MyBattleStatus::getEnemyName() const {
    (void) tank_index;  // Suppress unused parameter warning
    return (player_id == 1) ? boardItemToChar(BoardItem::TANK_PLAYER_2) : boardItemToChar(BoardItem::TANK_PLAYER_1);
}

/**
 * @brief Gets the character representing ally tanks on the board
 * 
 * @return char '1' for player 1's allies, '2' for player 2's allies
 */
char MyBattleStatus::getAllyName() const {
    return (player_id == 1) ? boardItemToChar(BoardItem::TANK_PLAYER_1) : boardItemToChar(BoardItem::TANK_PLAYER_2);
}

/**
 * @brief Gets the positions of all enemy tanks
 * 
 * @return std::vector<Position> List of all enemy tank positions
 */
std::vector<Position> MyBattleStatus::getEnemyPositions() const {
    return enemy_positions;
}

/**
 * @brief Gets the number of enemy tanks on the board
 * 
 * @return size_t The count of enemy tanks
 */
size_t MyBattleStatus::getEnemyTankCounts() const {
    return enemy_positions.size();
}

/**
 * @brief Updates the tank's direction based on the action taken
 * 
 * Uses the Direction utility to calculate the new direction after a rotation action.
 * 
 * @param action The action taken by the tank (rotations affect direction)
 */
void MyBattleStatus::updateTankDirectionBaseAction(const ActionRequest action) {
    tank_direction = Direction::getDirectionAfterAction(tank_direction, action);
}

/**
 * @brief Updates the battle status based on the action taken
 * 
 * Handles various state changes:
 * - If shooting, sets cooldown and reduces ammo
 * - Otherwise, decreases cooldown and updates direction
 * 
 * @param action The action taken by the tank
 */
void MyBattleStatus::updateBattleStatusBaseAction(ActionRequest action) {
    if (action == ActionRequest::Shoot) {
        cool_down = 4;
        shells_count--;
        return;
    }
    cool_down = (cool_down == 0) ? 0 : cool_down - 1;
    updateTankDirectionBaseAction(action);
}

/**
 * @brief Checks if there are any shells close to a specific position
 * 
 * A shell is considered "close" if its distance is less than the threshold.
 * 
 * @param position The position to check
 * @param thresh Distance threshold (default: 6)
 * @return true If any shell is closer than the threshold
 * @return false If no shells are close to the position
 */
bool MyBattleStatus::isShellClose(Position position, size_t thresh) const {
    for (auto shellPosition: shells_position) {
        if (getDistance(position, shellPosition) < thresh) {
            return true;
        }
    }
    return false;
}

/**
 * @brief Checks if there are any enemy tanks close to a specific position
 * 
 * An enemy is considered "close" if its distance is less than or equal to 2.
 * 
 * @param position The position to check
 * @return true If any enemy is within distance 2
 * @return false If no enemies are close to the position
 */
bool MyBattleStatus::isEnemyClose(Position position) const {
    if (enemy_positions.empty()) {
        return false;
    }
    for (auto enemy_position: enemy_positions) {
        if (getDistance(position, enemy_position) <= 2) {
            return true;
        }
    }
    return false;
}

/**
 * @brief Checks if there are any shells close to the current tank position
 * 
 * This is a convenience method that checks if any shells are close to the
 * current tank's position. Since shells' directions are unknown, we consider
 * shells threatening if they're within the threshold distance.
 * 
 * @param thresh Distance threshold (default: 6)
 * @return true If any shell is within the threshold distance
 * @return false If no shells are close to the current tank
 */
bool MyBattleStatus::isShellClose(size_t thresh) const {
    return isShellClose(tank_position, thresh);
}

/**
 * @brief Checks if there are any enemy tanks close to the current tank position
 * 
 * This is a convenience method that checks if any enemy tanks are close
 * to the current tank's position.
 * 
 * @return true If any enemy tank is close to the current tank
 * @return false If no enemy tanks are close to the current tank
 */
bool MyBattleStatus::isEnemyClose() const {
    return isEnemyClose(tank_position);
}
