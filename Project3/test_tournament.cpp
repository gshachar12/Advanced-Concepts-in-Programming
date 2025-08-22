#include "src/algorithms/TournamentManager.h"
#include "src/algorithms/AlgorithmFactory.h"
#include <iostream>

int main() {
    std::cout << "🏆 Testing Tournament System\n";
    std::cout << "══════════════════════════════\n\n";
    
    try {
        // Initialize algorithms
        std::cout << "Step 1: Setting up algorithms\n";
        std::cout << "─────────────────────────────\n";
        AlgorithmRegistrar::registerDefaultAlgorithms();
        
        // Create tournament manager
        std::cout << "Step 2: Creating tournament\n";
        std::cout << "───────────────────────────\n";
        TournamentManager tournament(10, 10, 30, 3, true); // 10x10 map, 30 steps, 3 shells, verbose
        
        // Add algorithms to tournament
        std::cout << "Step 3: Adding participants\n";
        std::cout << "──────────────────────────\n";
        tournament.addAlgorithm("Simple");
        tournament.addAlgorithm("BFS");
        tournament.addAlgorithm("Random");
        
        // Run round robin tournament
        std::cout << "Step 4: Running tournament\n";
        std::cout << "─────────────────────────\n";
        tournament.runRoundRobin(2); // 2 games per matchup
        
        // Display results
        std::cout << "Step 5: Final results\n";
        std::cout << "────────────────────\n";
        tournament.printTournamentResults();
        tournament.printAlgorithmRankings();
        
        std::cout << "🎯 Tournament system test completed successfully! ✓\n";
        
    } catch (const std::exception& e) {
        std::cout << "❌ Test failed with exception: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}
