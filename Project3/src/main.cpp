#include "GameManager.h"
#include "VisualizingGameManager.h"
#include "algorithms/AlgorithmFactory.h"
#include "algorithms/TournamentManager.h"
#include "simulator/Simulator.h"
#include "simulator/AlgorithmRegistrar.h"
#include "../common/SatelliteView.h"
#include "CellType.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <sstream>

/**
 * Enhanced map loader that properly parses Project 2-style input files
 */
class FileMap : public SatelliteView {
private:
    std::vector<std::string> map_data_;
    size_t width_, height_;
    size_t max_steps_, num_shells_;
    std::string description_;
    
public:
    FileMap(const std::string& filename) : width_(0), height_(0), max_steps_(100), num_shells_(5) {
        loadFromFile(filename);
    }
    
    char getObjectAt(size_t x, size_t y) const override {
        if (y >= map_data_.size() || x >= map_data_[y].size()) {
            return '#'; // Wall for out of bounds
        }
        return map_data_[y][x];
    }
    
    size_t getWidth() const { return width_; }
    size_t getHeight() const { return height_; }
    size_t getMaxSteps() const { return max_steps_; }
    size_t getNumShells() const { return num_shells_; }
    const std::string& getDescription() const { return description_; }
    
private:
    void loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "[ERROR] Could not open map file: " << filename << std::endl;
            createDefaultMap();
            return;
        }
        
        std::string line;
        
        // Parse Project 2-style header
        if (std::getline(file, line)) {
            description_ = line; // e.g., "2 - Tactical Showdown"
        }
        
        // Parse MaxSteps=4500
        if (std::getline(file, line)) {
            if (line.find("MaxSteps=") == 0) {
                max_steps_ = std::stoi(line.substr(9));
            }
        }
        
        // Parse NumShells=30
        if (std::getline(file, line)) {
            if (line.find("NumShells=") == 0) {
                num_shells_ = std::stoi(line.substr(10));
            }
        }
        
        // Parse Rows=12
        size_t expected_height = 0;
        if (std::getline(file, line)) {
            if (line.find("Rows=") == 0) {
                expected_height = std::stoi(line.substr(5));
            }
        }
        
        // Parse Cols=18
        size_t expected_width = 0;
        if (std::getline(file, line)) {
            if (line.find("Cols=") == 0) {
                expected_width = std::stoi(line.substr(5));
            }
        }
        
        // Now read the actual map data
        width_ = 0;
        while (std::getline(file, line)) {
            map_data_.push_back(line);
            width_ = std::max(width_, line.length());
        }
        height_ = map_data_.size();
        
        // Validate dimensions match header
        if (expected_height > 0 && height_ != expected_height) {
            std::cerr << "[WARNING] Map height mismatch: expected " << expected_height 
                      << ", got " << height_ << std::endl;
        }
        if (expected_width > 0 && width_ != expected_width) {
            std::cerr << "[WARNING] Map width mismatch: expected " << expected_width 
                      << ", got " << width_ << std::endl;
        }
        
        if (height_ == 0) {
            std::cerr << "[ERROR] Empty map file, creating default map" << std::endl;
            createDefaultMap();
        } else {
            std::cout << "[SUCCESS] Loaded map: " << width_ << "x" << height_ << " from " << filename << std::endl;
            std::cout << "[INFO] Game parameters: MaxSteps=" << max_steps_ << ", NumShells=" << num_shells_ << std::endl;
        }
    }
    
    void createDefaultMap() {
        // Create a simple default 10x10 map
        width_ = 10;
        height_ = 10;
        map_data_.clear();
        
        for (size_t y = 0; y < height_; ++y) {
            std::string row;
            for (size_t x = 0; x < width_; ++x) {
                if (x == 0 || y == 0 || x == width_ - 1 || y == height_ - 1) {
                    row += '#'; // Border walls
                } else if (x == 1 && y == 1) {
                    row += '1'; // Player 1 tank
                } else if (x == width_ - 2 && y == height_ - 2) {
                    row += '2'; // Player 2 tank
                } else if ((x == width_ / 2 && y == height_ / 2) ||
                          (x == width_ / 2 + 1 && y == height_ / 2)) {
                    row += '#'; // Some obstacles
                } else {
                    row += ' '; // Empty space
                }
            }
            map_data_.push_back(row);
        }
        std::cout << "[SUCCESS] Created default " << width_ << "x" << height_ << " map" << std::endl;
    }
};

