#ifndef GAME_MANAGER_REGISTRATION_H
#define GAME_MANAGER_REGISTRATION_H

#include "AbstractGameManager.h"
#include <functional>
#include <memory>

/**
 * Game manager registration structure for dynamic plugin loading
 */
struct GameManagerRegistration {
    GameManagerRegistration(std::function<std::unique_ptr<AbstractGameManager>(bool)> factory);
};

/**
 * Registration macro for GameManager classes
 * Usage: REGISTER_GAME_MANAGER(MyGameManagerClass);
 */
#define REGISTER_GAME_MANAGER(class_name) \
    GameManagerRegistration register_me_##class_name \
    ( [](bool verbose) { return std::make_unique<class_name>(verbose); } );

#endif // GAME_MANAGER_REGISTRATION_H
