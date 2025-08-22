#ifndef TOURNAMENT_MANAGER_H
#define TOURNAMENT_MANAGER_H

#include "../../common/AbstractGameManager.h"
#include "../../common/GameResult.h"
#include "../../common/Player.h"
#include "../../common/TankAlgorithm.h"
#include "../../common/SatelliteView.h"
#include "../GameManager.h"
#include "AlgorithmFactory.h"
#include <vector>
#include <string>
#include <map>

/**
 * Manages tournaments between different algorithms
 */
class TournamentManager {
public:
    struct TournamentResult {
        std::string algorithm1_name;
        std::string algorithm2_name;
        int algorithm1_wins;
        int algorithm2_wins;
        int ties;
        std::vector<GameResult> individual_results;
    };
    
    struct AlgorithmStats {
        std::string name;
        int total_games;
        int wins;
        int losses;
        int ties;
        double win_rate;
    };

private:
    std::vector<AlgorithmRegistrar::AlgorithmInfo*> participating_algorithms_;
    size_t map_width_;
    size_t map_height_;
    size_t max_steps_;
    size_t shells_per_tank_;
    bool verbose_mode_;
    
    std::vector<TournamentResult> tournament_results_;
    
public:
    TournamentManager(size_t map_width = 10, size_t map_height = 10, 
                     size_t max_steps = 50, size_t shells_per_tank = 3, 
                     bool verbose = false);
    
    /**
     * Add algorithm to tournament
     */
    void addAlgorithm(const std::string& algorithm_name);
    void addAllRegisteredAlgorithms();
    
    /**
     * Run tournament
     */
    void runRoundRobin(int games_per_matchup = 3);
    void runSingleElimination();
    
    /**
     * Results and statistics
     */
    void printTournamentResults() const;
    std::vector<AlgorithmStats> calculateAlgorithmStats() const;
    void printAlgorithmRankings() const;
    
    /**
     * Configuration
     */
    void setVerbose(bool verbose) { verbose_mode_ = verbose; }
    void setGameParameters(size_t max_steps, size_t shells_per_tank);

private:
    TournamentResult runMatchup(AlgorithmRegistrar::AlgorithmInfo* algo1, 
                               AlgorithmRegistrar::AlgorithmInfo* algo2,
                               int num_games);
    
    GameResult runSingleGame(AlgorithmRegistrar::AlgorithmInfo* algo1,
                            AlgorithmRegistrar::AlgorithmInfo* algo2);
    
    std::unique_ptr<SatelliteView> createTestMap();
    void printMatchupResult(const TournamentResult& result) const;
};

#endif // TOURNAMENT_MANAGER_H
