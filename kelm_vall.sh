#!/bin/bash

# kelman_vs_all.sh
# Run kelman's algorithm against all algorithms in algorithm_folder on input_a.txt

ALGOFOLDER="algorithm_folder"
MYALGO="Algorithm/Algorithm_206480972_206899163.so"
MYGM="GameManager"
MAP="maps/input_a.txt"
LOGFILE="kelman_vs_all.log"
# > "$LOGFILE"

for algo in "$ALGOFOLDER"/*.so; do
    # Skip the specific file
    if [[ "$algo" == *"libAlgorithm_208011650_323870188.so" ]]; then
        echo "SKIPPED: $algo"  >> "$LOGFILE"
        continue
    fi
    echo "==============================" >> "$LOGFILE"
    echo "Running: $algo vs $MYALGO on $MAP" >> "$LOGFILE"
    echo "==============================" >> "$LOGFILE"
    echo "COMMAND: ./Simulator/simulator -comparative algorithm1=\"$algo\" algorithm2=\"$MYALGO\" game_managers_folder=$MYGM game_map=\"$MAP\" -verbose" #>> "$LOGFILE"
    ./Simulator/simulator -comparative algorithm1="$algo" algorithm2="$MYALGO" game_managers_folder=$MYGM game_map="$MAP" -verbose >> "$LOGFILE" 2>&1
    if [ $? -ne 0 ]; then
        echo "FAILED: $algo vs $MYALGO on $MAP" >> "$LOGFILE"
    fi
    echo -e "\n\n" >> "$LOGFILE"
done

echo "kelman_vs_all tests completed." >> "$LOGFILE"
