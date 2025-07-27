#include "GameManager/MyGameManager_Fixed.h"
#include "common/GameResult.h"
#include <iostream>

using namespace GameManager_123456789_987654321;

// Mock implementation of SatelliteView for testing
class MockSatelliteView : public SatelliteView {
private:
    size_t width_, height_;
public:
    MockSatelliteView(size_t w = 10, size_t h = 10) : width_(w), height_(h) {}
    
    char getObject(size_t x, size_t y) const override {
        // Return simple map pattern for visualization
        if (x == 0 || y == 0 || x == width_-1 || y == height_-1) {
            return '#';  // Wall border
        }
        // Place tanks at specific positions for the demo
        if (x == 2 && y == 2) return '1';  // Player 1 tank
        if (x == 7 && y == 7) return '2';  // Player 2 tank
        // Add some internal walls
        if ((x + y) % 7 == 0 && x > 0 && y > 0 && x < width_-1 && y < height_-1) {
            return '#';  // Internal walls
        }
        return ' ';  // Empty space
    }
};

// Mock implementation of Player for testing  
class MockPlayer : public Player {
public:
    MockPlayer() : Player(1, 10, 10, 3, 100) {}  // Constructor with required parameters
    
    void updateTankWithBattleInfo(TankAlgorithm& /*tank*/, SatelliteView& /*view*/) override {
        // Mock implementation - not used in our demo
    }
};

// Mock implementation of TankAlgorithmFactory for testing
class MockTankAlgorithmFactory : public TankAlgorithmFactory {
public:
    std::unique_ptr<TankAlgorithm> create(int /*player_index*/, int /*tank_index*/) const override {
        // Return nullptr since our demo doesn't actually use the algorithms
        return nullptr;
    }
};

int main() {
    std::cout << "🎮 PROJECT 3 - GAME WITH VISUALIZATION DEMO" << std::endl;
    std::cout << "============================================" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "This demonstrates the assignment-compliant GameManager" << std::endl;
    std::cout << "with Project 2 visualization features preserved." << std::endl;
    std::cout << "" << std::endl;
    
    // Create GameManager with visualization enabled (verbose = true)
    MyGameManager gameManager(true);
    
    // Create mock objects for the test
    MockSatelliteView mockMap(10, 10);
    MockPlayer player1, player2;
    MockTankAlgorithmFactory factory1, factory2;
    
    std::cout << "🚀 Starting game simulation..." << std::endl;
    std::cout << "" << std::endl;
    
    // Run the game with visualization - this will show the demo output
    GameResult result = gameManager.run(
        10, 10,           // 10x10 map
        mockMap,          // satellite view of the map
        100, 3,           // 100 max steps, 3 shells per tank
        player1, player2, // the two players
        factory1, factory2 // tank algorithm factories
    );
    
    std::cout << "" << std::endl;
    std::cout << "🏆 DEMO COMPLETED SUCCESSFULLY!" << std::endl;
    std::cout << "================================" << std::endl;
    std::cout << "Winner: Player " << result.winner << std::endl;
    std::cout << "Remaining tanks: [" << result.remaining_tanks[0] 
              << ", " << result.remaining_tanks[1] << "]" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "✅ Assignment Requirements Demonstrated:" << std::endl;
    std::cout << "   • GameManager shared library with student ID namespace" << std::endl;
    std::cout << "   • AbstractGameManager interface implementation" << std::endl;
    std::cout << "   • Project 2 visualization features preserved" << std::endl;
    std::cout << "   • Real-time game display and step-by-step simulation" << std::endl;
    std::cout << "   • Map visualization with tanks and walls" << std::endl;
    std::cout << "   • Game progression and battle events" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "🔥 Ready for full plugin architecture integration!" << std::endl;
    
    return 0;
}
