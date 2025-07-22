#include "TournamentManager.h"
#include "../../src/CellType.h"
#include <iostream>
#include <algorithm>
#include <iomanip>

// Simple test map implementation for tournaments
class TournamentMap : public SatelliteView {
private:
    size_t width_, height_;
    
public:
    TournamentMap(size_t width, size_t height) : width_(width), height_(height) {}
    
    char getObjectAt(size_t x, size_t y) const override {
        // Create a simple map layout
        if (x == 0 || y == 0 || x == width_ - 1 || y == height_ - 1) {
            return '#'; // Wall border
        }
        
        // Place tanks at opposite corners
        if (x == 1 && y == 1) {
            return '1'; // Player 1 tank
        }
        if (x == width_ - 2 && y == height_ - 2) {
            return '2'; // Player 2 tank
        }
        
        // Add some obstacles in the middle
        if ((x == width_ / 2 && y == height_ / 2) ||
            (x == width_ / 2 + 1 && y == height_ / 2) ||
            (x == width_ / 2 && y == height_ / 2 + 1)) {
            return '#'; // Walls
        }
        
        return ' '; // Empty space
    }
};

TournamentManager::TournamentManager(size_t map_width, size_t map_height, 
                                   size_t max_steps, size_t shells_per_tank, 
                                   bool verbose) 
    : map_width_(map_width), map_height_(map_height), 
      max_steps_(max_steps), shells_per_tank_(shells_per_tank), 
      verbose_mode_(verbose) {
}

void TournamentManager::addAlgorithm(const std::string& algorithm_name) {
    auto* algorithm_info = AlgorithmRegistrar::findAlgorithm(algorithm_name);
    if (algorithm_info) {
        participating_algorithms_.push_back(algorithm_info);
        std::cout << "[SUCCESS] Added " << algorithm_name << " to tournament\n";
    } else {
        std::cout << "[ERROR] Algorithm " << algorithm_name << " not found\n";
    }
}

void TournamentManager::addAllRegisteredAlgorithms() {
    const auto& registered = AlgorithmRegistrar::getRegisteredAlgorithms();
    for (const auto& algorithm : registered) {
        participating_algorithms_.push_back(const_cast<AlgorithmRegistrar::AlgorithmInfo*>(&algorithm));
    }
    std::cout << "[SUCCESS] Added all " << participating_algorithms_.size() << " registered algorithms to tournament\n";
}

void TournamentManager::runRoundRobin(int games_per_matchup) {
    std::cout << "\nStarting Round Robin Tournament\n";
    std::cout << "===================================\n";
    std::cout << "Participants: " << participating_algorithms_.size() << " algorithms\n";
    std::cout << "Games per matchup: " << games_per_matchup << "\n";
    std::cout << "Map size: " << map_width_ << "x" << map_height_ << "\n";
    std::cout << "Max steps: " << max_steps_ << ", Shells: " << shells_per_tank_ << "\n\n";
    
    tournament_results_.clear();
    
    // Run all possible matchups
    for (size_t i = 0; i < participating_algorithms_.size(); ++i) {
        for (size_t j = i + 1; j < participating_algorithms_.size(); ++j) {
            auto* algo1 = participating_algorithms_[i];
            auto* algo2 = participating_algorithms_[j];
            
            std::cout << "🥊 Matchup: " << algo1->name << " vs " << algo2->name << "\n";
            std::cout << "─────────────────────────────────────────────\n";
            
            TournamentResult result = runMatchup(algo1, algo2, games_per_matchup);
            tournament_results_.push_back(result);
            
            printMatchupResult(result);
            std::cout << "\n";
        }
    }
    
    std::cout << "Tournament completed!\n\n";
}

void TournamentManager::runSingleElimination() {
    std::cout << "\n🏆 Starting Single Elimination Tournament\n";
    std::cout << "════════════════════════════════════════════\n";
    
    // For now, implement as round robin with single games
    runRoundRobin(1);
}

TournamentManager::TournamentResult TournamentManager::runMatchup(
    AlgorithmRegistrar::AlgorithmInfo* algo1, 
    AlgorithmRegistrar::AlgorithmInfo* algo2,
    int num_games) {
    
    TournamentResult result;
    result.algorithm1_name = algo1->name;
    result.algorithm2_name = algo2->name;
    result.algorithm1_wins = 0;
    result.algorithm2_wins = 0;
    result.ties = 0;
    
    for (int game = 0; game < num_games; ++game) {
        if (verbose_mode_) {
            std::cout << "  Game " << (game + 1) << "/" << num_games << ": ";
        }
        
        GameResult game_result = runSingleGame(algo1, algo2);
        result.individual_results.push_back(game_result);
        
        if (game_result.winner == 1) {
            result.algorithm1_wins++;
            if (verbose_mode_) {
                std::cout << algo1->name << " wins!\n";
            }
        } else if (game_result.winner == 2) {
            result.algorithm2_wins++;
            if (verbose_mode_) {
                std::cout << algo2->name << " wins!\n";
            }
        } else {
            result.ties++;
            if (verbose_mode_) {
                std::cout << "Tie!\n";
            }
        }
    }
    
    return result;
}

