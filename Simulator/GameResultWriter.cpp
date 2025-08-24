
#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include <chrono>
#include <sstream>
#include <cmath>
#include <iomanip>
#include <fstream>
#include "../UserCommon/TimeStamp.h"
#include "../common/GameResult.h"
#include "GameResultWriter.h"
#include <string>
#include <cstddef>
#include <cassert>


using namespace UserCommon_206480972_206899163;

std::string GameResultWriter::getGameManagersList(const std::vector<std::string>& v) {
    // This function generates a comma-separated list of game manager names
    std::string gm_list;
    for (std::size_t i = 0; i < v.size(); ++i) {
        gm_list.append(v[i]);
        if (i < v.size() - 1) {
            gm_list.append(", ");
        }
    }
    return gm_list;
}


std::string GameResultWriter::getMessageResult(const GameResultEx* game_result) {
    // This function writes message result for printing to file
    const int player_id1 = 1;
    const int player_id2 = 2;
    const int extra_steps = 40;
    std::string message;
    if (game_result->winner == 0) {
        if (game_result->reason == GameResult::Reason::MAX_STEPS) {
            message = "Tie, reached max steps=" + std::to_string(game_result->rounds) + ", player " + std::to_string(player_id1) + " has " + std::to_string(game_result->remaining_tanks[0]) +
                " tanks, player " + std::to_string(player_id2) + " has "
                + std::to_string(game_result->remaining_tanks[1]) + " tanks";
        } else if (game_result->reason == GameResult::Reason::ZERO_SHELLS) {
            message = "Tie, both players have zero shells for " + std::to_string(extra_steps) + " steps";
        } else if (game_result->reason == GameResult::Reason::ALL_TANKS_DEAD) {
            message = "Tie, both players have zero tanks";
        }
    } else {
        if (game_result->winner == 1) {
            message = "Player " + std::to_string(player_id1) + " won with " + std::to_string(game_result->remaining_tanks[0]) + " tanks still alive";
        }
        else{
            message = "Player " + std::to_string(player_id2) + " won with " + std::to_string(game_result->remaining_tanks[1]) + " tanks still alive";
        }
    }

    return message;
}

// This function writes the comparative results to a file.
void GameResultWriter::writeComparativeResults(const std::string& game_map_path, const std::string& algo1_so, const std::string& algo2_so, const std::vector<std::pair<std::string, GameResultEx>>& results, int, int)
{
    const std::string filename = "comparative_results_" + getTimestamp() + ".txt";
    std::ofstream file(filename);
    std::ostream* out = file.is_open() ? static_cast<std::ostream*>(&file) : static_cast<std::ostream*>(&std::cout);
    if (!file.is_open()) { std::cerr << "Warning: could not open '" << filename << "' for writing. Falling back to stdout.\n";}
    (*out) << "game_map="   << game_map_path << '\n'; // Line 1
    std::string algo1_final = removeSoSuffix(algo1_so);
    std::string algo2_final = removeSoSuffix(algo2_so);
    (*out) << "algorithm1=" << algo1_final      << '\n'; // Line 2
    (*out) << "algorithm2=" << algo2_final      << '\n'; // Line 3
    (*out) << '\n'; // Line 4
    if (results.empty()) { out->flush(); return; }
    std::unordered_map<OutcomeKey, std::size_t, OutcomeKeyHash> key_to_index;      // Bucket by identical final outcome
    std::vector<Group> groups;
    groups.reserve(results.size());
    for (const auto& [name, gr] : results) {
        OutcomeKey key{gr.winner, gr.reason, gr.rounds, gr.game_map_grid};
        auto it = key_to_index.find(key);
        if (it == key_to_index.end()) {
            std::size_t idx = groups.size();
            key_to_index.emplace(std::move(key), idx);
            groups.push_back(Group{std::vector<std::string>{name}, &gr});
        } else { groups[it->second].names.push_back(name);}
    }
    std::vector<std::size_t> order(groups.size());     // Sort groups by size (desc), stable on ties
    std::iota(order.begin(), order.end(), 0);
    std::stable_sort(order.begin(), order.end(),[&](std::size_t a, std::size_t b) { return groups[a].names.size() > groups[b].names.size();});
    bool printed_state_for_first_group = false;
    for (std::size_t pos = 0; pos < order.size(); ++pos) {
        const Group& g = groups[order[pos]];
        if (pos > 0) (*out) << '\n';     // separate groups
        (*out) << getGameManagersList(g.names) << '\n'; // Line 5: game managers names
        (*out) << getMessageResult(g.sample) << '\n';  // Line 6: result as string
        (*out) << g.sample->rounds << '\n'; // Line 7: rounds
        if (!printed_state_for_first_group) { // Line 8+: only for the first (largest) group
            if (g.sample->game_map_grid != "") {
                (*out) << g.sample->game_map_grid << '\n';
            } else { (*out) << '\n';}
            //printed_state_for_first_group = true;
        }
    }
    out->flush();
}

/*
expected ouput structure :

game_maps_folder=Maps
game_manager=GameManager_206038929_314620071

Algorithm_322213836_212054837 61
Algorithm_209399021_208239152 59
Algorithm_206038929_314620071 35
Algorithm_207174533_321321945 13

*/
void GameResultWriter::writeCompetitiveResults( const std::string& game_maps_folder,
    const std::string& game_manager, const std::vector<std::pair<std::string, int>>& winners) {
    // This function writes the competitive results to a file.
    // Sort by wins (desc), then by name (asc) for determinism
    std::vector<std::pair<std::string, int>> sorted = winners;
    std::stable_sort(sorted.begin(), sorted.end(),
        [](const auto& a, const auto& b) {
            if (a.second != b.second) return a.second > b.second;
            return a.first < b.first;
        });

    // Build output filename based on game_manager
    const std::string filename = "competition_" + getTimestamp() + ".txt";

    std::ofstream file(filename);
    std::ostream* out = nullptr;

    if (file.is_open()) {
        out = &file;
    } else {
        std::cerr << "Warning: couldn't open '" << filename << "' for writing. Falling back to stdout.\n";
        out = &std::cout;
    }
    // Write header
    (*out) << "game_maps_folder=" << game_maps_folder << '\n';
    (*out) << "game_manager="     << game_manager     << '\n';
    (*out) << '\n';
    // Write sorted winners
    for (const auto& [name, wins] : sorted) {
        (*out) << name << ' ' << wins << '\n';
    }
    // Ensure flush if writing to file
    out->flush();
}

// Helper function to remove .so suffix from a string if present
std::string GameResultWriter::removeSoSuffix(const std::string& s) {
    if (s.size() >= 3 && s.substr(s.size() - 3) == ".so") {
        return s.substr(0, s.size() - 3);
    }
    return s;
}