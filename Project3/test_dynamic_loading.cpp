#include "src/simulator/Simulator.h"
#include "src/simulator/AlgorithmRegistrar.h"
#include <iostream>
#include <memory>

/**
 * Test the dynamic plugin loading system
 */
int main() {
    std::cout << "🧪 Testing Dynamic Plugin Loading System\n";
    std::cout << "==========================================\n\n";
    
    try {
        // Test 1: Create simulator
        std::cout << "Test 1: Simulator Creation\n";
        std::cout << "──────────────────────────\n";
        
        Simulator simulator("plugins/");
        std::cout << "✅ Simulator created successfully\n\n";
        
        // Test 2: Check initial state
        std::cout << "Test 2: Initial State\n";
        std::cout << "────────────────────\n";
        
        auto& registrar = DynamicAlgorithmRegistrar::getAlgorithmRegistrar();
        std::cout << "Initial registered algorithms: " << registrar.count() << "\n\n";
        
        // Test 3: Discover algorithm files
        std::cout << "Test 3: Algorithm Discovery\n";
        std::cout << "──────────────────────────\n";
        
        auto discovered_files = simulator.discoverAlgorithmFiles();
        if (discovered_files.empty()) {
            std::cout << "ℹ️  No plugin files found in plugins/ directory\n";
            std::cout << "   This is expected if plugins haven't been built yet\n";
        } else {
            std::cout << "Found " << discovered_files.size() << " plugin files:\n";
            for (const auto& file : discovered_files) {
                std::cout << "  📦 " << file << "\n";
            }
        }
        std::cout << "\n";
        
        // Test 4: Try to load algorithms
        std::cout << "Test 4: Algorithm Loading\n";
        std::cout << "────────────────────────\n";
        
        bool loaded = simulator.loadAllAlgorithms();
        if (loaded) {
            std::cout << "✅ Algorithms loaded successfully\n";
            std::cout << "Registered algorithms after loading: " << registrar.count() << "\n";
            
            // List loaded algorithms
            for (const auto& algo : registrar) {
                std::cout << "  🤖 " << algo.name() << "\n";
            }
        } else {
            std::cout << "ℹ️  No algorithms loaded (plugins not built or directory empty)\n";
        }
        std::cout << "\n";
        
        // Test 5: Test algorithm lookup
        std::cout << "Test 5: Algorithm Lookup\n";
        std::cout << "───────────────────────\n";
        
        if (registrar.count() > 0) {
            const auto& first_algo = *registrar.begin();
            std::cout << "Testing lookup for: " << first_algo.name() << "\n";
            
            auto* found = registrar.findAlgorithm(first_algo.name());
            if (found) {
                std::cout << "✅ Algorithm lookup successful\n";
            } else {
                std::cout << "❌ Algorithm lookup failed\n";
            }
        } else {
            std::cout << "ℹ️  No algorithms to test lookup\n";
        }
        std::cout << "\n";
        
        // Test 6: Test factory creation (if algorithms are loaded)
        std::cout << "Test 6: Factory Function Test\n";
        std::cout << "────────────────────────────\n";
        
        if (registrar.count() > 0) {
            try {
                const auto& first_algo = *registrar.begin();
                auto tank_algo = first_algo.createTankAlgorithm(0, 0);
                auto player = first_algo.createPlayer(0, 5, 5, 100, 5);
                
                if (tank_algo && player) {
                    std::cout << "✅ Factory functions work correctly\n";
                } else {
                    std::cout << "❌ Factory functions returned null\n";
                }
            } catch (const std::exception& e) {
                std::cout << "❌ Factory function test failed: " << e.what() << "\n";
            }
        } else {
            std::cout << "ℹ️  No algorithms to test factories\n";
        }
        std::cout << "\n";
        
        // Test 7: Cleanup
        std::cout << "Test 7: Cleanup\n";
        std::cout << "──────────────\n";
        
        simulator.unloadAllAlgorithms();
        std::cout << "✅ Cleanup completed\n";
        std::cout << "Algorithms after cleanup: " << registrar.count() << "\n\n";
        
        std::cout << "🎯 Dynamic Plugin Loading test completed!\n";
        std::cout << "\nTo test with actual plugins:\n";
        std::cout << "1. Run: mingw32-make plugins\n";
        std::cout << "2. Run: test_dynamic_loading.exe\n";
        
    } catch (const std::exception& e) {
        std::cout << "❌ Test failed with exception: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}