void printUsage(const char* program_name) {
    std::cout << "Usage: " << program_name << " [options]\n";
    std::cout << "\nOptions:\n";
    std::cout << "  -map <file>         Map file to load (default: creates default map)\n";
    std::cout << "  -steps <num>        Maximum steps per game (default: 100)\n";
    std::cout << "  -shells <num>       Shells per tank (default: 5)\n";
    std::cout << "  -algo1 <name>       Algorithm for player 1 (default: Simple)\n";
    std::cout << "  -algo2 <name>       Algorithm for player 2 (default: BFS)\n";
    std::cout << "  -verbose            Enable verbose output\n";
    std::cout << "  -visualize          Enable board visualization\n";
    std::cout << "  -tournament         Run tournament mode\n";
    std::cout << "  -games <num>        Games per tournament matchup (default: 3)\n";
    std::cout << "  -list               List available algorithms\n";
    std::cout << "  -help               Show this help message\n";
    std::cout << "\nAvailable Algorithms:\n";
    
    // List algorithms
    const auto& algorithms = AlgorithmRegistrar::getRegisteredAlgorithms();
    for (const auto& algo : algorithms) {
        std::cout << "  " << algo.name << " - " << algo.description << "\n";
    }
    
    std::cout << "\nExample:\n";
    std::cout << "  " << program_name << " -map board.txt -algo1 Simple -algo2 BFS -steps 50 -verbose\n";
    std::cout << "  " << program_name << " -tournament -games 5\n";
}

/**
 * Unified algorithm lookup that searches both built-in and dynamic algorithms
 */
struct UnifiedAlgorithmInfo {
    std::string name;
    TankAlgorithmFactory tank_factory;
    PlayerFactory player_factory;
    bool is_dynamic;
};

UnifiedAlgorithmInfo* findUnifiedAlgorithm(const std::string& name) {
    static std::vector<UnifiedAlgorithmInfo> unified_cache;
    
    // Clear cache and rebuild
    unified_cache.clear();
    
    // Add built-in algorithms
    const auto& builtin_algorithms = AlgorithmRegistrar::getRegisteredAlgorithms();
    for (const auto& algo : builtin_algorithms) {
        unified_cache.push_back({
            algo.name,
            algo.factory,
            algo.player_factory,
            false
        });
    }
    
    // Add dynamic algorithms
    auto& dynamic_registrar = DynamicAlgorithmRegistrar::getAlgorithmRegistrar();
    for (const auto& algo : dynamic_registrar) {
        unified_cache.push_back({
            algo.name(),
            [algo](int pi, int ti) { return algo.createTankAlgorithm(pi, ti); },
            [algo](int pi, size_t x, size_t y, size_t ms, size_t ns) { return algo.createPlayer(pi, x, y, ms, ns); },
            true
        });
    }
    
    // Search for algorithm
    for (auto& algo : unified_cache) {
        if (algo.name == name) {
            return &algo;
        }
    }
    
    return nullptr;
}

