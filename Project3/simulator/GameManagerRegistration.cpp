#include "../common/GameManagerRegistration.h"
#include <vector>
#include <functional>

// Global registry for game manager factories
static std::vector<GameManagerFactory> g_gamemanager_factories;

GameManagerRegistration::GameManagerRegistration(GameManagerFactory factory) {
    g_gamemanager_factories.push_back(factory);
}

// Accessor function for simulator
const std::vector<GameManagerFactory>& getRegisteredGameManagerFactories() {
    return g_gamemanager_factories;
}
