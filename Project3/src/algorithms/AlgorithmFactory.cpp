#include "AlgorithmFactory.h"
#include <iostream>
#include <random>

// Static member initialization
std::vector<AlgorithmRegistrar::AlgorithmInfo> AlgorithmRegistrar::registered_algorithms_;

// Tank Algorithm Factory Implementations
std::unique_ptr<TankAlgorithm> AlgorithmFactory::createSimpleAlgorithm(int player_index, int tank_index) {
    int seed = player_index * 100 + tank_index + static_cast<int>(std::time(nullptr));
    return std::make_unique<SimpleAlgorithm>(seed);
}

std::unique_ptr<TankAlgorithm> AlgorithmFactory::createBfsAlgorithm(int player_index, int tank_index) {
    return std::make_unique<BfsAlgorithm>();
}

std::unique_ptr<TankAlgorithm> AlgorithmFactory::createRandomAlgorithm(int player_index, int tank_index) {
    // Create a simple algorithm with different random seed for variety
    int seed = player_index * 1000 + tank_index * 17 + static_cast<int>(std::time(nullptr));
    return std::make_unique<SimpleAlgorithm>(seed);
}

// Player Factory Implementations
std::unique_ptr<Player> AlgorithmFactory::createSimplePlayer(int player_index, size_t x, size_t y, size_t max_steps, size_t num_shells) {
    return std::make_unique<MyPlayer>("SimplePlayer_" + std::to_string(player_index));
}

std::unique_ptr<Player> AlgorithmFactory::createBfsPlayer(int player_index, size_t x, size_t y, size_t max_steps, size_t num_shells) {
    return std::make_unique<MyPlayer>("BfsPlayer_" + std::to_string(player_index));
}

// Function Factory Getters
TankAlgorithmFactory AlgorithmFactory::getSimpleAlgorithmFactory() {
    return [](int player_index, int tank_index) {
        return createSimpleAlgorithm(player_index, tank_index);
    };
}

TankAlgorithmFactory AlgorithmFactory::getBfsAlgorithmFactory() {
    return [](int player_index, int tank_index) {
        return createBfsAlgorithm(player_index, tank_index);
    };
}

TankAlgorithmFactory AlgorithmFactory::getRandomAlgorithmFactory() {
    return [](int player_index, int tank_index) {
        return createRandomAlgorithm(player_index, tank_index);
    };
}

PlayerFactory AlgorithmFactory::getSimplePlayerFactory() {
    return [](int player_index, size_t x, size_t y, size_t max_steps, size_t num_shells) {
        return createSimplePlayer(player_index, x, y, max_steps, num_shells);
    };
}

PlayerFactory AlgorithmFactory::getBfsPlayerFactory() {
    return [](int player_index, size_t x, size_t y, size_t max_steps, size_t num_shells) {
        return createBfsPlayer(player_index, x, y, max_steps, num_shells);
    };
}

// Algorithm Registration System Implementation
void AlgorithmRegistrar::registerAlgorithm(const std::string& name, 
                                         const std::string& description,
                                         TankAlgorithmFactory tank_factory,
                                         PlayerFactory player_factory) {
    registered_algorithms_.push_back({name, description, tank_factory, player_factory});
}

const std::vector<AlgorithmRegistrar::AlgorithmInfo>& AlgorithmRegistrar::getRegisteredAlgorithms() {
    return registered_algorithms_;
}

AlgorithmRegistrar::AlgorithmInfo* AlgorithmRegistrar::findAlgorithm(const std::string& name) {
    for (auto& info : registered_algorithms_) {
        if (info.name == name) {
            return &info;
        }
    }
    return nullptr;
}

void AlgorithmRegistrar::registerDefaultAlgorithms() {
    std::cout << "[INFO] Registering default algorithms...\n";
    
    registerAlgorithm(
        "Simple",
        "Basic algorithm with random movement and occasional shooting",
        AlgorithmFactory::getSimpleAlgorithmFactory(),
        AlgorithmFactory::getSimplePlayerFactory()
    );
    
    registerAlgorithm(
        "BFS",
        "Breadth-First Search pathfinding algorithm with strategic movement",
        AlgorithmFactory::getBfsAlgorithmFactory(),
        AlgorithmFactory::getBfsPlayerFactory()
    );
    
    registerAlgorithm(
        "Random",
        "Random variant of simple algorithm with different behavior patterns",
        AlgorithmFactory::getRandomAlgorithmFactory(),
        AlgorithmFactory::getSimplePlayerFactory()
    );
    
    std::cout << "[SUCCESS] Registered " << registered_algorithms_.size() << " algorithms\n";
    
    // Display registered algorithms
    std::cout << "\nAvailable Algorithms:\n";
    for (const auto& info : registered_algorithms_) {
        std::cout << "  * " << info.name << ": " << info.description << "\n";
    }
    std::cout << "\n";
}
