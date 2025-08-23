#include "AlgorithmRegistrar.h"

#include <stdexcept>
// This file contains the implementation of the AlgorithmRegistrar class, which manages
 
AlgorithmRegistrar AlgorithmRegistrar::registrar; // Singleton instance of AlgorithmRegistrar

AlgorithmRegistrar& AlgorithmRegistrar::getAlgorithmRegistrar() {
    // Return the singleton instance of AlgorithmRegistrar
    return registrar;
}




