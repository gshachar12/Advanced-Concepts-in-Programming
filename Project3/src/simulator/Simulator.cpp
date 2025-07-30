#include "Simulator.h"
#include <iostream>
#include <filesystem>
#in    } catch (DynamicAlgorithmRegistrar::BadRegistrationException& e) {lude <algorithm>

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

Simulator::Simulator(const std::string& algo_dir) 
    : algorithm_directory(algo_dir) {
}

Simulator::~Simulator() {
    unloadAllAlgorithms();
}

bool Simulator::loadAllAlgorithms() {
    std::cout << "[SIMULATOR] Loading algorithms from directory: " << algorithm_directory << std::endl;
    
    auto algorithm_files = discoverAlgorithmFiles();
    if (algorithm_files.empty()) {
        std::cout << "[SIMULATOR] No algorithm files found in directory" << std::endl;
        return false;
    }
    
    bool all_loaded = true;
    for (const auto& filename : algorithm_files) {
        std::string algorithm_name = extractAlgorithmName(filename);
        if (!loadAlgorithm(algorithm_name)) {
            all_loaded = false;
        }
    }
    
    std::cout << "[SIMULATOR] Loaded " << DynamicAlgorithmRegistrar::getAlgorithmRegistrar().count() 
              << " algorithms successfully" << std::endl;
    
    return all_loaded;
}

bool Simulator::loadAlgorithm(const std::string& algorithm_name) {
    std::cout << "[SIMULATOR] Loading algorithm: " << algorithm_name << std::endl;
    
    // Create algorithm factory entry first
    auto& registrar = DynamicAlgorithmRegistrar::getAlgorithmRegistrar();
    registrar.createAlgorithmFactoryEntry(algorithm_name);
    
#ifdef _WIN32
    std::string filepath = algorithm_directory + algorithm_name + ".dll";
#else
    std::string filepath = algorithm_directory + algorithm_name + ".so";
#endif
    
    // Try to load the library
    void* handle = loadLibrary(filepath);
    if (!handle) {
        std::cerr << "[SIMULATOR] Failed to load library: " << filepath << std::endl;
        registrar.removeLast();
        return false;
    }
    
    loaded_libraries.push_back(handle);
    
    // Validate registration
    try {
        registrar.validateLastRegistration();
        std::cout << "[SIMULATOR] Successfully loaded and validated: " << algorithm_name << std::endl;
        return true;
    }
    catch (AlgorithmRegistrar::BadRegistrationException& e) {
        std::cerr << "[SIMULATOR] BadRegistrationException for: " << algorithm_name << std::endl;
        std::cerr << "  Name as registered: " << e.name << std::endl;
        std::cerr << "  Has tank algorithm factory? " << std::boolalpha << e.hasTankAlgorithmFactory << std::endl;
        std::cerr << "  Has Player factory? " << std::boolalpha << e.hasPlayerFactory << std::endl;
        
        // Remove the failed registration
        registrar.removeLast();
        
        // Unload the library
        unloadLibrary(handle);
        loaded_libraries.pop_back();
        
        return false;
    }
}

void Simulator::unloadAllAlgorithms() {
    std::cout << "[SIMULATOR] Unloading all algorithms..." << std::endl;
    
    // Clear registrations
    DynamicAlgorithmRegistrar::getAlgorithmRegistrar().clear();
    
    // Unload all libraries
    for (void* handle : loaded_libraries) {
        if (handle) {
            unloadLibrary(handle);
        }
    }
    loaded_libraries.clear();
    
    std::cout << "[SIMULATOR] All algorithms unloaded" << std::endl;
}

std::vector<std::string> Simulator::discoverAlgorithmFiles() {
    std::vector<std::string> algorithm_files;
    
    try {
        if (!std::filesystem::exists(algorithm_directory)) {
            std::cout << "[SIMULATOR] Algorithm directory does not exist: " << algorithm_directory << std::endl;
            return algorithm_files;
        }
        
        for (const auto& entry : std::filesystem::directory_iterator(algorithm_directory)) {
            if (entry.is_regular_file()) {
                std::string filename = entry.path().filename().string();
                
#ifdef _WIN32
                if (filename.ends_with(".dll")) {
#else
                if (filename.ends_with(".so")) {
#endif
                    algorithm_files.push_back(filename);
                }
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << "[SIMULATOR] Error discovering algorithm files: " << e.what() << std::endl;
    }
    
    return algorithm_files;
}

std::string Simulator::extractAlgorithmName(const std::string& filename) {
    std::string name = filename;
    
    // Remove .so or .dll extension
#ifdef _WIN32
    if (name.ends_with(".dll")) {
        name = name.substr(0, name.length() - 4);
    }
#else
    if (name.ends_with(".so")) {
        name = name.substr(0, name.length() - 3);
    }
#endif
    
    return name;
}

void* Simulator::loadLibrary(const std::string& filepath) {
#ifdef _WIN32
    HMODULE handle = LoadLibraryA(filepath.c_str());
    if (!handle) {
        std::cerr << "[SIMULATOR] LoadLibrary failed for: " << filepath 
                  << " (Error: " << GetLastError() << ")" << std::endl;
    }
    return handle;
#else
    void* handle = dlopen(filepath.c_str(), RTLD_LAZY);
    if (!handle) {
        std::cerr << "[SIMULATOR] dlopen failed for: " << filepath 
                  << " (Error: " << dlerror() << ")" << std::endl;
    }
    return handle;
#endif
}

void Simulator::unloadLibrary(void* handle) {
    if (!handle) return;
    
#ifdef _WIN32
    FreeLibrary((HMODULE)handle);
#else
    dlclose(handle);
#endif
}
