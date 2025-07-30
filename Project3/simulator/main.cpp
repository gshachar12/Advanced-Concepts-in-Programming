#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <map>
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <chrono>
#include <sstream>
#include <iomanip>

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

#include "../common/AbstractGameManager.h"
#include "../common/Player.h"
#include "../common/TankAlgorithm.h"
#include "../common/SatelliteView.h"
#include "../common/GameResult.h"

namespace fs = std::filesystem;

struct CommandLineArgs {
    bool comparative_mode = false;
    bool competition_mode = false;
    bool verbose = false;
    int num_threads = 1;
    std::string game_map;
    std::string game_maps_folder;
    std::string game_managers_folder;
    std::string game_manager;
    std::string algorithms_folder;
    std::string algorithm1;
    std::string algorithm2;
};

class Simulator {
private:
    std::vector<void*> loaded_libraries;
    std::map<std::string, std::function<std::unique_ptr<AbstractGameManager>(bool)>> game_manager_factories;
    std::map<std::string, std::function<std::unique_ptr<Player>(int, size_t, size_t, size_t, size_t)>> player_factories;
    std::map<std::string, std::function<std::unique_ptr<TankAlgorithm>(int, int)>> algorithm_factories;
    
    std::vector<GameResult> results;
    std::mutex results_mutex;
    
    std::string getCurrentTimeString() {
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch()) % 1000;
        
        std::stringstream ss;
        ss << std::put_time(std::localtime(&time_t), "%Y%m%d_%H%M%S");
        ss << "_" << std::setfill('0') << std::setw(3) << ms.count();
        return ss.str();
    }

public:
    ~Simulator() {
        for (void* lib : loaded_libraries) {
            dlclose(lib);
        }
    }
    
    bool loadLibrary(const std::string& library_path) {
        void* handle = dlopen(library_path.c_str(), RTLD_LAZY);
        if (!handle) {
            std::cerr << "Cannot load library " << library_path << ": " << dlerror() << std::endl;
            return false;
        }
        
        loaded_libraries.push_back(handle);
        
        // Try to load GameManager factory
        auto gm_factory = (std::unique_ptr<AbstractGameManager>(*)(bool))dlsym(handle, "createGameManager");
        if (gm_factory) {
            std::string name = fs::path(library_path).stem().string();
            game_manager_factories[name] = gm_factory;
        }
        
        // Try to load Player factory  
        auto player_factory = (std::unique_ptr<Player>(*)(int, size_t, size_t, size_t, size_t))dlsym(handle, "createPlayer");
        if (player_factory) {
            std::string name = fs::path(library_path).stem().string();
            player_factories[name] = player_factory;
        }
        
        // Try to load TankAlgorithm factory
        auto algo_factory = (std::unique_ptr<TankAlgorithm>(*)(int, int))dlsym(handle, "createTankAlgorithm");
        if (algo_factory) {
            std::string name = fs::path(library_path).stem().string();
            algorithm_factories[name] = algo_factory;
        }
        
        return true;
    }
    
    void loadLibrariesFromFolder(const std::string& folder_path) {
        if (!fs::exists(folder_path) || !fs::is_directory(folder_path)) {
            std::cerr << "Error: Folder " << folder_path << " does not exist or is not a directory" << std::endl;
            return;
        }
        
        int loaded_count = 0;
        for (const auto& entry : fs::directory_iterator(folder_path)) {
            if (entry.is_regular_file()) {
                std::string ext = entry.path().extension().string();
                if (ext == ".so" || ext == ".dll") {
                    if (loadLibrary(entry.path().string())) {
                        loaded_count++;
                    }
                }
            }
        }
        
        if (loaded_count == 0) {
            std::cerr << "Error: No valid shared libraries found in " << folder_path << std::endl;
        }
    }
    
    bool runComparative(const CommandLineArgs& args) {
        // Load game managers from folder
        loadLibrariesFromFolder(args.game_managers_folder);
        
        // Load specific algorithms
        if (!loadLibrary(args.algorithm1) || !loadLibrary(args.algorithm2)) {
            return false;
        }
        
        if (game_manager_factories.empty()) {
            std::cerr << "Error: No game managers loaded" << std::endl;
            return false;
        }
        
        // Run comparative analysis
        std::map<std::string, std::vector<std::string>> result_groups;
        
        for (const auto& [gm_name, gm_factory] : game_manager_factories) {
            // TODO: Run game with both algorithms and collect results
            // For now, placeholder implementation
        }
        
        // Write results to file
        std::string output_file = args.game_managers_folder + "/comparative_results_" + getCurrentTimeString() + ".txt";
        std::ofstream outfile(output_file);
        if (!outfile.is_open()) {
            std::cerr << "Error: Cannot create output file " << output_file << std::endl;
            std::cout << "Results would be printed to screen instead:" << std::endl;
            // Print to screen instead
            return false;
        }
        
        outfile << "game_map=" << args.game_map << std::endl;
        outfile << "algorithm1=" << args.algorithm1 << std::endl;
        outfile << "algorithm2=" << args.algorithm2 << std::endl;
        outfile << std::endl;
        
        // TODO: Write actual results
        
        outfile.close();
        return true;
    }
    
    bool runCompetition(const CommandLineArgs& args) {
        // Load game manager
        if (!loadLibrary(args.game_manager)) {
            return false;
        }
        
        // Load algorithms from folder
        loadLibrariesFromFolder(args.algorithms_folder);
        
        if (algorithm_factories.size() < 2) {
            std::cerr << "Error: Competition requires at least 2 algorithms, found " << algorithm_factories.size() << std::endl;
            return false;
        }
        
        // Load game maps
        std::vector<std::string> map_files;
        for (const auto& entry : fs::directory_iterator(args.game_maps_folder)) {
            if (entry.is_regular_file() && entry.path().extension() == ".txt") {
                map_files.push_back(entry.path().string());
            }
        }
        
        if (map_files.empty()) {
            std::cerr << "Error: No game maps found in " << args.game_maps_folder << std::endl;
            return false;
        }
        
        // Competition algorithm pairing as specified in assignment
        std::vector<std::string> algorithm_names;
        for (const auto& [name, _] : algorithm_factories) {
            algorithm_names.push_back(name);
        }
        
        std::map<std::string, int> scores;
        for (const auto& name : algorithm_names) {
            scores[name] = 0;
        }
        
        // Run competition with proper threading
        int actual_threads = (args.num_threads == 1) ? 1 : args.num_threads;
        if (actual_threads == 2) actual_threads = 3; // Assignment: never exactly 2 threads
        
        // TODO: Implement competition logic with proper pairing algorithm
        
        // Write results to file
        std::string output_file = args.algorithms_folder + "/competition_" + getCurrentTimeString() + ".txt";
        std::ofstream outfile(output_file);
        if (!outfile.is_open()) {
            std::cerr << "Error: Cannot create output file " << output_file << std::endl;
            return false;
        }
        
        outfile << "game_maps_folder=" << args.game_maps_folder << std::endl;
        outfile << "game_manager=" << args.game_manager << std::endl;
        outfile << std::endl;
        
        // Sort algorithms by score
        std::vector<std::pair<std::string, int>> sorted_scores(scores.begin(), scores.end());
        std::sort(sorted_scores.begin(), sorted_scores.end(), 
                  [](const auto& a, const auto& b) { return a.second > b.second; });
        
        for (const auto& [name, score] : sorted_scores) {
            outfile << name << " " << score << std::endl;
        }
        
        outfile.close();
        return true;
    }
};

