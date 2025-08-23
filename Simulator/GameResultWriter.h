#pragma once

#include <functional>
#include <string>
#include <vector>

#include "../common/SatelliteView.h"
#include "../common/GameResult.h"


/**
 * @brief Key for grouping GameResult outcomes by winner, reason, rounds, and state.
 */
struct OutcomeKey {
    int winner;                    ///< The winner of the game (0 for tie)
    GameResult::Reason reason;     ///< The reason the game ended
    size_t rounds;                 ///< Number of rounds played
    std::string state;             ///< Board state as a string

    bool operator==(const OutcomeKey& o) const noexcept {
        return winner == o.winner &&
               reason == o.reason &&
               rounds == o.rounds &&
               state == o.state;
    }
};

struct OutcomeKeyHash {
    static inline void hash_combine(std::size_t& seed, std::size_t v) {
        // 64-bit version of boost::hash_combine
        seed ^= v + 0x9e3779b97f4a7c15ULL + (seed << 6) + (seed >> 2);
    }
    std::size_t operator()(const OutcomeKey& k) const noexcept {
        std::size_t seed = 0;
        hash_combine(seed, std::hash<int>{}(k.winner));
        hash_combine(seed, std::hash<int>{}(static_cast<int>(k.reason)));
        hash_combine(seed, std::hash<std::size_t>{}(k.rounds));
        hash_combine(seed, std::hash<std::string>{}(k.state));
        return seed;
    }
};

/**
 * @brief Represents a group of GameResults with the same outcome.
 */
struct Group {
    std::vector<std::string> names;      ///< Names of GameResult objects in this bucket
    const GameResult* sample = nullptr;  ///< Representative for rounds/state printing
};

/**
 * @brief Utility class for writing and formatting game results.
 */
class GameResultWriter {
public:
    /**
     * @brief Writes comparative results for two algorithms on a specific map.
     * @param game_map_path Path to the game map.
     * @param algo1_so Shared object filename for algorithm 1.
     * @param algo2_so Shared object filename for algorithm 2.
     * @param results Vector of (name, GameResult) pairs.
     */
    static void writeComparativeResults(
        const std::string& game_map_path,
        const std::string& algo1_so,
        const std::string& algo2_so,
        const std::vector<std::pair<std::string, GameResult>>& results,
        int map_width,
        int map_height
    );

    /**
     * @brief Writes competitive results for a set of games.
     * @param game_maps_folder Path to the folder containing game maps.
     * @param game_manager Name of the game manager used.
     * @param winners Vector of (algorithm name, score) pairs.
     */
    static void writeCompetitiveResults(
        const std::string& game_maps_folder,
        const std::string& game_manager,
        const std::vector<std::pair<std::string, int>>& winners
    );

private:
    /**
     * @brief Returns a formatted message for a game result.
     */
    static std::string getMessageResult(const GameResult& game_result);

    /**
     * @brief Returns a formatted list of game managers.
     */
    static std::string getGameManagersList(const std::vector<std::string>& v);

    /**
     * @brief Converts a GameResult::Reason to a string.
     */
    static std::string reasonToString(GameResult::Reason r);

    /**
     * @brief Returns a formatted result message for a GameResult.
     */
    static std::string resultMessage(const GameResult& gr);

    /**
     * @brief Joins lines into a single string with newlines.
     */
    static std::string joinLines(const std::vector<std::string>& lines);

    /**
     * @brief Extracts just the filename from a path.
     */
    static std::string justFilename(const std::string& path);

    static std::string satelliteViewToString(const SatelliteView* view, std::size_t width, std::size_t height);

};

