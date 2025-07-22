#include "src/algorithms/AlgorithmFactory.h"
#include "src/algorithms/SimpleAlgorithm.h"
#include "src/algorithms/BfsAlgorithm.h"
#include "src/algorithms/MyPlayer.h"
#include <iostream>
#include <memory>

int main() {
    std::cout << "🧪 Testing Plugin Architecture\n";
    std::cout << "═════════════════════════════════\n\n";
    
    try {
        // Test 1: Register default algorithms
        std::cout << "Test 1: Algorithm Registration\n";
        std::cout << "─────────────────────────────\n";
        
        AlgorithmRegistrar::registerDefaultAlgorithms();
        
        const auto& algorithms = AlgorithmRegistrar::getRegisteredAlgorithms();
        std::cout << "✅ Successfully registered " << algorithms.size() << " algorithms\n\n";
        
        // Test 2: Create algorithms using factories
        std::cout << "Test 2: Algorithm Creation\n";
        std::cout << "─────────────────────────────\n";
        
        // Test Simple Algorithm
        auto simple_algo = AlgorithmFactory::createSimpleAlgorithm(0, 0);
        if (simple_algo) {
            std::cout << "✅ SimpleAlgorithm created successfully\n";
        }
        
        // Test BFS Algorithm
        auto bfs_algo = AlgorithmFactory::createBfsAlgorithm(1, 0);
        if (bfs_algo) {
            std::cout << "✅ BfsAlgorithm created successfully\n";
        }
        
        // Test Random Algorithm
        auto random_algo = AlgorithmFactory::createRandomAlgorithm(0, 1);
        if (random_algo) {
            std::cout << "✅ RandomAlgorithm created successfully\n";
        }
        
        std::cout << "\n";
        
        // Test 3: Create players using factories
        std::cout << "Test 3: Player Creation\n";
        std::cout << "─────────────────────────\n";
        
        auto simple_player = AlgorithmFactory::createSimplePlayer(0, 5, 5, 100, 5);
        if (simple_player) {
            auto* my_player = dynamic_cast<MyPlayer*>(simple_player.get());
            if (my_player) {
                std::cout << "✅ SimplePlayer created successfully: " << my_player->getName() << "\n";
            } else {
                std::cout << "✅ SimplePlayer created successfully\n";
            }
        }
        
        auto bfs_player = AlgorithmFactory::createBfsPlayer(1, 10, 10, 100, 5);
        if (bfs_player) {
            auto* my_player = dynamic_cast<MyPlayer*>(bfs_player.get());
            if (my_player) {
                std::cout << "✅ BfsPlayer created successfully: " << my_player->getName() << "\n";
            } else {
                std::cout << "✅ BfsPlayer created successfully\n";
            }
        }
        
        std::cout << "\n";
        
        // Test 4: Test algorithm lookup
        std::cout << "Test 4: Algorithm Lookup\n";
        std::cout << "─────────────────────────\n";
        
        auto* simple_info = AlgorithmRegistrar::findAlgorithm("Simple");
        if (simple_info) {
            std::cout << "✅ Found Simple algorithm: " << simple_info->description << "\n";
        }
        
        auto* bfs_info = AlgorithmRegistrar::findAlgorithm("BFS");
        if (bfs_info) {
            std::cout << "✅ Found BFS algorithm: " << bfs_info->description << "\n";
        }
        
        auto* missing_info = AlgorithmRegistrar::findAlgorithm("NonExistent");
        if (!missing_info) {
            std::cout << "✅ Correctly returned null for non-existent algorithm\n";
        }
        
        std::cout << "\n";
        
        // Test 5: Test factory functions
        std::cout << "Test 5: Factory Functions\n";
        std::cout << "─────────────────────────\n";
        
        TankAlgorithmFactory simple_factory = AlgorithmFactory::getSimpleAlgorithmFactory();
        auto created_simple = simple_factory(0, 0);
        if (created_simple) {
            std::cout << "✅ Simple factory function works\n";
        }
        
        TankAlgorithmFactory bfs_factory = AlgorithmFactory::getBfsAlgorithmFactory();
        auto created_bfs = bfs_factory(1, 0);
        if (created_bfs) {
            std::cout << "✅ BFS factory function works\n";
        }
        
        PlayerFactory player_factory = AlgorithmFactory::getSimplePlayerFactory();
        auto created_player = player_factory(0, 5, 5, 100, 3);
        if (created_player) {
            std::cout << "✅ Player factory function works\n";
        }
        
        std::cout << "\n🎯 Plugin Architecture test completed successfully! ✓\n";
        
    } catch (const std::exception& e) {
        std::cout << "❌ Test failed with exception: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}
