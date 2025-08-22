#ifndef PLAYER_REGISTRATION_H
#define PLAYER_REGISTRATION_H

#include "Player.h"
#include <functional>
#include <memory>

/**
 * Player registration structure for dynamic plugin loading
 */
struct PlayerRegistration {
    using PlayerCreator = std::function<std::unique_ptr<Player>(int, size_t, size_t, size_t, size_t)>;
    PlayerRegistration(PlayerCreator creator);
};

/**
 * Registration macro for Player classes
 * Usage: REGISTER_PLAYER(MyPlayerClass);
 */
#define REGISTER_PLAYER(class_name) \
    PlayerRegistration register_me_##class_name \
    ( [] (int player_index, size_t x, size_t y, size_t max_steps, size_t num_shells) { \
        return std::make_unique<class_name>(player_index, x, y, max_steps, num_shells); \
    } );

#endif // PLAYER_REGISTRATION_H
