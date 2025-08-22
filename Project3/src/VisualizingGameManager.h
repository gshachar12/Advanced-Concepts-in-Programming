#ifndef VISUALIZING_GAME_MANAGER_H
#define VISUALIZING_GAME_MANAGER_H

#include "GameManager.h"
#include "../common/SatelliteView.h"
#include "../common/GameResult.h"
#include "Direction.h"
#include "CellType.h"
#include <vector>
#include <string>
#include <map>

/**
 * Enhanced GameManager that provides visual feedback during game execution
 * Implements rich Project 2-style visualization with emojis and detailed status
 */
class VisualizingGameManager : public GameManager {
public:
    explicit VisualizingGameManager(bool verbose = false);
    
    /**
     * Override run method to add visualization
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

protected:
    /**
     * Execute one turn with visualization
     */
    void executeVisualTurn(Player& player1, Player& player2, size_t turn);

private:
    // Visual board representation
    std::vector<std::vector<std::string>> visualBoard;
    
    /**
     * Main visualization method - displays complete game state
     */
    void displayGame(size_t turn);
    
    /**
     * Initialize the visual board with emojis for terrain
     */
    void initVisualBoard();
    
    /**
     * Add shell emojis to the visual board
     */
    void overlayShells();
    
    /**
     * Add tank representations with directional arrows
     */
    void overlayTanks();
    
    /**
     * Print the visual board to console
     */
    void printBoard();
    
    /**
     * Print detailed tank status information
     */
    void printTankStatus();
    
    /**
     * Print active shell information
     */
    void printShellStatus();
    
    /**
     * Print game summary (tank counts, shell counts, etc.)
     */
    void printGameSummary();
    
    /**
     * Pause execution and wait for user input
     */
    void pauseForVisualization();
    
    /**
     * Clear screen for better visualization
     */
    void clearScreen();
    
    /**
     * Display board from satellite view with emojis
     */
    void displayBoardFromSatelliteView(const SatelliteView& map, size_t width, size_t height);
    
    /**
     * Display rich emoji board using game state
     */
    void displayRichBoard();
    
    /**
     * Get direction symbol for display
     */
    std::string getDirectionSymbol(Direction dir);
    
    /**
     * Get ASCII direction symbol for display
     */
    std::string getDirectionSymbolASCII(Direction dir);
    
    /**
     * Get emoji representation for board cells
     */
    std::string getCellEmoji(CellType cell);
    
    /**
     * Get rich emoji representation for board cells (Project 2 style)
     */
    std::string getCellEmojiRich(CellType cell);
    
    /**
     * Get tank emoji with direction for Project 2 style visualization
     */
    std::string getTankEmojiWithDirection(int playerId, Direction dir);
    
    /**
     * Get emoji representation for character cells
     */
    std::string getEmojiForChar(char cell);
};

#endif // VISUALIZING_GAME_MANAGER_H
