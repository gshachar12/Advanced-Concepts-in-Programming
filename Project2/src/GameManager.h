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
    std::vector<std::unique_ptr<Tank>> tanks;
    std::vector<std::unique_ptr<GameObject>> shells;
    std::unique_ptr<Player> player1;
    std::unique_ptr<Player> player2;
    std::ofstream outputFile;
    size_t currentStep;
    bool gameOver;
    std::string gameResultMessage;
    bool visualMode;
    
    // Constants for game rules
    static const size_t MAX_NO_SHELLS_STEPS = 40;  // Tie after 40 steps with no shells
    
    // Tracking variables
    size_t noShellsSteps;  // Count of consecutive steps where no player has shells
    
    // Factory objects for creating players and algorithms
    MyPlayerFactory playerFactory;
    MyTankAlgorithmFactory algorithmFactory;

public:
    // Constructor accepting factories and visual mode
    GameManager(const PlayerFactory& playerFactory, 
                const TankAlgorithmFactory& algorithmFactory,
                bool visualMode = false);

    // Core game methods
    bool readBoard(const std::string& filename);
    void run();

private:
    // Helper methods
    void initializePlayers();
    void initializeTanks();
    void processRound();
    bool isGameOver();
    void checkInitialGameOver();  // Check if game is over before first round
    void writeOutputLine();
    void writeFinalOutput();
    void cleanupDeadObjects();
    void processTankAction(Tank& tank);
    void handleCollisions();
    void displayGame(); // New method for visualization
};

#endif // GAME_MANAGER_H