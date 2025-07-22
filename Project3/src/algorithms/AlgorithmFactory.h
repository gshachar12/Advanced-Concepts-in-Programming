#ifndef ALGORITHM_FACTORY_H
#define ALGORITHM_FACTORY_H

#include "../../common/TankAlgorithm.h"
#include "../../common/Player.h"
#include "SimpleAlgorithm.h"
#include "BfsAlgorithm.h"
#include "MyPlayer.h"
#include <memory>
#include <functional>
#include <vector>
#include <ctime>

/**
 * Factory functions for creating different algorithm types
 */
class AlgorithmFactory {
public:
    /**
     * Tank Algorithm Factories
     */
    static std::unique_ptr<TankAlgorithm> createSimpleAlgorithm(int player_index, int tank_index);
    static std::unique_ptr<TankAlgorithm> createBfsAlgorithm(int player_index, int tank_index);
    static std::unique_ptr<TankAlgorithm> createRandomAlgorithm(int player_index, int tank_index);
    
    /**
     * Player Factories
     */
    static std::unique_ptr<Player> createSimplePlayer(int player_index, size_t x, size_t y, size_t max_steps, size_t num_shells);
    static std::unique_ptr<Player> createBfsPlayer(int player_index, size_t x, size_t y, size_t max_steps, size_t num_shells);
    
    /**
     * Function factory getters for GameManager integration
     */
    static TankAlgorithmFactory getSimpleAlgorithmFactory();
    static TankAlgorithmFactory getBfsAlgorithmFactory();
    static TankAlgorithmFactory getRandomAlgorithmFactory();
    
    static PlayerFactory getSimplePlayerFactory();
    static PlayerFactory getBfsPlayerFactory();
};

/**
 * Algorithm Registration System
 */
class AlgorithmRegistrar {
public:
    struct AlgorithmInfo {
        std::string name;
        std::string description;
        TankAlgorithmFactory factory;
        PlayerFactory player_factory;
    };
    
private:
    static std::vector<AlgorithmInfo> registered_algorithms_;
    
public:
    static void registerAlgorithm(const std::string& name, 
                                const std::string& description,
                                TankAlgorithmFactory tank_factory,
                                PlayerFactory player_factory);
    
    static const std::vector<AlgorithmInfo>& getRegisteredAlgorithms();
    static AlgorithmInfo* findAlgorithm(const std::string& name);
    
    static void registerDefaultAlgorithms();
};

#endif // ALGORITHM_FACTORY_H
