#include "Task.h"
#include "MapDataSatelliteView.h"

void Task::run() {
    try {
        // 1) Read map (local, per task)
        std::vector<std::string> errors;
        auto map_info = readMapFile(map_filename, errors);
        // 2) Create per-task game manager
        auto gm = gm_factory(verbose);

        // 3) Build players for this map
        auto p1_up = pf1(player1_index, map_info.rows, map_info.cols,
            map_info.max_steps, map_info.num_shells);

        auto p2_up = pf2(player2_index, map_info.rows, map_info.cols,
            map_info.max_steps, map_info.num_shells);

        MapDataSatelliteView sat_view(&map_info);

        auto game_result = gm->run(map_info.cols, map_info.rows, sat_view, map_filename,
            map_info.max_steps,map_info.num_shells, *p1_up, player1_name, *p2_up, player2_name,taf1, taf2
        );
        if (on_complete) {
            on_complete(this, std::move(game_result));
        }
    }
    catch (const std::exception& ex) {
        // Optional: log & continue so one bad task doesn’t crash the pool
        std::cerr << "Task failed: " << ex.what() << "\n";
    }
}
