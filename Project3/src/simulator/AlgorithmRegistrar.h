#ifndef ALGORITHM_REGISTRAR_H
#define ALGORITHM_REGISTRAR_H

#include "../../common/TankAlgorithm.h"
#include "../../common/Player.h"
#include <vector>
#include <string>
#include <functional>
#include <memory>
#include <cassert>

/**
 * Dynamic algorithm registration system for Project 3
 * Implements the exact interface specified in the assignment
 * Named DynamicAlgorithmRegistrar to avoid conflicts with the built-in AlgorithmRegistrar
 */
class DynamicAlgorithmRegistrar {
private:
    class AlgorithmAndPlayerFactories {
        std::string so_name;
        TankAlgorithmFactory tankAlgorithmFactory;
        PlayerFactory playerFactory;
        
    public:
        AlgorithmAndPlayerFactories(const std::string& so_name) : so_name(so_name) {}
        
        void setTankAlgorithmFactory(TankAlgorithmFactory&& factory) {
            assert(tankAlgorithmFactory == nullptr);
            tankAlgorithmFactory = std::move(factory);
        }
        
        void setPlayerFactory(PlayerFactory&& factory) {
            assert(playerFactory == nullptr);
            playerFactory = std::move(factory);
        }
        
        const std::string& name() const { return so_name; }
        
        std::unique_ptr<Player> createPlayer(int player_index, size_t x, size_t y, size_t max_steps, size_t num_shells) const {
            return playerFactory(player_index, x, y, max_steps, num_shells);
        }
        
        std::unique_ptr<TankAlgorithm> createTankAlgorithm(int player_index, int tank_index) const {
            return tankAlgorithmFactory(player_index, tank_index);
        }
        
        bool hasPlayerFactory() const {
            return playerFactory != nullptr;
        }
        
        bool hasTankAlgorithmFactory() const {
            return tankAlgorithmFactory != nullptr;
        }
    };
    
    std::vector<AlgorithmAndPlayerFactories> algorithms;
    static DynamicAlgorithmRegistrar registrar;
    
public:
    static DynamicAlgorithmRegistrar& getAlgorithmRegistrar();
    
    void createAlgorithmFactoryEntry(const std::string& name) {
        algorithms.emplace_back(name);
    }
    
    void addPlayerFactoryToLastEntry(PlayerFactory&& factory) {
        algorithms.back().setPlayerFactory(std::move(factory));
    }
    
    void addTankAlgorithmFactoryToLastEntry(TankAlgorithmFactory&& factory) {
        algorithms.back().setTankAlgorithmFactory(std::move(factory));
    }
    
    struct BadRegistrationException {
        std::string name;
        bool hasName;
        bool hasPlayerFactory;
        bool hasTankAlgorithmFactory;
    };
    
    void validateLastRegistration() {
        const auto& last = algorithms.back();
        bool hasName = (last.name() != "");
        if(!hasName || !last.hasPlayerFactory() || !last.hasTankAlgorithmFactory()) {
            throw BadRegistrationException{
                .name = last.name(),
                .hasName = hasName,
                .hasPlayerFactory = last.hasPlayerFactory(),
                .hasTankAlgorithmFactory = last.hasTankAlgorithmFactory()
            };
        }
    }
    
    void removeLast() {
        algorithms.pop_back();
    }
    
    auto begin() const {
        return algorithms.begin();
    }
    
    auto end() const {
        return algorithms.end();
    }
    
    std::size_t count() const { 
        return algorithms.size(); 
    }
    
    void clear() { 
        algorithms.clear(); 
    }
    
    // Additional methods for compatibility with current system
    const AlgorithmAndPlayerFactories* findAlgorithm(const std::string& name) const {
        for (const auto& algo : algorithms) {
            if (algo.name() == name) {
                return &algo;
            }
        }
        return nullptr;
    }
};

#endif // ALGORITHM_REGISTRAR_H
