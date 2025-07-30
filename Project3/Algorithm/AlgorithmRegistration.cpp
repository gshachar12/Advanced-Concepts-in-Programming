#include "SimpleAlgorithm.h"
#include "../common/TankAlgorithmRegistration.h"
#include <memory>

using namespace Algorithm_123456789_987654321;

// Registration using assignment-required macros
REGISTER_TANK_ALGORITHM(SimpleAlgorithm)

// Entry points for the shared library
extern "C" {
    std::unique_ptr<TankAlgorithm> createTankAlgorithm(int player_index, int tank_index) {
        return std::make_unique<SimpleAlgorithm>(player_index, tank_index);
    }
}
