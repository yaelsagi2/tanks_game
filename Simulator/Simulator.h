#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <thread>
#include <map>
#include <mutex>

#include "../common/SatelliteView.h"
#include "ArgsParser.h"
#include "../common/GameResult.h"
#include "../common/AbstractGameManager.h"

/**
 * @brief Simulator class for running tank game tournaments and matches.
 * 
 * This class manages loading algorithms, running games in different modes,
 * and handling multi-threaded execution of games.
 */
class Simulator {
public:
    /**
     * @brief Constructs a Simulator.
     */
    Simulator();

    /**
     * @brief Runs the simulator (main entry point).
     */
    void run();

    /**
     * @brief Loads all required shared object files (algorithms, managers).
     */
    void loadSharedObjects();

    /**
     * @brief Unloads all loaded shared object files.
     */
    void unloadSharedObjects();

    /**
     * @brief Runs the simulator in comparative mode (pairwise algorithm matches).
     * @param args Parsed command-line arguments.
     */
    void runComparativeMode(const ParsedArgs& args);

    /**
     * @brief Runs the simulator in competition mode (tournament).
     * @param args Parsed command-line arguments.
     */
    void runCompetitionMode(const ParsedArgs& args);

    /**
     * @brief Gets all map file names from a folder.
     * @param folder_path Path to the folder containing maps.
     * @return Vector of map file names.
     */
    std::vector<std::string> getAllMapNames(const std::string& folder_path);

private:
    /**
     * @brief Computes a positive modulus.
     * @param x The value.
     * @param m The modulus.
     * @return The positive modulus result.
     */
    static int modp(int x, int m) { return (x % m + m) % m; }

    /**
     * @brief Sorts and converts a map of winners to a vector of pairs.
     * @param winners_map Map of algorithm names to scores.
     * @return Sorted vector of (algorithm name, score) pairs.
     */
    std::vector<std::pair<std::string, int>> sortAndConvertMap(const std::map<std::string, int>& winners_map);

    /**
     * @brief Generates all pairs of algorithm indices for a given number of algorithms and maps.
     * @param number_of_algorithms Number of algorithms.
     * @param number_of_maps Number of maps.
     * @return Vector of (algorithm index, map index) pairs.
     */
    static std::vector<std::pair<int, int>> pairsForMap(int number_of_algorithms, int number_of_maps);

    std::size_t countFilesWithPrefixAndExtension(const std::string& dir, const std::string& prefix = "", const std::string& ext = "");

    std::vector<AbstractGameManager*> game_managers; ///< List of game manager instances
    std::mutex gm_access_mutex; ///< Mutex for synchronizing access to game_managers
    bool exit_threads = false; ///< Flag to signal threads to exit
    std::vector<std::thread> threads; ///< Worker threads
};


