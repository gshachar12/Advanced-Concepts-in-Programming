#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "AlgorithmRegistrar.h"
#include <string>
#include <vector>
#include <memory>

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

/**
 * Simulator class handles dynamic loading of algorithm plugins
 * Implements the core plugin architecture required by the assignment
 */
class Simulator {
private:
    std::vector<void*> loaded_libraries;
    std::string algorithm_directory;
    
public:
    Simulator(const std::string& algo_dir = "algorithms/");
    ~Simulator();
    
    /**
     * Load all algorithm plugins from the specified directory
     */
    bool loadAllAlgorithms();
    
    /**
     * Load a specific algorithm plugin
     */
    bool loadAlgorithm(const std::string& algorithm_name);
    
    /**
     * Unload all loaded plugins
     */
    void unloadAllAlgorithms();
    
    /**
     * Get list of available algorithm files in directory
     */
    std::vector<std::string> discoverAlgorithmFiles();
    
    /**
     * Extract algorithm name from filename (remove .so/.dll extension)
     */
    std::string extractAlgorithmName(const std::string& filename);
    
private:
    /**
     * Platform-specific dynamic library loading
     */
    void* loadLibrary(const std::string& filepath);
    void unloadLibrary(void* handle);
    
    /**
     * Validate that a loaded algorithm registered properly
     */
    bool validateAlgorithmRegistration(const std::string& algorithm_name);
};

#endif // SIMULATOR_H
