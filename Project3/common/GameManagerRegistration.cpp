#include "GameManagerRegistration.h"

GameManagerRegistration::GameManagerRegistration(std::function<std::unique_ptr<AbstractGameManager>(bool)> factory) {
    // Store factory for later use by simulator
    // This is a simplified implementation
}
