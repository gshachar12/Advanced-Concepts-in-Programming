#include "../common/PlayerRegistration.h"
#include <vector>
#include <functional>

// Global registry for player factories
static std::vector<PlayerFactory> g_player_factories;

PlayerRegistration::PlayerRegistration(PlayerFactory factory) {
    g_player_factories.push_back(factory);
}

// Accessor function for simulator
const std::vector<PlayerFactory>& getRegisteredPlayerFactories() {
    return g_player_factories;
}
