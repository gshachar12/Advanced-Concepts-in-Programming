#ifndef TANK_ALGORITHM_REGISTRATION_H
#define TANK_ALGORITHM_REGISTRATION_H

#include "TankAlgorithm.h"
#include <functional>
#include <memory>

/**
 * Tank algorithm registration structure for dynamic plugin loading
 */
struct TankAlgorithmRegistration {
    TankAlgorithmRegistration(TankAlgorithmFactory factory);
};

/**
 * Registration macro for TankAlgorithm classes
 * Usage: REGISTER_TANK_ALGORITHM(MyTankAlgorithmClass);
 */
#define REGISTER_TANK_ALGORITHM(class_name) \
    TankAlgorithmRegistration register_me_##class_name \
    ( [](int player_index, int tank_index) { \
        return std::make_unique<class_name>(player_index, tank_index); \
    } );

#endif // TANK_ALGORITHM_REGISTRATION_H
