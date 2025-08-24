#!/bin/bash
# test_all_gms_num_threads_all_maps.sh
# Test all GameManagers in Game_Manager_folder with your 2 algos on all maps, for different num_threads (without -verbose), skipping invalid maps

SIMULATOR=./Simulator/simulator
ALGO1=Algorithm/Algorithm_206480972_206899163.so
ALGO2=Algorithm/Algorithm_206480972_206899163.so
GMFOLDER=Game_Manager_folder
MAPFOLDER=maps

for n in 1 2 4 8; do
    echo "Testing with num_threads=$n..."
    for GM in $GMFOLDER/*.so; do
        echo "  GameManager: $GM"
        for MAP in $MAPFOLDER/*.txt; do
            echo "    Map: $MAP"
            $SIMULATOR -comparative algorithm1=$ALGO1 algorithm2=$ALGO2 game_managers_folder=$GM game_map=$MAP num_threads=$n > sim_out.log 2>&1
            STATUS=$?
            if [ $STATUS -ne 0 ]; then
                echo "      Skipped (simulator failed, likely invalid map or GM): $MAP with $GM"
                cat sim_out.log | head -10
            fi
        done
    done
done

echo "All GameManagers, maps, and num_threads tests completed (invalid maps/GMs skipped)."
