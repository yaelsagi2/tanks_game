#pragma once

#include <vector>
#include <string>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include "MapParser.h"
#include "../common/Player.h"
#include "../common/TankAlgorithm.h"
#include "../common/AbstractGameManager.h"

struct Task {
    std::string                map_filename;
    int                        player1_index;
    int                        player2_index;


    // Factories captured at task creation (thread-safe to copy/use)
    PlayerFactory              pf1;
	std::string 			   player1_name;
    PlayerFactory              pf2;
	std::string 			   player2_name;

    TankAlgorithmFactory       taf1;
    TankAlgorithmFactory       taf2;
    GameManagerFactory         gm_factory;

    bool                       verbose = false;

    // Optional serialization for output
    std::mutex* output_mutex = nullptr;

    std::string player1_algo_name = "";
    std::string player2_algo_name = "";
    std::string game_manager = "";

    std::function<void(const Task *task, GameResult&& game_result)> on_complete = nullptr;

    // The function names referenced here must exist in your codebase:
    // - readMapFile(const std::string&, std::vector<std::string>&)
    // - write_game_result_to_file(const GameResult&)
    // - SatelliteView( MapType* )   // ctor takes pointer to the map object
    // - gm->run(length, height, view, max_steps, num_shells, player1, player2, taf1, taf2)
    void run();
};
