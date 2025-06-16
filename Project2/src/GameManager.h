#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include "Board.h"
#include "Tank.h"
#include "Shell.h"
#include "common/PlayerFactory.h"
#include "common/TankAlgorithmFactory.h"
#include "common/Player.h"
#include "algorithms/Controller.h"
#include "algorithms/Defensive.h"

// Concrete implementation of PlayerFactory
class MyPlayerFactory : public PlayerFactory {
public:
    std::unique_ptr<Player> create(int player_index, 
                                 size_t x, size_t y,
                                 size_t max_steps, 
                                 size_t num_shells) const override;
};

// Concrete implementation of TankAlgorithmFactory
class MyTankAlgorithmFactory : public TankAlgorithmFactory {
public:
    std::unique_ptr<TankAlgorithm> create(
        int player_index, int tank_index) const override;
};

class GameManager {
private:
    Board board;
    std::vector<Position> explosions;
    std::vector<std::unique_ptr<Tank>> tanks;
    std::vector<std::unique_ptr<GameObject>> shells;
    std::unique_ptr<MyPlayer> player1;
    std::unique_ptr<Player> player2;
    std::ofstream outputFile;
    size_t currentStep;
    bool gameOver;
    std::string gameResultMessage;
    bool visualMode;

    static const size_t MAX_NO_SHELLS_STEPS = 40;  // Tie after 40 steps with no shells
    size_t noShellsSteps;  // Count of consecutive steps where no player has shells

    MyPlayerFactory playerFactory;
    MyTankAlgorithmFactory algorithmFactory;

    // Visual board representation for display
    std::vector<std::vector<std::string>> visualBoard;

public:
    // Constructor
    GameManager(const PlayerFactory& playerFactory, 
                const TankAlgorithmFactory& algorithmFactory,
                bool visualMode = false);

    // Game control
    bool readBoard(const std::string& filename);
    void run();

private:
    // Core logic
    void initializePlayers();
    void initializeTanks();
    bool isTankAboutToHitWall(const int newX, const int newY) const; 
    bool isTankCollidingWithOthers(const Tank &tank,   Board &board);
    bool checkTankOnMine(Tank& tank);
    void processRound();
    bool isGameOver();
    void checkInitialGameOver();
    void writeOutputLine();
    void writeFinalOutput();
    void cleanupDeadObjects();
    void processTankAction(Tank& tank);
    void handleShellCollisions(Shell* shell);

    
    // Visualization
    void displayGame();              // Main visual output function
    void initVisualBoard();         // Fill visualBoard with base cell types
    void overlayShells();           // Place shells on visualBoard
    void overlayTanks();            // Place tanks (as arrows) on visualBoard
    void printBoard();              // Print visualBoard to console
    void printTankStatus();         // Show tank state info
    void printShellStatus();        // Show shell state info
    void printGameSummary();        // Summary of tank/shell counts
    void waitForVisualPause();      // Wait and clear screen
};

#endif // GAME_MANAGER_H
