#ifndef BATTLEUTILS_H
#define BATTLEUTILS_H
#include <vector>
#include "Direction.h"

/**
 * @class MyBattleStatus
 * @brief Maintains and processes the current state of the battle
 * 
 * This class tracks the game state including board configuration, tank positions,
 * ammunition, cooldown timers, and provides utilities for determining safe moves,
 * checking line of sight, detecting nearby enemies/shells, and handling tank actions.
 * It serves as the core decision-making support for tank algorithms.
 */
class MyBattleStatus {
public:
    size_t board_x{0};           ///< Width of the game board
    size_t board_y{0};           ///< Height of the game board
    size_t max_steps{0};         ///< Maximum number of steps allowed in the game
    size_t shells_count{0};       ///< Number of shells (ammunition) available to the tank
    size_t turn_number{0};       ///< Current turn number in the game
    size_t cool_down{0};         ///< Cooldown timer for the tank's shooting ability
    size_t last_requested_info_turn{0}; ///< Last turn when satellite info was requested

    Direction::DirectionType tank_direction; ///< Current direction the tank is facing
    Position tank_position = {-1, -1}; ///< Current position of the tank on the board

    /**
     * @enum BoardItem
     * @brief Represents different items that can appear on the game board
     * 
     * Each enum value corresponds to a specific game element and is mapped
     * to a character representation for display and logic purposes.
     */
    enum class BoardItem {
        WALL,         ///< Represents a wall obstacle
        MINE,         ///< Represents a mine
        SHELL,        ///< Represents a fired shell/projectile
        TANK_PLAYER_1, ///< Represents player 1's tank
        TANK_PLAYER_2, ///< Represents player 2's tank
        CURRENT_TANK, ///< Represents the current player's tank
        EMPTY,        ///< Represents an empty space
        OUTSIDE       ///< Represents space outside the board
    };

    explicit MyBattleStatus(int player_id, int tank_index);

    void updateBoard(const std::vector<std::vector<char> > &updated_board);

    Position updatePosition(Position p) const;

    void updateBattleStatusBaseAction(ActionRequest action);

    char getBoardItem(int x, int y) const;

    char getBoardItem(Position p) const;

    size_t getDistance(const Position &pos1, const Position &pos2) const;

    bool isShellClose(Position position, size_t thresh = 6) const;

    bool isShellClose(size_t thresh = 6) const;

    bool isEnemyClose(Position position) const;

    bool isEnemyClose() const;

    size_t getEnemyTankCounts() const;

    std::vector<Position> getEnemyPositions() const;

    bool hasTankAmmo() const;

    bool canTankShoot() const;

    bool canTankHitEnemy(bool include_shells = false) const;

    bool canTankHitEnemy(Position enemy) const;

    bool canTankHitEnemy(Direction::DirectionType dir, bool include_shells = false) const;

    bool isSafePosition(Position p, bool immediate_safe = false) const;

    std::vector<Direction::DirectionType> getSafeDirections(Position position) const;

    ActionRequest rotateTowards(Direction::DirectionType to_direction) const;

    /**
     * @brief Converts a BoardItem enum to its character representation
     * 
     * Maps each game element to a specific character for board representation:
     * - WALL: '#'
     * - SHELL: '*'
     * - MINE: '@'
     * - TANK_PLAYER_1: '1'
     * - TANK_PLAYER_2: '2'
     * - CURRENT_TANK: '%'
     * - EMPTY: ' '
     * - OUTSIDE: '&'
     * 
     * @param b The BoardItem to convert
     * @return char The character representation of the item
     */
    static char boardItemToChar(const BoardItem b) {
        switch (b) {
            case BoardItem::WALL: return '#';
            case BoardItem::SHELL: return '*';
            case BoardItem::MINE: return '@';
            case BoardItem::TANK_PLAYER_1: return '1';
            case BoardItem::TANK_PLAYER_2: return '2';
            case BoardItem::CURRENT_TANK: return '%';
            case BoardItem::EMPTY: return ' ';
            case BoardItem::OUTSIDE: return '&';
            default: return '?';
        }
    }

private:
    std::vector<std::vector<char> > board = {};  ///< 2D representation of the current game board
    std::vector<Position> enemy_positions = {};  ///< Cached positions of all enemy tanks
    std::vector<Position> ally_positions = {};   ///< Cached positions of all allied tanks
    std::vector<Position> shells_position = {};  ///< Cached positions of all shells on the board

    int player_id{0};    ///< ID of the player (1 or 2)
    int tank_index{0};   ///< Index of this tank for the player

    /**
     * @brief Gets the character representing enemy tanks
     * @return char Character representing enemy tanks
     */
    char getEnemyName() const;

    /**
     * @brief Gets the character representing allied tanks
     * @return char Character representing allied tanks
     */
    char getAllyName() const;

    /**
     * @brief Updates the positions of all tanks and shells on the board
     */
    void updateTanksPosition();

    /**
     * @brief Gets the current position of the tank
     * @return Position The tank's current position
     */
    Position getTankPosition() const;

    /**
     * @brief Updates the tank's direction based on an action
     * @param action The action that may change the tank's direction
     */
    void updateTankDirectionBaseAction(ActionRequest action);

    /**
     * @brief Checks if a target is in the line of sight from the tank
     * @param dir Direction to look in
     * @param enemy Position of the target to check
     * @return bool True if the target is in line of sight
     */
    bool isTargetOnSight(Direction::DirectionType dir, Position enemy) const;
};


#endif //BATTLEUTILS_H
