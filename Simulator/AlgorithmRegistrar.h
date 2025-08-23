#pragma once 
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <cassert>
#include "../common/TankAlgorithm.h"
#include "../common/Player.h"

/**
 * @class AlgorithmRegistrar
 * @brief Manages registration and creation of algorithm and player factories for loaded .so files.
 */
class AlgorithmRegistrar {
    /**
     * @class AlgorithmAndPlayerFactories
     * @brief Holds factories and metadata for a single algorithm/player implementation.
     */
    class AlgorithmAndPlayerFactories {
        // This class holds the name of the shared object and the factories for creating TankAlgorithm and Player instances.
    private:
        std::string _name; ///< Name of the shared object file
        TankAlgorithmFactory tankAlgorithmFactory; ///< Factory for creating TankAlgorithm instances
        PlayerFactory playerFactory; ///< Factory for creating Player instances
    public:
        /**
         * @brief Constructor with shared object name.
         */
        explicit AlgorithmAndPlayerFactories()
            : tankAlgorithmFactory(nullptr), playerFactory(nullptr) {
        }

        /**
         * @brief Set the TankAlgorithm factory.
         * @param factory Rvalue reference to TankAlgorithmFactory.
         */
        void setTankAlgorithmFactory(const TankAlgorithmFactory& factory) {
            tankAlgorithmFactory = factory;
        }

        auto getTankAlgorithmFactory() {
            return tankAlgorithmFactory;
        }

        /**
         * @brief Set the Player factory.
         * @param factory Rvalue reference to PlayerFactory.
         */
        void setPlayerFactory(const PlayerFactory &factory) {
            playerFactory = factory;
        }

        auto getPlayerFactory() {
            return playerFactory;
        }

        void setPlayerName(const std::string& name) {
            _name = name.ends_with(".so") ? name.substr(0, name.size() - 3) : name;
        }

        /**
         * @brief Get the name of the shared object.
         * @return Reference to the name string.
         */
        const std::string& getPlayerName() const { return _name; }

        /**
         * @brief Create a Player instance using the factory.
         * @param player_index Index of the player.
         * @param x X position.
         * @param y Y position.
         * @param max_steps Maximum steps allowed.
         * @param num_shells Number of shells.
         * @return Unique pointer to Player.
         */
        std::unique_ptr<Player> createPlayer(int player_index, size_t x, size_t y, size_t max_steps, size_t num_shells) const {
            return playerFactory(player_index, x, y, max_steps, num_shells);
        }

        /**
         * @brief Create a TankAlgorithm instance using the factory.
         * @param player_index Index of the player.
         * @param tank_index Index of the tank.
         * @return Unique pointer to TankAlgorithm.
         */
        std::unique_ptr<TankAlgorithm> createTankAlgorithm(int player_index, int tank_index) const {
            return tankAlgorithmFactory(player_index, tank_index);
        }

        /**
         * @brief Check if PlayerFactory is set.
         * @return True if PlayerFactory is set, false otherwise.
         */
        bool hasPlayerFactory() const {
            return playerFactory != nullptr;
        }

        /**
         * @brief Check if TankAlgorithmFactory is set.
         * @return True if TankAlgorithmFactory is set, false otherwise.
         */
        bool hasTankAlgorithmFactory() const {
            return tankAlgorithmFactory != nullptr;
        }
    };

    std::vector<AlgorithmAndPlayerFactories> algorithms; ///< List of registered algorithms
    static AlgorithmRegistrar registrar; ///< Singleton instance

public:

    /**
    * @brief Get a reference to AlgorithmAndPlayerFactories at a given index.
    * @param index Position in the algorithms list.
    * @return Const reference to the AlgorithmAndPlayerFactories.
    * @throws std::out_of_range if index is invalid.
    */
    auto& getAt(std::size_t index) const {
        if (index + 1 > algorithms.size()) {
            throw std::out_of_range("AlgorithmRegistrar::getAt - index out of range");
        }
        return algorithms[index];
    }


    /**
     * @brief Get the singleton AlgorithmRegistrar instance.
     * @return Reference to the AlgorithmRegistrar class.
     */
    static AlgorithmRegistrar& getAlgorithmRegistrar();

    /**
     * @brief Create a new algorithm factory entry.
     * @param name Name of the shared object.
     */
    void createAlgorithmFactoryEntry(const std::string& name) {
        algorithms.emplace_back();
        algorithms.back().setPlayerName(name);
    }

    /**
     * @brief Add a PlayerFactory to the last entry.
     * @param factory Rvalue reference to PlayerFactory.
     */
    void addPlayerFactoryToLastEntry(PlayerFactory factory) {
        if (algorithms.empty()) {
            algorithms.emplace_back();
        }
        algorithms.back().setPlayerFactory(factory);
    }

    /**
     * @brief Add a TankAlgorithmFactory to the last entry.
     * @param factory Rvalue reference to TankAlgorithmFactory.
     */
    void addTankAlgorithmFactoryToLastEntry(TankAlgorithmFactory factory) {
        if (algorithms.empty()) {
            algorithms.emplace_back();
        }
        algorithms.back().setTankAlgorithmFactory(factory);
    }

    /**
     * @struct BadRegistrationException
     * @brief Exception thrown for invalid registration.
     */
    struct BadRegistrationException {
        std::string name; ///< Name of the shared object
        bool hasName; ///< True if name is set
        bool hasPlayerFactory; ///< True if PlayerFactory is set
        bool hasTankAlgorithmFactory; ///< True if TankAlgorithmFactory is set
    };

    /**
     * @brief Validate the last registration entry.
     * @throws BadRegistrationException if registration is incomplete.
     */
    void validateLastRegistration() {
        // Ensure the last entry has a name, PlayerFactory, and TankAlgorithmFactory set.
        const auto& last = algorithms.back();
        bool hasName = (last.getPlayerName() != "");
        if(!hasName || !last.hasPlayerFactory() || !last.hasTankAlgorithmFactory() ) {
            throw BadRegistrationException{
                .name = last.getPlayerName(),
                .hasName = hasName,
                .hasPlayerFactory = last.hasPlayerFactory(),
                .hasTankAlgorithmFactory = last.hasTankAlgorithmFactory()
            };
        }
    }

    /**
     * @brief Remove the last registration entry.
     */
    void removeLast() {
        if (!algorithms.empty()) {
            algorithms.pop_back();
        }
    }

    /**
     * @brief Get iterator to beginning of algorithms list.
     * @return Iterator to beginning.
     */
    auto begin() const {
        return algorithms.begin();
    }

    /**
     * @brief Get iterator to end of algorithms list.
     * @return Iterator to end.
     */
    auto end() const {
        return algorithms.end();
    }

    /**
     * @brief Get the number of registered algorithms.
     * @return Number of algorithms.
     */
    std::size_t count() const { return algorithms.size(); }

    /**
     * @brief Clear all registrations.
     */
    void clear() { algorithms.clear(); }
};