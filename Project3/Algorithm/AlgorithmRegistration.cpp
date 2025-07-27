#include "SimpleAlgorithm.h"
#include "BfsAlgorithm.h"
#include "SimplePlayer.h" 
#include "BfsPlayer.h"
#include <memory>

using namespace Algorithm_123456789_987654321;

// Entry points for the shared library
extern "C" {
    std::unique_ptr<TankAlgorithm> createTankAlgorithm() {
        return std::make_unique<SimpleAlgorithm>();
    }
    
    std::unique_ptr<Player> createPlayer() {
        return std::make_unique<SimplePlayer>();
    }
    
    // Alternative algorithms
    std::unique_ptr<TankAlgorithm> createBfsTankAlgorithm() {
        return std::make_unique<BfsAlgorithm>();
    }
    
    std::unique_ptr<Player> createBfsPlayer() {
        return std::make_unique<BfsPlayer>();
    }
}
