#include "AlgorithmRegistrar.h"

// Static member definition
DynamicAlgorithmRegistrar DynamicAlgorithmRegistrar::registrar;

DynamicAlgorithmRegistrar& DynamicAlgorithmRegistrar::getAlgorithmRegistrar() {
    return registrar;
}
