#include "GameManager/MyGameManager_Fixed.h"
#include "common/GameResult.h"
#include "plugins/SimplePlugin/SimpleTankAlgorithm.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

using namespace GameManager_123456789_987654321;

// SatelliteView implementation that reads from input file
class FileSatelliteView : public SatelliteView {
private:
    std::vector<std::string> map_data_;
    size_t width_, height_;
    
public:
    FileSatelliteView(const std::string& filename) {
        loadFromFile(filename);
    }
    
    void loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file " << filename << std::endl;
            return;
        }
        
        std::string line;
        bool reading_map = false;
        
        while (std::getline(file, line)) {
            // Skip comment lines and metadata
            if (line.empty() || line[0] == '#' || 
                line.find("MaxSteps=") == 0 || 
                line.find("NumShells=") == 0 ||
                line.find("Rows=") == 0 ||
                line.find("Cols=") == 0 ||
                line.find(" - ") != std::string::npos) {
                continue;
            }
            
            // This is a map line
            map_data_.push_back(line);
        }
        
        if (!map_data_.empty()) {
            height_ = map_data_.size();
            width_ = map_data_[0].length();
            
            // Ensure all rows have the same width
            for (auto& row : map_data_) {
                while (row.length() < width_) {
                    row += ' ';
                }
            }
        }
        
        std::cout << "Loaded map: " << width_ << "x" << height_ << std::endl;
    }
    
    char getObject(size_t x, size_t y) const override {
        if (y >= map_data_.size() || x >= map_data_[y].length()) {
            return '#'; // Wall for out of bounds
        }
        
        char cell = map_data_[y][x];
        
        // Convert map symbols to our format
        switch (cell) {
            case '@': return '#';  // Wall
            case '#': return '#';  // Wall  
            case '1': return '1';  // Player 1 tank
            case '2': return '2';  // Player 2 tank
            case ' ': return ' ';  // Empty space
            default: return ' ';   // Default to empty
        }
    }
    
    size_t getWidth() const { return width_; }
    size_t getHeight() const { return height_; }
};

// Mock implementation of Player for testing  
class MockPlayer : public Player {
public:
    MockPlayer() : Player(1, 10, 10, 3, 100) {}  // Constructor with required parameters
    
    void updateTankWithBattleInfo(TankAlgorithm& /*tank*/, SatelliteView& /*view*/) override {
        // Mock implementation - not used in our demo
    }
};

// Tank algorithm factory that creates real algorithms
class RealTankAlgorithmFactory : public TankAlgorithmFactory {
public:
    std::unique_ptr<TankAlgorithm> create(int player_index, int tank_index) const override {
        // Create actual SimpleTankAlgorithm instances for real gameplay
        return std::make_unique<SimplePlugin::SimpleTankAlgorithm>(player_index, tank_index);
    }
};

int main(int argc, char* argv[]) {
    std::string input_file = "inputs/input1.txt"; // Default
    
    if (argc > 1) {
        input_file = argv[1];
    }
    
    std::cout << "🎮 PROJECT 3 - GAME WITH REAL INPUT AND ALGORITHMS" << std::endl;
    std::cout << "==================================================" << std::endl;
    std::cout << "Input file: " << input_file << std::endl;
    std::cout << "" << std::endl;
    
    // Load map from input file
    FileSatelliteView fileMap(input_file);
    
    // Create GameManager with visualization enabled (verbose = true)
    MyGameManager gameManager(true);
    
    // Create objects for the test
    MockPlayer player1, player2;
    RealTankAlgorithmFactory factory1, factory2;
    
    std::cout << "🚀 Starting game simulation with real algorithms..." << std::endl;
    std::cout << "" << std::endl;
    
    // Run the game with visualization using real input
    GameResult result = gameManager.run(
        fileMap.getWidth(), fileMap.getHeight(),  // Map dimensions from file
        fileMap,          // satellite view loaded from file
        0, 25,            // 0 max steps (unlimited - ends by elimination), 25 shells per tank
        player1, player2, // the two players
        factory1, factory2 // tank algorithm factories with real algorithms
    );
    
    std::cout << "" << std::endl;
    std::cout << "🏆 GAME COMPLETED!" << std::endl;
    std::cout << "==================" << std::endl;
    std::cout << "Winner: Player " << result.winner << std::endl;
    std::cout << "Remaining tanks: [" << result.remaining_tanks[0] 
              << ", " << result.remaining_tanks[1] << "]" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "✅ Features Demonstrated:" << std::endl;
    std::cout << "   • Real algorithms controlling tank behavior" << std::endl;
    std::cout << "   • Tanks moving, rotating, and shooting based on AI" << std::endl;
    std::cout << "   • Game ending when one player's tanks are eliminated" << std::endl;
    std::cout << "   • Interactive step-by-step visualization" << std::endl;
    std::cout << "   • Project 2-style enhanced display with emojis" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "🔥 Real tank warfare in action!" << std::endl;
    
    return 0;
}
