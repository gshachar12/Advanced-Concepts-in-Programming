#ifndef MY_GAME_MANAGER_FIXED_H
#define MY_GAME_MANAGER_FIXED_H

#include "../common/AbstractGameManager.h"
#include "MyBattleInfo.h"
#include <memory>
#include <vector>
#include <string>

namespace GameManager_123456789_987654321 {

// Game state structures for interactive visualization
struct Tank {
    size_t x, y;
    int player;
    int shells;
    int direction; // 0-7 for 8 directions
    bool alive;
    int cooldown;
    std::unique_ptr<TankAlgorithm> algorithm; // Tank's algorithm instance
    
    // Make Tank movable but not copyable
    Tank() = default;
    Tank(const Tank&) = delete;
    Tank& operator=(const Tank&) = delete;
    Tank(Tank&&) = default;
    Tank& operator=(Tank&&) = default;
};

struct Shell {
    size_t x, y;
    int direction;
    int owner;
    bool active;
};

struct GameState {
    std::vector<Tank> tanks;
    std::vector<Shell> shells;
    size_t width, height;
    size_t max_steps;
    size_t current_step;
    bool all_shells_exhausted;
    size_t post_shell_steps;
};

class MyGameManager : public AbstractGameManager {
public:
    MyGameManager(bool verbose = false) : verbose_(verbose) {}

    GameResult run(
        size_t map_width, size_t map_height,
        SatelliteView& map,
        size_t max_steps, size_t num_shells,
        Player& player1, Player& player2,
        TankAlgorithmFactory& player1_tank_algo_factory,
        TankAlgorithmFactory& player2_tank_algo_factory) override;

private:
    bool verbose_;
    TankAlgorithmFactory* player1_factory_;
    TankAlgorithmFactory* player2_factory_;
    
    // Basic visualization methods
    void displayMap(SatelliteView& map, size_t width, size_t height);
    void simulateGameWithVisualization(size_t width, size_t height, size_t max_steps);
    
    // Interactive Project 2-style visualization methods
    void displayInteractiveMap(SatelliteView& map, size_t width, size_t height);
    std::string getEmojiForCell(char cell);
    GameResult simulateInteractiveGame(SatelliteView& map, size_t width, size_t height, size_t max_steps, size_t num_shells);
    GameResult simulateInteractiveGameWithAlgorithms(SatelliteView& map, size_t width, size_t height, size_t max_steps, size_t num_shells);
    
    // Screen control
    void clearScreen();
    void waitForInput();
    
    // Game state management
    void initializeTanks(GameState& state, SatelliteView& map, size_t width, size_t height, size_t num_shells);
    void initializeTanksWithAlgorithms(GameState& state, SatelliteView& map, size_t width, size_t height, size_t num_shells);
    void executeTurn(GameState& state);
    void executeTurnWithAlgorithms(GameState& state, SatelliteView& map);
    void displayGameState(const GameState& state, size_t width, size_t height);
    
    // Status display methods
    void printTankStatus(const GameState& state);
    void printShellStatus(const GameState& state);
    void printGameSummary(const GameState& state);
    
    // Game logic helpers
    bool isGameOver(GameState& state);
    GameResult generateFinalResult(const GameState& state);
    void moveShell(Shell& shell);
    void checkShellCollisions(Shell& shell, GameState& state);
    void executeTankAction(Tank& tank, int action, GameState& state);
    void executeTankActionFromAlgorithm(Tank& tank, ActionRequest action, GameState& state, SatelliteView& map);
    void moveTank(Tank& tank, const GameState& state);
    void shootShell(Tank& tank, GameState& state);
    MyBattleInfo createBattleInfo(const GameState& state, const Tank& tank, SatelliteView& map);
};

} // namespace GameManager_123456789_987654321

#endif // MY_GAME_MANAGER_FIXED_H
