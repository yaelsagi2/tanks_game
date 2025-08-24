#!/bin/bash

# test_all_algorithms_vs_mine.sh
# Test all algorithms in algorithm_folder against the user's algorithm using the user's GameManager on all maps


ALGOFOLDER="../tanks_game/algorithm_folder"
MYALGO="Algorithm/Algorithm_206480972_206899163.so"
MYGM="Game_Manager_folder"
MAPFOLDER="maps"
LOGFILE="all_algorithms_vs_mine.log"
> "$LOGFILE"

for algo in "$ALGOFOLDER"/*.so; do
    # Skip if this is the user's own algorithm
    if [[ "$algo" == *"206480972_206899163.so"* ]]; then
        continue
    fi
    for map in "$MAPFOLDER"/*.txt; do
        echo "==============================" >> "$LOGFILE"
        echo "Running: $algo vs $MYALGO on $map" >> "$LOGFILE"
        echo "==============================" >> "$LOGFILE"
    echo "COMMAND: ./Simulator/simulator -comparative algorithm1=\"$algo\" algorithm2=\"$MYALGO\" game_managers_folder=$MYGM game_map=\"$map\" -verbose" >> "$LOGFILE"
    ./Simulator/simulator -comparative algorithm1="$algo" algorithm2="$MYALGO" game_managers_folder=$MYGM game_map="$map" -verbose >> "$LOGFILE" 2>&1
        if [ $? -ne 0 ]; then
            echo "FAILED: $algo vs $MYALGO on $map" >> "$LOGFILE"
        fi
        echo -e "\n\n" >> "$LOGFILE"
    done

done

echo "All algorithms vs mine tests completed." >> "$LOGFILE"
