#include "src/GameManager.h"
#include "common/Player.h"
#include "common/TankAlgorithm.h"
#include <iostream>
#include <memory>
#include <vector>

// Simple test algorithm that does basic actions
class TestTankAlgorithm : public TankAlgorithm {
private:
    std::vector<ActionRequest> action_sequence_;
    size_t current_action_index_;
    
public:
    TestTankAlgorithm(const std::vector<ActionRequest>& actions) 
        : action_sequence_(actions), current_action_index_(0) {}
    
    void updateBattleInfo(BattleInfo& battle_info) override {
        // Store battle info if needed
    }
    
    ActionRequest getAction() override {
        if (current_action_index_ < action_sequence_.size()) {
            ActionRequest action = action_sequence_[current_action_index_];
            current_action_index_++;
            return action;
        }
        return ActionRequest::DoNothing;
    }
};

// Simple test player
class TestPlayer : public Player {
private:
    std::string name_;
    
public:
    explicit TestPlayer(const std::string& name) : name_(name) {}
    
    std::string getName() const override { return name_; }
};

// Simple test map implementation
class TestMap : public SatelliteView {
private:
    std::vector<std::vector<CellType>> map_data_;
    size_t width_, height_;
    
public:
    TestMap(size_t width, size_t height) : width_(width), height_(height) {
        map_data_.resize(height, std::vector<CellType>(width, CellType::Empty));
    }
    
    void setCell(size_t x, size_t y, CellType type) {
        if (x < width_ && y < height_) {
            map_data_[y][x] = type;
        }
    }
    
    CellType getCellType(size_t x, size_t y) const override {
        if (x < width_ && y < height_) {
            return map_data_[y][x];
        }
        return CellType::Wall;
    }
};

// Tank algorithm factories
std::unique_ptr<TankAlgorithm> createPlayer1Algorithm() {
    std::vector<ActionRequest> actions = {
        ActionRequest::MoveForward,
        ActionRequest::RotateRight45,
        ActionRequest::Shoot,
        ActionRequest::MoveForward,
        ActionRequest::DoNothing
    };
    return std::make_unique<TestTankAlgorithm>(actions);
}

std::unique_ptr<TankAlgorithm> createPlayer2Algorithm() {
    std::vector<ActionRequest> actions = {
        ActionRequest::RotateLeft90,
        ActionRequest::MoveForward,
        ActionRequest::Shoot,
        ActionRequest::MoveBackward,
        ActionRequest::DoNothing
    };
    return std::make_unique<TestTankAlgorithm>(actions);
}

int main() {
    std::cout << "🧪 Testing Complete Game Manager\n";
    std::cout << "════════════════════════════════\n\n";
    
    // Test 1: Basic game initialization and execution
    std::cout << "Test 1: Basic Game Execution\n";
    std::cout << "─────────────────────────────\n";
    
    try {
        // Create game manager with verbose output
        GameManager manager(true);
        
        // Create test map (10x10 with tanks at opposite corners)
        TestMap test_map(10, 10);
        test_map.setCell(1, 1, CellType::TANK1);  // Player 1 tank
        test_map.setCell(8, 8, CellType::TANK2);  // Player 2 tank
        
        // Add some walls for variety
        test_map.setCell(5, 5, CellType::WALL);
        test_map.setCell(5, 6, CellType::WALL);
        test_map.setCell(6, 5, CellType::WALL);
        
        // Create players
        TestPlayer player1("Player1");
        TestPlayer player2("Player2");
        
        // Run the game
        GameResult result = manager.run(
            10, 10,              // map dimensions
            test_map,            // satellite view
            20,                  // max steps
            3,                   // shells per tank
            player1, player2,    // players
            createPlayer1Algorithm,  // player1 algorithm factory
            createPlayer2Algorithm   // player2 algorithm factory
        );
        
        std::cout << "\n✅ Game completed successfully!\n";
        
        std::cout << "🏆 Result: ";
        
        if (result.winner == 1) {
            std::cout << "Player 1 Wins\n";
        } else if (result.winner == 2) {
            std::cout << "Player 2 Wins\n";
        } else {
            std::cout << "Tie\n";
        }
        
    } catch (const std::exception& e) {
        std::cout << "❌ Test failed with exception: " << e.what() << "\n";
        return 1;
    }
    
    std::cout << "\n\n";
    
    // Test 2: Quick game with limited steps
    std::cout << "Test 2: Limited Steps Game\n";
    std::cout << "──────────────────────────\n";
    
    try {
        GameManager manager(false); // No verbose output
        
        TestMap small_map(5, 5);
        small_map.setCell(1, 1, CellType::TANK1);
        small_map.setCell(3, 3, CellType::TANK2);
        
        TestPlayer player1("P1");
        TestPlayer player2("P2");
        
        GameResult result = manager.run(
            5, 5,
            small_map,
            3,  // Only 3 steps
            2,  // 2 shells per tank
            player1, player2,
            createPlayer1Algorithm,
            createPlayer2Algorithm
        );
        
        std::cout << "✅ Limited steps game completed\n";
        std::cout << "🏆 Result: ";
        
        if (result.winner == 1) {
            std::cout << "Player 1 Wins\n";
        } else if (result.winner == 2) {
            std::cout << "Player 2 Wins\n";
        } else {
            std::cout << "Tie (likely due to step limit)\n";
        }
        
    } catch (const std::exception& e) {
        std::cout << "❌ Test failed with exception: " << e.what() << "\n";
        return 1;
    }
    
    std::cout << "\n🎯 Complete Game Manager working! ✓\n";
    
    return 0;
}
