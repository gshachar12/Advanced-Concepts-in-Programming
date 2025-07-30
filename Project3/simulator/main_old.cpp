#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#ifdef _WIN32
    #include <windows.h>
    #define dlopen(lib, flags) LoadLibraryA(lib)
    #define dlsym(lib, func) GetProcAddress((HMODULE)lib, func)
    #define dlclose(lib) FreeLibrary((HMODULE)lib)
    #define dlerror() "Windows DLL Error"
    #define RTLD_LAZY 0
#else
    #include <dlfcn.h>
#endif
#include <filesystem>
#include <fstream>
#include <chrono>
#include <map>
#include <algorithm>

#include "../common/AbstractGameManager.h"
#include "../common/Player.h"
#include "../common/TankAlgorithm.h"
#include "../common/SatelliteView.h"
#include "../common/GameResult.h"

namespace fs = std::filesystem;

struct SimulationResult {
    std::string algorithm1;
    std::string algorithm2;
    std::string game_manager;
    std::string map_name;
    GameResult result;
    int steps;
    double time_seconds;
};

class Simulator {
private:
    std::vector<void*> loaded_libraries;
    std::map<std::string, std::function<AbstractGameManager*(bool)>> game_manager_factories;
    std::map<std::string, std::function<std::unique_ptr<Player>()>> player_factories;
    std::map<std::string, std::function<std::unique_ptr<TankAlgorithm>()>> algorithm_factories;
    
    std::vector<SimulationResult> results;
    std::mutex results_mutex;
    
public:
    ~Simulator() {
        // Close all loaded libraries
        for (void* lib : loaded_libraries) {
            dlclose(lib);
        }
    }
    
    bool loadLibrary(const std::string& library_path) {
        void* handle = dlopen(library_path.c_str(), RTLD_LAZY);
        if (!handle) {
            std::cerr << "Cannot load library: " << dlerror() << std::endl;
            return false;
        }
        
        loaded_libraries.push_back(handle);
        
        // Try to load GameManager factory
        auto gm_factory = (AbstractGameManager*(*)(bool))dlsym(handle, "createGameManager");
        if (gm_factory) {
            std::string name = fs::path(library_path).stem().string();
            game_manager_factories[name] = gm_factory;
            std::cout << "Loaded GameManager: " << name << std::endl;
        }
        
        // Try to load Player factory
        auto player_factory = (std::unique_ptr<Player>(*)())dlsym(handle, "createPlayer");
        if (player_factory) {
            std::string name = fs::path(library_path).stem().string();
            player_factories[name] = player_factory;
            std::cout << "Loaded Player: " << name << std::endl;
        }
        
        // Try to load TankAlgorithm factory
        auto algo_factory = (std::unique_ptr<TankAlgorithm>(*)())dlsym(handle, "createTankAlgorithm");
        if (algo_factory) {
            std::string name = fs::path(library_path).stem().string();
            algorithm_factories[name] = algo_factory;
            std::cout << "Loaded Algorithm: " << name << std::endl;
        }
        
        return true;
    }
    
