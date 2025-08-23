#include "Simulator.h"
#include "MapParser.h"
#include "ArgsParser.h"
#include "Loader.h"
#include "GameManagerRegistrar.h"
#include "AlgorithmRegistrar.h"
//#include "../UserCommon/GameBoardSatelliteView.h"
//#include "GameManager.h"

#include <thread>
#include <mutex>
#include <map>
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <vector>
#include <string>
#include <set>
#include <iostream>
#include "ConcurrentQueue.h"
#include "Task.h"
#include "MapData.h"
#include "GameResultWriter.h"
#include "MapDataSatelliteView.h"
#include "../UserCommon/TimeStamp.h"
#include <cmath>

using namespace std;

int main(int argc, char* argv[]) {
    try {
	    Simulator simulator;
        ParsedArgs args = parseArgs(argc, argv);
        Loader::LoadSharedObjects(args);
        if (args.mode == ParsedArgs::Mode::Comparative) {
            simulator.runComparativeMode(args);
        } else if (args.mode == ParsedArgs::Mode::Competition) {
            simulator.runCompetitionMode(args);
        }
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

Simulator::Simulator() {

}

void Simulator::runComparativeMode(const ParsedArgs& args) {
    // Same players/algorithms & same map for all managers
    auto& play_and_algorithm_registrar = AlgorithmRegistrar::getAlgorithmRegistrar();
    auto& game_managers_registrar     = GameManagerRegistrar::getGameManagerRegistrar();

    std::vector<std::string> errors;
    auto map_data = readMapFile(args.game_map, errors); // tasks will read again by filename

    // Choose the two algorithms (mirrors your original code)
    const int player1_index = 1;
    const int player2_index = 2;
    auto algorithm1 = play_and_algorithm_registrar.getAlgorithmRegistrar().getAt(0);
    auto algorithm2 = play_and_algorithm_registrar.getAlgorithmRegistrar().getAt(1);

    // Shared collection for results
    std::vector<std::pair<std::string, GameResult>> results;
    std::mutex results_mtx;

    // Queue of tasks
    ConcurrentQueue<Task> queue;

    // Enqueue: one task per GameManager factory
    for (auto& gm_factory : game_managers_registrar) {
        Task t{
            args.game_map,
            player1_index,
            player2_index,
            algorithm1.getPlayerFactory(),
            algorithm1.getPlayerName(),
            algorithm2.getPlayerFactory(),
            algorithm2.getPlayerName(),
            algorithm1.getTankAlgorithmFactory(),
            algorithm2.getTankAlgorithmFactory(),
            gm_factory,
            args.verbose,
            nullptr // we are collecting results, not writing files
        };

        // Collect results thread-safely
        t.on_complete = [&](const Task* task, GameResult&& r) {
            std::lock_guard<std::mutex> lk(results_mtx);
            results.emplace_back(task->game_manager, std::move(r));
        };

        queue.push(std::move(t));
    }

    // No more tasks will be added
    queue.close();

    // Worker pool
    const unsigned num_threads = args.num_threads;

    std::vector<std::thread> workers;
    workers.reserve(num_threads);
    for (unsigned i = 0; i < num_threads; ++i) {
        workers.emplace_back([&queue] {
            Task task;
            while (queue.pop(task)) {
                task.run();
            }
        });
    }
    for (auto& th : workers) th.join();

    GameResultWriter::writeComparativeResults(
        args.game_map,
        args.algorithm1,
        args.algorithm2,
        results,
        map_data.rows,
        map_data.cols
    );
}

std::vector<std::pair<std::string, int>>
Simulator::sortAndConvertMap(const std::map<std::string, int>& winners_map) {
    std::vector<std::pair<std::string, int>> v;
    v.reserve(winners_map.size());
    for (const auto& kv : winners_map) v.emplace_back(kv.first, kv.second);

    std::sort(v.begin(), v.end(),
              [](const auto& a, const auto& b) {
                  if (a.second != b.second) return a.second > b.second; // value desc
                  return a.first < b.first; // tie-break by key asc (optional)
              });
    return v;
}

std::vector<std::pair<int, int>> Simulator::pairsForMap(int number_of_algorithms, int number_of_maps) {
    int d = 1 + (int)(number_of_maps % (number_of_algorithms - 1));
    std::set<std::pair<int, int>> uniq;
    for (int i = 0; i < number_of_algorithms; ++i) {
        int j1 = modp(i + d, number_of_algorithms);
        int j2 = modp(i - d, number_of_algorithms);
        if (i != j1) uniq.insert({ min(i, j1), max(i, j1) });
        if (i != j2) uniq.insert({ min(i, j2), max(i, j2) });
    }
    return std::vector(uniq.begin(), uniq.end());
}

void Simulator::runCompetitionMode(const ParsedArgs& args) {
    if (countFilesWithPrefixAndExtension(args.game_maps_folder) == 0) {
        cerr << "map folder is empty, exiting" << endl;
        exit(1);
    }

    if (countFilesWithPrefixAndExtension(args.algorithms_folder, "Algorithm", "so") == 0) {
        cerr << "algorithm folder should contain at least two shared libraries, exiting" << endl;
        exit(1);
    }

    // we expect to have exactly 2 players/algorithms
    auto& play_and_algorithm_registrar = AlgorithmRegistrar::getAlgorithmRegistrar();
    // we expect to have exactly one type of game manager (we'll create many instances)
    auto& game_managers_registrar = GameManagerRegistrar::getGameManagerRegistrar();
    std::vector<std::string> map_names = getAllMapNames(args.game_maps_folder);

    // calculate the matchups
    std::map<int, std::vector<std::pair<int, int>>> matchups;
    for (size_t k = 0; k < map_names.size(); ++k) {
        matchups[(int)k] = pairsForMap((int)play_and_algorithm_registrar.count(), (int)k);
    }

    // We'll capture the manager factory (create one GM per task)
    GameManagerFactory gm_factory = game_managers_registrar.getAt(0);

    // Queue and output mutex
    ConcurrentQueue<Task> queue;
    std::mutex output_mutex;

    std::map<std::string, int> player_scores;

    // Fill the queue with Tasks (one per (map, pair))
    for (auto& [map_index, pairs] : matchups) {
        const std::string& map_filename = map_names[map_index];
        for (auto [player1_index, player2_index] : pairs) {
            auto algo1 = play_and_algorithm_registrar.getAt(player1_index);
            auto algo2 = play_and_algorithm_registrar.getAt(player2_index);

            Task t{
                map_filename,
                player1_index,
                player2_index,
                algo1.getPlayerFactory(),
                algo1.getPlayerName(),
                algo2.getPlayerFactory(),
                algo2.getPlayerName(),
                algo1.getTankAlgorithmFactory(),
                algo2.getTankAlgorithmFactory(),
                gm_factory,
                args.verbose,
                &output_mutex,
                algo1.getPlayerName(),
                algo2.getPlayerName()
            };

            t.on_complete = [&](const Task* task, GameResult&& game_result) {
                // guard the map from multithread access
                switch (game_result.winner) {
                    case 1:
                        player_scores[task->player1_algo_name] += 3;
                        break;
                    case 2:
                        player_scores[task->player2_algo_name] += 3;
                        break;
                    case 0:
                        player_scores[task->player1_algo_name] += 1;
                        player_scores[task->player2_algo_name] += 1;
                        break;
                    default:
                        std::cerr << "unknown game winner value" << std::endl;
                }
            };
            queue.push(std::move(t));
        }
    }

    // Close the queue so workers stop when exhausted
    queue.close();

    // Decide how many threads to use (you can make this an arg if you want)
    const unsigned num_threads = args.num_threads;

    std::vector<std::thread> workers;
    workers.reserve(num_threads);
    for (unsigned i = 0; i < num_threads; ++i) {
        workers.emplace_back([&queue] {
            Task task;
            while (queue.pop(task)) {
                task.run();
            }
        });
    }

    // Join
    for (auto& th : workers) th.join();

    auto winners_vector = sortAndConvertMap(player_scores);

    GameResultWriter::writeCompetitiveResults(args.game_maps_folder,
                                              args.game_manager_so,
                                              winners_vector);
}

std::vector<std::string> Simulator::getAllMapNames(const std::string& folder_path) {
    std::vector<std::string> map_names;
    try {
        for (const auto& entry : std::filesystem::directory_iterator(folder_path)) {
            if (entry.is_regular_file()) {
                map_names.push_back(folder_path + "/" + entry.path().filename().string());
            }
        }
    }
    catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Error accessing folder '" << folder_path
            << "': " << e.what() << "\n";
    }
    return map_names;
}

std::size_t Simulator::countFilesWithPrefixAndExtension(const std::string& dir, const std::string& prefix,
                                                        const std::string& ext)
{
    namespace fs = std::filesystem;

    const bool all_files = ext.empty();
    std::string want_ext;
    if (!all_files) {
        if (!ext.empty() && ext.front() == '.') {
            want_ext = ext;
        } else {
            want_ext = "." + ext;
        }
    }

    std::error_code ec;
    fs::directory_iterator it(dir, ec), end;
    if (ec) {
        throw std::system_error(ec, "Failed to open directory: " + dir);
    }

    std::size_t count = 0;
    for (; it != end; it.increment(ec)) {
        if (ec) {
            throw std::system_error(ec, "Error iterating directory: " + dir);
        }

        const fs::directory_entry& e = *it;
        std::error_code fec;
        if (!e.is_regular_file(fec)) {
            continue;
        }

        if (all_files) {
            // No extension filter -> count every regular file (ignore prefix).
            ++count;
            continue;
        }

        const std::string name = e.path().filename().string();
        if (!prefix.empty()) {
            if (!name.starts_with(prefix)) {
                continue;
            }
        }

        if (e.path().extension().string() == want_ext) {
            ++count;
        }
    }
    return count;
}

