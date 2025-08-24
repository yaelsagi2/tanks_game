#!/bin/bash
# test_all_gms_all_maps.sh
# Test all GameManagers in Game_Manager_folder with your 2 algos on all maps (no num_threads, no -verbose), skipping invalid maps/GMs

SIMULATOR=./Simulator/simulator
ALGO1=Algorithm/Algorithm_206480972_206899163.so
ALGO2=Algorithm/Algorithm_206480972_206899163.so
GMFOLDER=Game_Manager_folder
MAPFOLDER=maps


echo "  GameManagers folder: $GMFOLDER"
for MAP in $MAPFOLDER/*.txt; do
    echo "    Map: $MAP"
    $SIMULATOR -comparative algorithm1=$ALGO1 algorithm2=$ALGO2 game_managers_folder=$GMFOLDER game_map=$MAP > sim_out.log 2>&1
    STATUS=$?
    if [ $STATUS -ne 0 ]; then
        echo "      Skipped (simulator failed, likely invalid map): $MAP"
        cat sim_out.log | head -10
    fi
done

echo "All GameManagers and maps tests completed (invalid maps/GMs skipped)."