int main(int argc, char* argv[]) {
    // Initialize dynamic loading system
    Simulator simulator("plugins/");
    
    // Load built-in algorithms as fallback
    AlgorithmRegistrar::registerDefaultAlgorithms();
    
    // Try to load dynamic plugins
    if (!simulator.loadAllAlgorithms()) {
        std::cout << "[INFO] No dynamic plugins found, using built-in algorithms only\n";
    }
    
    // Default parameters
    std::string map_file = "";
    std::string algo1_name = "Simple";
    std::string algo2_name = "BFS";
    size_t max_steps = 100;
    size_t shells_per_tank = 5;
    bool verbose = false;
    bool visualize = false;
    bool tournament_mode = false;
    int tournament_games = 3;
    
    // Parse command line arguments
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        
        if (arg == "-help" || arg == "--help" || arg == "-h") {
            printUsage(argv[0]);
            return 0;
        } else if (arg == "-list") {
            std::cout << "Available Algorithms:\n";
            std::cout << "Built-in algorithms:\n";
            const auto& builtin_algorithms = AlgorithmRegistrar::getRegisteredAlgorithms();
            for (const auto& algo : builtin_algorithms) {
                std::cout << "  * " << algo.name << ": " << algo.description << "\n";
            }
            
            std::cout << "\nDynamic plugin algorithms:\n";
            auto& dynamic_registrar = DynamicAlgorithmRegistrar::getAlgorithmRegistrar();
            if (dynamic_registrar.count() > 0) {
                for (const auto& algo : dynamic_registrar) {
                    std::cout << "  * " << algo.name() << " (plugin)\n";
                }
            } else {
                std::cout << "  (none loaded)\n";
            }
            return 0;
        } else if (arg == "-map" && i + 1 < argc) {
            map_file = argv[++i];
        } else if (arg == "-steps" && i + 1 < argc) {
            max_steps = std::stoul(argv[++i]);
        } else if (arg == "-shells" && i + 1 < argc) {
            shells_per_tank = std::stoul(argv[++i]);
        } else if (arg == "-algo1" && i + 1 < argc) {
            algo1_name = argv[++i];
        } else if (arg == "-algo2" && i + 1 < argc) {
            algo2_name = argv[++i];
        } else if (arg == "-verbose") {
            verbose = true;
        } else if (arg == "-visualize") {
            visualize = true;
        } else if (arg == "-tournament") {
            tournament_mode = true;
        } else if (arg == "-games" && i + 1 < argc) {
            tournament_games = std::stoi(argv[++i]);
        } else {
            std::cerr << "[ERROR] Unknown argument: " << arg << std::endl;
            printUsage(argv[0]);
            return 1;
        }
    }
    
    std::cout << "Tank Battle Game - Project 3\n";
    std::cout << "===============================\n\n";
    
    if (tournament_mode) {
        std::cout << "Tournament Mode\n";
        std::cout << "---------------\n";
        
        TournamentManager tournament(10, 10, max_steps, shells_per_tank, verbose);
        tournament.addAllRegisteredAlgorithms();
        tournament.runRoundRobin(tournament_games);
        tournament.printTournamentResults();
        tournament.printAlgorithmRankings();
        
    } else {
        std::cout << "Single Game Mode\n";
        std::cout << "----------------\n";
        
        // Validate algorithms
        auto* algo1_info = findUnifiedAlgorithm(algo1_name);
        auto* algo2_info = findUnifiedAlgorithm(algo2_name);
        
        if (!algo1_info) {
            std::cerr << "[ERROR] Algorithm not found: " << algo1_name << std::endl;
            return 1;
        }
        
        if (!algo2_info) {
            std::cerr << "[ERROR] Algorithm not found: " << algo2_name << std::endl;
            return 1;
        }
        
        // Load or create map
        std::unique_ptr<FileMap> map;
        if (!map_file.empty()) {
            map = std::make_unique<FileMap>(map_file);
            // Use values from file if not overridden by command line
            if (max_steps == 100) max_steps = map->getMaxSteps(); // Use file value if default
            if (shells_per_tank == 5) shells_per_tank = map->getNumShells(); // Use file value if default
        } else {
            map = std::make_unique<FileMap>(""); // Will create default map
        }
        
        // Create game manager
        std::unique_ptr<GameManager> manager;
        if (visualize) {
            manager = std::make_unique<VisualizingGameManager>(verbose);
            std::cout << "   [INFO] Visual mode enabled - rich display with emojis\n";
        } else {
            manager = std::make_unique<GameManager>(verbose);
        }
        
        // Create players
        auto player1 = algo1_info->player_factory(0, 1, 1, max_steps, shells_per_tank);
        auto player2 = algo2_info->player_factory(1, map->getWidth() - 2, map->getHeight() - 2, max_steps, shells_per_tank);
        
        std::cout << "Starting game:\n";
        std::cout << "   Player 1: " << algo1_name << (algo1_info->is_dynamic ? " (plugin)" : " (built-in)") << "\n";
        std::cout << "   Player 2: " << algo2_name << (algo2_info->is_dynamic ? " (plugin)" : " (built-in)") << "\n";
        std::cout << "   Map: " << map->getWidth() << "x" << map->getHeight() << "\n";
        std::cout << "   Max Steps: " << max_steps << "\n";
        std::cout << "   Shells per Tank: " << shells_per_tank << "\n";
        std::cout << "   Verbose: " << (verbose ? "ON" : "OFF") << "\n";
        std::cout << "   Visualization: " << (visualize ? "ON (Rich Project2-style display)" : "OFF") << "\n\n";
        
        // Run the game
        GameResult result = manager->run(
            map->getWidth(), map->getHeight(),
            *map,
            max_steps,
            shells_per_tank,
            *player1, *player2,
            algo1_info->tank_factory,
            algo2_info->tank_factory
        );
        
        // Display result
        std::cout << "\nGame Result:\n";
        if (result.winner == 1) {
            std::cout << "WINNER: " << algo1_name << " (Player 1)\n";
        } else if (result.winner == 2) {
            std::cout << "WINNER: " << algo2_name << " (Player 2)\n";
        } else {
            std::cout << "RESULT: Game ended in a TIE\n";
        }
    }
    
    return 0;
}
