#include "../common/TankAlgorithmRegistration.h"
#include <vector>
#include <functional>

// Global registry for tank algorithm factories
static std::vector<TankAlgorithmFactory> g_algorithm_factories;

TankAlgorithmRegistration::TankAlgorithmRegistration(TankAlgorithmFactory factory) {
    g_algorithm_factories.push_back(factory);
}

// Accessor function for simulator
const std::vector<TankAlgorithmFactory>& getRegisteredAlgorithmFactories() {
    return g_algorithm_factories;
}
