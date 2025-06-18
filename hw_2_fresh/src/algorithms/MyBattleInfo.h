#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <vector>
#include "BattleInfo.h"
#include "Logger.h"

/**
 * @class MyBattleInfo
 * @brief Implementation of the BattleInfo interface that stores battle state information
 * 
 * This class maintains the current state of the battle, including the game board,
 * the maximum number of steps allowed in a game, and the number of shells available
 * to players. It serves as a container for game state information that algorithms
 * can use to make decisions.
 */
class MyBattleInfo final : public BattleInfo {
    // 2D representation of the game board using characters
    std::vector<std::vector<char> > board{};
    // Maximum number of steps allowed in a game
    size_t max_steps;
    // Number of shells available to a player
    size_t shells_count;

public:
    /**
     * @brief Constructor for MyBattleInfo
     * @param board A 2D vector representing the game board
     * @param player_id The ID of the player
     * @param max_steps Maximum number of steps allowed in the game
     * @param shells_count Number of shells available to the player
     */
    explicit MyBattleInfo(const std::vector<std::vector<char> > &board, const int player_id, size_t max_steps,
                          const size_t shells_count): board(board), max_steps(max_steps), shells_count(shells_count) {
        // Log the player ID for debugging purposes
        Logger::getInstance().log("Player_id: " + std::to_string(player_id));
    }

    /**
     * @brief Get the current state of the game board
     * @return A constant reference to the 2D game board
     */
    const std::vector<std::vector<char> > &getBoard() const { return board; }
    
    /**
     * @brief Get the number of shells available to the player
     * @return The number of shells as a size_t
     */
    size_t getNumShells() const { return shells_count; }
    
    /**
     * @brief Get the maximum number of steps allowed in the game
     * @return The maximum steps as a size_t
     */
    size_t getMaxSteps() const { return max_steps; }
};

#endif //GAMESTATE_H