CommandLineArgs parseCommandLine(int argc, char* argv[]) {
    CommandLineArgs args;
    
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        
        if (arg == "-comparative") {
            args.comparative_mode = true;
        } else if (arg == "-competition") {
            args.competition_mode = true;
        } else if (arg == "-verbose") {
            args.verbose = true;
        } else if (arg.substr(0, 9) == "game_map=") {
            args.game_map = arg.substr(9);
        } else if (arg.substr(0, 17) == "game_maps_folder=") {
            args.game_maps_folder = arg.substr(17);
        } else if (arg.substr(0, 21) == "game_managers_folder=") {
            args.game_managers_folder = arg.substr(21);
        } else if (arg.substr(0, 13) == "game_manager=") {
            args.game_manager = arg.substr(13);
        } else if (arg.substr(0, 18) == "algorithms_folder=") {
            args.algorithms_folder = arg.substr(18);
        } else if (arg.substr(0, 11) == "algorithm1=") {
            args.algorithm1 = arg.substr(11);
        } else if (arg.substr(0, 11) == "algorithm2=") {
            args.algorithm2 = arg.substr(11);
        } else if (arg.substr(0, 12) == "num_threads=") {
            args.num_threads = std::stoi(arg.substr(12));
        }
    }
    
    return args;
}

void printUsage(const std::string& program_name) {
    std::cout << "Usage:" << std::endl;
    std::cout << "Comparative mode:" << std::endl;
    std::cout << "  " << program_name << " -comparative game_map=<file> game_managers_folder=<folder> algorithm1=<so> algorithm2=<so> [num_threads=<num>] [-verbose]" << std::endl;
    std::cout << std::endl;
    std::cout << "Competition mode:" << std::endl;
    std::cout << "  " << program_name << " -competition game_maps_folder=<folder> game_manager=<so> algorithms_folder=<folder> [num_threads=<num>] [-verbose]" << std::endl;
}

bool validateArgs(const CommandLineArgs& args) {
    if (!args.comparative_mode && !args.competition_mode) {
        std::cerr << "Error: Must specify either -comparative or -competition mode" << std::endl;
        return false;
    }
    
    if (args.comparative_mode && args.competition_mode) {
        std::cerr << "Error: Cannot specify both -comparative and -competition modes" << std::endl;
        return false;
    }
    
    if (args.comparative_mode) {
        if (args.game_map.empty()) {
            std::cerr << "Error: Missing required argument: game_map" << std::endl;
            return false;
        }
        if (args.game_managers_folder.empty()) {
            std::cerr << "Error: Missing required argument: game_managers_folder" << std::endl;
            return false;
        }
        if (args.algorithm1.empty()) {
            std::cerr << "Error: Missing required argument: algorithm1" << std::endl;
            return false;
        }
        if (args.algorithm2.empty()) {
            std::cerr << "Error: Missing required argument: algorithm2" << std::endl;
            return false;
        }
    }
    
    if (args.competition_mode) {
        if (args.game_maps_folder.empty()) {
            std::cerr << "Error: Missing required argument: game_maps_folder" << std::endl;
            return false;
        }
        if (args.game_manager.empty()) {
            std::cerr << "Error: Missing required argument: game_manager" << std::endl;
            return false;
        }
        if (args.algorithms_folder.empty()) {
            std::cerr << "Error: Missing required argument: algorithms_folder" << std::endl;
            return false;
        }
    }
    
    return true;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printUsage(argv[0]);
        return 1;
    }
    
    CommandLineArgs args = parseCommandLine(argc, argv);
    
    if (!validateArgs(args)) {
        printUsage(argv[0]);
        return 1;
    }
    
    Simulator simulator;
    
    bool success = false;
    if (args.comparative_mode) {
        success = simulator.runComparative(args);
    } else if (args.competition_mode) {
        success = simulator.runCompetition(args);
    }
    
    return success ? 0 : 1;
}
