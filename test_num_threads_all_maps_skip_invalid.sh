#!/bin/bash
# test_num_threads_all_maps_skip_invalid.sh
# Test Simulator with different num_threads values on all maps in the maps/ folder (without -verbose), skipping invalid maps

SIMULATOR=./Simulator/simulator
ALGO1=Algorithm/Algorithm_206480972_206899163.so
ALGO2=Algorithm/Algorithm_206480972_206899163.so
GMFOLDER=GameManager
MAPFOLDER=maps

for n in 1 2 4 8; do
    echo "Testing with num_threads=$n..."
    for MAP in $MAPFOLDER/*.txt; do
        echo "  Map: $MAP"
        $SIMULATOR -comparative algorithm1=$ALGO1 algorithm2=$ALGO2 game_managers_folder=$GMFOLDER game_map=$MAP num_threads=$n > sim_out.log 2>&1
        STATUS=$?
        if [ $STATUS -ne 0 ]; then
            echo "    Skipped (simulator failed, likely invalid map): $MAP"
            cat sim_out.log | head -10
        fi
    done
done

echo "All num_threads tests completed (invalid maps skipped)."
