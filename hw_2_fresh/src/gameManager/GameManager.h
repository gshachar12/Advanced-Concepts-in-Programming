#ifndef MYGAMEMANAGER_H
#define MYGAMEMANAGER_H
#include <map>
#include <fstream>

#include "Board.h"
#include "PlayerFactory.h"
#include "TankAlgorithmFactory.h"
#include "Tank.h"

enum Winner {
    TIE_AMMO,
    TIE_STEPS,
    TIE,
    PLAYER_1,
    PLAYER_2,
    NO_WINNER
};

class GameManager {
    const PlayerFactory &player_factory;
    const TankAlgorithmFactory &tank_algorithm_factory;

public:
    GameManager(const PlayerFactory &player_factory, const TankAlgorithmFactory &tank_algorithm_factory)
        : player_factory(player_factory), tank_algorithm_factory(tank_algorithm_factory),
          satellite_view(MySatelliteView()) {
    }

    void readBoard(const std::string &file_name);

    void run();

    void updateCounters(Tank &tank, ActionRequest action);

    void setVisual(bool visual) { this->visual = visual; }
    
private:
    static constexpr int max_steps_empty_ammo = 40;

    bool visual = false;
    size_t game_step = 0;
    bool game_over = false;
    Winner winner = NO_WINNER;
    std::unique_ptr<Board> board;
    int empty_countdown = -1;
    std::vector<std::tuple<bool, ActionRequest, bool, bool> > tank_status;
    std::vector<std::unique_ptr<Player> > players;
    std::vector<std::unique_ptr<TankAlgorithm> > tanks;
    std::vector<std::vector<std::string>> visualBoard;
    MySatelliteView satellite_view;

    bool tankAction(Tank &tank, ActionRequest action);

    void checkDeaths();

    bool moveForward(Tank &tank);

    bool moveBackward(Tank &tank);

    bool rotate(Tank &tank, int turn);

    bool shoot(Tank &tank);

    bool getBattleInfo(const Tank &tank, size_t player_i);

    void updateSatelliteView();

    bool allEmptyAmmo() const;

    void tanksTurn();

    void shellsTurn() const;

    void processStep();

    bool isGameOver() const { return game_over; }

    std::string getGameResult() const;

    void logStep();

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

#endif //MYGAMEMANAGER_H
