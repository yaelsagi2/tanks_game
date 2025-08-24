#include "GameManagerRegistrar.h"
#include "../common/GameManagerRegistration.h"

/**
 * @brief Registers a GameManager factory with the global registrar.
 * 
 * This constructor is called when a GameManagerRegistration object is created,
 * adding the provided factory to the GameManagerRegistrar singleton.
 * 
 * @param factory A factory function that creates a unique_ptr<AbstractGameManager>.
 */
GameManagerRegistration::GameManagerRegistration(std::function<std::unique_ptr<AbstractGameManager>(bool)> factory) {
    auto& registrar = GameManagerRegistrar::getGameManagerRegistrar();
    registrar.addGameManagerFactory(std::move(factory), "");
}
