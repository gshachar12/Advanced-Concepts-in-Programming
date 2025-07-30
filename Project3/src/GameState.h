#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "Board.h"
#include "Tank.h"
#include "Shell.h"
#include "../common/GameResult.h"
#include <vector>
#include <memory>

/**
 * Manages the complete state of a game
 */
class GameState {
private:
    std::unique_ptr<Board> board_;
    std::vector<std::unique_ptr<Tank>> player1_tanks_;
    std::vector<std::unique_ptr<Tank>> player2_tanks_;
    std::vector<std::unique_ptr<Shell>> active_shells_;
    
    size_t max_steps_;
    size_t current_step_;
    size_t initial_shells_per_tank_;
    bool game_over_;
    GameResult result_;

public:
    GameState(size_t map_width, size_t map_height, size_t max_steps, size_t shells_per_tank);
    
    // Board access
    Board& getBoard() { return *board_; }
    const Board& getBoard() const { return *board_; }
    
    // Tank management
    void addTank(int player_id, const Position& pos, Direction dir);
    std::vector<Tank*> getPlayerTanks(int player_id);
    std::vector<const Tank*> getPlayerTanks(int player_id) const;
    std::vector<Tank*> getAllTanks();
    std::vector<const Tank*> getAllTanks() const;
    
    // Shell management
    void addShell(const Position& pos, Direction dir, int owner_player_id, int owner_tank_id);
    std::vector<Shell*> getAllShells();
    std::vector<const Shell*> getAllShells() const;
    void removeInactiveShells();
    
    // Game state
    size_t getCurrentStep() const { return current_step_; }
    size_t getMaxSteps() const { return max_steps_; }
    bool isGameOver() const { return game_over_; }
    const GameResult& getResult() const { return result_; }
    
    // Game progression
    void advanceStep() { current_step_++; }
    void setGameOver(int winner, GameResult::Reason reason);
    
    // Utility methods
    size_t getAlivePlayerTanks(int player_id) const;
    size_t getTotalPlayerShells(int player_id) const;
    bool hasPlayerLost(int player_id) const;
    
    // Update board representation with current entity positions
    void updateBoardRepresentation();
};

#endif // GAME_STATE_H