    void loadAllLibraries() {
        // Load from common directory structure
        for (const auto& dir : {"../Algorithm", "../GameManager"}) {
            if (fs::exists(dir)) {
                for (const auto& entry : fs::directory_iterator(dir)) {
#ifdef _WIN32
                    if (entry.path().extension() == ".dll") {
#else
                    if (entry.path().extension() == ".so") {
#endif
                        loadLibrary(entry.path().string());
                    }
                }
            }
        }
    }
    
    void runComparative(const std::vector<std::string>& map_files, bool verbose) {
        std::cout << "Running comparative simulation..." << std::endl;
        
        for (const auto& map_file : map_files) {
            std::cout << "Processing map: " << map_file << std::endl;
            
            // For each combination of algorithms and game managers
            for (const auto& [gm_name, gm_factory] : game_manager_factories) {
                for (const auto& [algo1_name, algo1_factory] : algorithm_factories) {
                    for (const auto& [algo2_name, algo2_factory] : algorithm_factories) {
                        if (algo1_name != algo2_name) { // Don't run against self
                            runSingleGame(map_file, gm_name, algo1_name, algo2_name, verbose);
                        }
                    }
                }
            }
        }
        
        printComparativeResults();
    }
    
    void runCompetition(const std::vector<std::string>& map_files, bool verbose, int num_threads) {
        std::cout << "Running competition simulation with " << num_threads << " threads..." << std::endl;
        
        std::vector<std::thread> threads;
        
        for (int t = 0; t < num_threads; ++t) {
            threads.emplace_back([this, &map_files, verbose, t, num_threads]() {
                for (size_t i = t; i < map_files.size(); i += num_threads) {
                    const auto& map_file = map_files[i];
                    
                    // Run all combinations for this map
                    for (const auto& [gm_name, gm_factory] : game_manager_factories) {
                        for (const auto& [algo1_name, algo1_factory] : algorithm_factories) {
                            for (const auto& [algo2_name, algo2_factory] : algorithm_factories) {
                                if (algo1_name != algo2_name) {
                                    runSingleGame(map_file, gm_name, algo1_name, algo2_name, verbose);
                                }
                            }
                        }
                    }
                }
            });
        }
        
        // Wait for all threads to complete
        for (auto& thread : threads) {
            thread.join();
        }
        
        printCompetitionResults();
    }
    
private:
    void runSingleGame(const std::string& map_file, const std::string& gm_name, 
                      const std::string& algo1_name, const std::string& algo2_name, bool verbose) {
        try {
            auto start_time = std::chrono::high_resolution_clock::now();
            
            // Create game manager
            auto gm = game_manager_factories[gm_name](verbose);
            
            // Create algorithms
            auto algo1 = algorithm_factories[algo1_name]();
            auto algo2 = algorithm_factories[algo2_name]();
            
            // Load map (simplified - would need proper map loading)
            // For now, create a simple test map
            size_t width = 10, height = 10, max_steps = 100, num_shells = 3;
            
            // Create mock satellite view and players
            // This would need to be implemented based on actual map file format
            
            auto end_time = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
            
            SimulationResult result;
            result.algorithm1 = algo1_name;
            result.algorithm2 = algo2_name;
            result.game_manager = gm_name;
            result.map_name = fs::path(map_file).stem().string();
            result.time_seconds = duration.count() / 1000.0;
            
            {
                std::lock_guard<std::mutex> lock(results_mutex);
                results.push_back(result);
            }
            
            delete gm;
            
        } catch (const std::exception& e) {
            std::cerr << "Error in game " << algo1_name << " vs " << algo2_name 
                     << " with " << gm_name << ": " << e.what() << std::endl;
        }
    }
    
    void printComparativeResults() {
        std::cout << "\n=== COMPARATIVE RESULTS ===" << std::endl;
        std::cout << "Algorithm1 vs Algorithm2 | GameManager | Map | Winner | Time(s)" << std::endl;
        std::cout << "--------------------------------------------------------" << std::endl;
        
        for (const auto& result : results) {
            std::cout << result.algorithm1 << " vs " << result.algorithm2 << " | "
                     << result.game_manager << " | " << result.map_name << " | ";
            
            switch (result.result.winner) {
                case GameResult::PLAYER1: std::cout << result.algorithm1; break;
                case GameResult::PLAYER2: std::cout << result.algorithm2; break;
                case GameResult::TIE: std::cout << "TIE"; break;
            }
            
            std::cout << " | " << result.time_seconds << std::endl;
        }
    }
    
    void printCompetitionResults() {
        std::cout << "\n=== COMPETITION RESULTS ===" << std::endl;
        
        // Calculate scores for each algorithm
        std::map<std::string, int> scores;
        for (const auto& result : results) {
            if (result.result.winner == GameResult::PLAYER1) {
                scores[result.algorithm1] += 3;
            } else if (result.result.winner == GameResult::PLAYER2) {
                scores[result.algorithm2] += 3;
            } else {
                scores[result.algorithm1] += 1;
                scores[result.algorithm2] += 1;
            }
        }
        
        // Sort by score
        std::vector<std::pair<std::string, int>> sorted_scores(scores.begin(), scores.end());
        std::sort(sorted_scores.begin(), sorted_scores.end(), 
                 [](const auto& a, const auto& b) { return a.second > b.second; });
        
        std::cout << "Final Rankings:" << std::endl;
        for (size_t i = 0; i < sorted_scores.size(); ++i) {
            std::cout << (i + 1) << ". " << sorted_scores[i].first 
                     << " - " << sorted_scores[i].second << " points" << std::endl;
        }
    }
};

void printUsage(const char* program_name) {
    std::cout << "Usage: " << program_name << " [options] [map_files...]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -comparative  Run comparative analysis" << std::endl;
    std::cout << "  -competition  Run competition mode" << std::endl;
    std::cout << "  -verbose      Enable verbose output" << std::endl;
    std::cout << "  -threads N    Number of threads for competition mode (default: 4)" << std::endl;
}

int main(int argc, char* argv[]) {
    bool comparative = false;
    bool competition = false;
    bool verbose = false;
    int num_threads = 4;
    std::vector<std::string> map_files;
    
    // Parse command line arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        
        if (arg == "-comparative") {
            comparative = true;
        } else if (arg == "-competition") {
            competition = true;
        } else if (arg == "-verbose") {
            verbose = true;
        } else if (arg == "-threads" && i + 1 < argc) {
            num_threads = std::stoi(argv[++i]);
        } else if (arg[0] != '-') {
            map_files.push_back(arg);
        } else {
            std::cerr << "Unknown option: " << arg << std::endl;
            printUsage(argv[0]);
            return 1;
        }
    }
    
    if (!comparative && !competition) {
        std::cerr << "Must specify either -comparative or -competition mode" << std::endl;
        printUsage(argv[0]);
        return 1;
    }
    
    if (map_files.empty()) {
        std::cerr << "No map files specified" << std::endl;
        printUsage(argv[0]);
        return 1;
    }
    
    Simulator simulator;
    simulator.loadAllLibraries();
    
    if (comparative) {
        simulator.runComparative(map_files, verbose);
    } else {
        simulator.runCompetition(map_files, verbose, num_threads);
    }
    
    return 0;
}
