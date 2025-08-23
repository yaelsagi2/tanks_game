#include "GameManagerRegistrar.h"
// This file contains the implementation of the GameManagerRegistrar class, which manages game manager factories.

GameManagerRegistrar GameManagerRegistrar::registrar; // Singleton instance of GameManagerRegistrar

GameManagerRegistrar& GameManagerRegistrar::getGameManagerRegistrar() {
    // Return the singleton instance of GameManagerRegistrar
    return registrar;
}
