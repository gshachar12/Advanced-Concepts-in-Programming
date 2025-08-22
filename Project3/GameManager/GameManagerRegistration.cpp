#include "MyGameManager_Fixed.h"
#include "../common/AbstractGameManager.h"
#include "../common/GameManagerRegistration.h"
#include <memory>

using namespace GameManager_123456789_987654321;

// Registration using assignment-required macro
REGISTER_GAME_MANAGER(MyGameManager)

// Entry point for the shared library
extern "C" {
    std::unique_ptr<AbstractGameManager> createGameManager(bool verbose) {
        return std::make_unique<MyGameManager>(verbose);
    }
}
