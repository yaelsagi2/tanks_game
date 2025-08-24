#!/bin/bash

# kelman_vs_all_maps.sh
# Run kelman's algorithm against itself on all maps in maps/

MYALGO="Algorithm/Algorithm_206480972_206899163.so"
MYGMFOLDER="Game_Manager_folder"
MAPFOLDER="maps"
LOGFILE="kelman_vs_all_maps.log"
> "$LOGFILE"

for map in "$MAPFOLDER"/*.txt; do
    echo "==============================" >> "$LOGFILE"
    echo "Running: $MYALGO vs $MYALGO on $map" >> "$LOGFILE"
    echo "==============================" >> "$LOGFILE"
    echo "COMMAND: ./Simulator/simulator -comparative algorithm1=\"$MYALGO\" algorithm2=\"$MYALGO\" game_managers_folder=$MYGMFOLDER game_map=\"$map\"" >> "$LOGFILE"
    ./Simulator/simulator -comparative algorithm1="$MYALGO" algorithm2="$MYALGO" game_managers_folder=$MYGMFOLDER game_map="$map" >> "$LOGFILE" 2>&1
    if [ $? -ne 0 ]; then
        echo "FAILED: $MYALGO vs $MYALGO on $map" >> "$LOGFILE"
    fi
    echo -e "\n\n" >> "$LOGFILE"
done

echo "kelman_vs_all_maps tests completed." >> "$LOGFILE"
