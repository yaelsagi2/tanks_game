#pragma once
#include <vector>
#include <memory>
#include <functional>
#include "../common/AbstractGameManager.h"
#include <iostream>
#include <string>
/**
 * @brief Singleton registrar for storing and accessing GameManager factories.
 * 
 * This class allows registration and retrieval of factories that create AbstractGameManager instances.
 */
class GameManagerRegistrar {
public:
    /**
     * @brief Constructs a GameManagerRegistrar.
     */
    GameManagerRegistrar() {}

    /**
     * @brief Returns the singleton instance of the registrar.
     * @return Reference to the singleton GameManagerRegistrar.
     */
    static GameManagerRegistrar& getGameManagerRegistrar();

    /**
     * @brief Adds a new GameManager factory to the registrar.
     * @param factory A factory function that creates a unique_ptr<AbstractGameManager>.
     */
    void addGameManagerFactory(std::function<std::unique_ptr<AbstractGameManager>(bool)>&& factory, const std::string& filename) {
        managers.emplace_back(factory);
        filenames.emplace_back(filename);
    }
        // Set the filename for the last registered factory (used by loader after dlopen)
    void setLastFilename(const std::string& filename) {
        if (!filenames.empty()) {
            std::string clean_name = filename;
            if (clean_name.size() > 3 && clean_name.substr(clean_name.size() - 3) == ".so") {
                clean_name = clean_name.substr(0, clean_name.size() - 3);
            }
            filenames.back() = clean_name;
        }
    }
    /**
     * @brief Returns an iterator to the beginning of the factories vector.
     */
    auto begin() const {
        return managers.begin();
    }

    /**
     * @brief Returns an iterator to the end of the factories vector.
     */
    auto end() const {
        return managers.end();
    }

    /**
     * @brief Gets the factory at the specified index.
     * @param index The index of the factory.
     * @return The factory function at the given index.
     */
    auto getAt(int index) const {
        return managers[index];
    }

    // Get the filename for a given index
    const std::string& getFilenameAt(int index) const {
        return filenames[index];
    }

    /**
     * @brief Returns the number of registered factories.
     * @return The number of factories.
     */
    std::size_t count() const { return managers.size(); }

    /**
     * @brief Clears all registered factories.
     */
    void clear() { managers.clear(); filenames.clear(); }

    /**
     * @brief The singleton instance of the registrar.
     */
    static GameManagerRegistrar registrar;

private:
    std::vector<std::function<std::unique_ptr<AbstractGameManager>(bool)>> managers; ///< Registered factories
    std::vector<std::string> filenames; ///< Filenames for each factory
};



