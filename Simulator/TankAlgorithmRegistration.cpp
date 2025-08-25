#include "../common/TankAlgorithmRegistration.h"
#include "AlgorithmRegistrar.h"


TankAlgorithmRegistration::TankAlgorithmRegistration(TankAlgorithmFactory factory) {
    // This constructor registers a new tank algorithm factory
    auto& registrar = AlgorithmRegistrar::getAlgorithmRegistrar();
    registrar.addTankAlgorithmFactoryToLastEntry(std::move(factory));
}