GameResult TournamentManager::runSingleGame(AlgorithmRegistrar::AlgorithmInfo* algo1,
                                           AlgorithmRegistrar::AlgorithmInfo* algo2) {
    // Create game manager
    GameManager manager(false); // Non-verbose for tournament
    
    // Create test map
    auto map = createTestMap();
    
    // Create players
    auto player1 = algo1->player_factory(0, 1, 1, max_steps_, shells_per_tank_);
    auto player2 = algo2->player_factory(1, map_width_ - 2, map_height_ - 2, max_steps_, shells_per_tank_);
    
    // Run the game
    return manager.run(
        map_width_, map_height_,
        *map,
        max_steps_,
        shells_per_tank_,
        *player1, *player2,
        algo1->factory,
        algo2->factory
    );
}

std::unique_ptr<SatelliteView> TournamentManager::createTestMap() {
    return std::make_unique<TournamentMap>(map_width_, map_height_);
}

void TournamentManager::printMatchupResult(const TournamentResult& result) const {
    std::cout << "📊 Results: " << result.algorithm1_name << " " 
              << result.algorithm1_wins << "-" << result.algorithm2_wins 
              << "-" << result.ties << " " << result.algorithm2_name;
    
    if (result.algorithm1_wins > result.algorithm2_wins) {
        std::cout << " (🏆 " << result.algorithm1_name << " wins matchup)";
    } else if (result.algorithm2_wins > result.algorithm1_wins) {
        std::cout << " (🏆 " << result.algorithm2_name << " wins matchup)";
    } else {
        std::cout << " (🤝 Matchup tied)";
    }
    std::cout << "\n";
}

void TournamentManager::printTournamentResults() const {
    std::cout << "🏆 TOURNAMENT RESULTS\n";
    std::cout << "═══════════════════════\n\n";
    
    for (const auto& result : tournament_results_) {
        printMatchupResult(result);
    }
    std::cout << "\n";
}

std::vector<TournamentManager::AlgorithmStats> TournamentManager::calculateAlgorithmStats() const {
    std::map<std::string, AlgorithmStats> stats_map;
    
    // Initialize stats for all algorithms
    for (const auto* algo : participating_algorithms_) {
        AlgorithmStats stats;
        stats.name = algo->name;
        stats.total_games = 0;
        stats.wins = 0;
        stats.losses = 0;
        stats.ties = 0;
        stats.win_rate = 0.0;
        stats_map[algo->name] = stats;
    }
    
    // Calculate stats from tournament results
    for (const auto& result : tournament_results_) {
        // Update algorithm1 stats
        auto& stats1 = stats_map[result.algorithm1_name];
        stats1.total_games += result.individual_results.size();
        stats1.wins += result.algorithm1_wins;
        stats1.losses += result.algorithm2_wins;
        stats1.ties += result.ties;
        
        // Update algorithm2 stats
        auto& stats2 = stats_map[result.algorithm2_name];
        stats2.total_games += result.individual_results.size();
        stats2.wins += result.algorithm2_wins;
        stats2.losses += result.algorithm1_wins;
        stats2.ties += result.ties;
    }
    
    // Calculate win rates
    std::vector<AlgorithmStats> stats_vector;
    for (auto& [name, stats] : stats_map) {
        if (stats.total_games > 0) {
            stats.win_rate = static_cast<double>(stats.wins) / stats.total_games;
        }
        stats_vector.push_back(stats);
    }
    
    // Sort by win rate (descending)
    std::sort(stats_vector.begin(), stats_vector.end(), 
              [](const AlgorithmStats& a, const AlgorithmStats& b) {
                  return a.win_rate > b.win_rate;
              });
    
    return stats_vector;
}

void TournamentManager::printAlgorithmRankings() const {
    auto stats = calculateAlgorithmStats();
    
    std::cout << "📊 ALGORITHM RANKINGS\n";
    std::cout << "═══════════════════════\n\n";
    
    std::cout << std::left << std::setw(15) << "Rank" << std::setw(15) << "Algorithm" 
              << std::setw(10) << "Games" << std::setw(8) << "Wins" 
              << std::setw(8) << "Losses" << std::setw(8) << "Ties" 
              << std::setw(12) << "Win Rate" << "\n";
    std::cout << std::string(80, '-') << "\n";
    
    for (size_t i = 0; i < stats.size(); ++i) {
        const auto& stat = stats[i];
        std::cout << std::left << std::setw(15) << ("#" + std::to_string(i + 1))
                  << std::setw(15) << stat.name
                  << std::setw(10) << stat.total_games
                  << std::setw(8) << stat.wins
                  << std::setw(8) << stat.losses  
                  << std::setw(8) << stat.ties
                  << std::setw(12) << std::fixed << std::setprecision(3) << stat.win_rate
                  << "\n";
    }
    std::cout << "\n";
}

void TournamentManager::setGameParameters(size_t max_steps, size_t shells_per_tank) {
    max_steps_ = max_steps;
    shells_per_tank_ = shells_per_tank;
}
