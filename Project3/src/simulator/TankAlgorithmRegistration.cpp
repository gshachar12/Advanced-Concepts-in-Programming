#include "../simulator/AlgorithmRegistrar.h"

/**
 * Tank algorithm registration implementation for dynamic loading
 */
TankAlgorithmRegistration::TankAlgorithmRegistration(TankAlgorithmFactory factory) {
    auto& registrar = DynamicAlgorithmRegistrar::getAlgorithmRegistrar();
    registrar.addTankAlgorithmFactoryToLastEntry(std::move(factory));
}
