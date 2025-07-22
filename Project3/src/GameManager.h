#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "../common/AbstractGameManager.h"
#include "../common/GameResult.h"
#include "../common/Player.h"
#include "../common/TankAlgorithm.h"
#include "GameState.h"
#include "CollisionDetector.h"
#include "ActionProcessor.h"
#include <memory>
#include <vector>

/**
 * Complete game manager implementation that orchestrates the entire game
 */
class GameManager : public AbstractGameManager {
protected:
    // Make these accessible to VisualizingGameManager
    std::unique_ptr<GameState> game_state_;
    std::unique_ptr<CollisionDetector> collision_detector_;
    std::unique_ptr<ActionProcessor> action_processor_;
    bool verbose_mode_;
    
    // Player algorithms
    std::vector<std::unique_ptr<TankAlgorithm>> player1_algorithms_;
    std::vector<std::unique_ptr<TankAlgorithm>> player2_algorithms_;
    
    /**
     * Initialize game state from satellite view
     */
    bool initializeGameFromMap(SatelliteView& map, size_t map_width, size_t map_height, size_t num_shells);
    
    /**
     * Initialize player algorithms
     */
    void initializePlayers(Player& player1, Player& player2,
                          TankAlgorithmFactory player1_factory, TankAlgorithmFactory player2_factory);
    
    /**
     * Execute one complete turn
     */
    void executeTurn(Player& player1, Player& player2);
    
    /**
     * Update tank algorithms with current battle info
     */
    void updateTanksWithBattleInfo(Player& player1, Player& player2);
    
    /**
     * Check win/loss conditions
     */
    bool checkGameEndConditions();
    
    /**
     * Generate final game result
     */
    GameResult generateGameResult();
    
    /**
     * Set up tank algorithms for both players
     */
    void setupTankAlgorithms(TankAlgorithmFactory factory1, TankAlgorithmFactory factory2);

public:
    explicit GameManager(bool verbose = false);
    
    /**
     * AbstractGameManager interface implementation
     */
    GameResult run(
        size_t map_width, 
        size_t map_height,
        SatelliteView& map,
        size_t max_steps, 
        size_t num_shells,
        Player& player1, 
        Player& player2,
        TankAlgorithmFactory player1_tank_algo_factory,
        TankAlgorithmFactory player2_tank_algo_factory) override;

private:
    /**
     * Execute actions for all tanks
     */
    void executeAllTankActions();
    
    /**
     * Update shell positions and handle movement
     */
    void updateShells();
    
    /**
     * Check and handle all collisions
     */
    void handleCollisions();
    
    /**
     * Update all tank states (cooldowns, etc.)
     */
    void updateTankStates();
    
    /**
     * Logging and debug output
     */
    void logTurnStart(size_t turn);
    void logTankAction(const Tank* tank, ActionRequest action, bool success);
    void logGameEnd(const GameResult& result);
    
    /**
     * Visualization
     */
    void displayBoard();
    void displayGameState();
};

#endif // GAME_MANAGER_H
