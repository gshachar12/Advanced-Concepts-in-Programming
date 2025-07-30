#include "../simulator/AlgorithmRegistrar.h"

/**
 * Player registration implementation for dynamic loading
 */
PlayerRegistration::PlayerRegistration(PlayerFactory factory) {
    auto& registrar = DynamicAlgorithmRegistrar::getAlgorithmRegistrar();
    registrar.addPlayerFactoryToLastEntry(std::move(factory));
}
