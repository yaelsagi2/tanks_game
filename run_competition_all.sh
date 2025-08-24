#!/bin/bash

# run_competition_all.sh
# Run the competition mode for all algorithms in the specified folder and all maps in the specified folder

SIMULATOR="./Simulator/simulator"  # Update this to your actual simulator binary name if different
ALGOFOLDER="algorithm_folder"
MAPFOLDER="maps"
GMSO="Game_Manager_folder/GameManager_206480972_206899163.so"  # Update to your actual GameManager .so file
LOGFILE="competition_all.log"
> "$LOGFILE"

for map in "$MAPFOLDER"/*.txt; do
    echo "==============================" >> "$LOGFILE"
    echo "Running competition on $map with all algorithms in $ALGOFOLDER" >> "$LOGFILE"
    echo "==============================" >> "$LOGFILE"
    echo "COMMAND: $SIMULATOR -competition game_maps_folder=$MAPFOLDER game_manager=$GMSO algorithms_folder=$ALGOFOLDER -verbose" >> "$LOGFILE"
    $SIMULATOR -competition game_maps_folder="$MAPFOLDER" game_manager="$GMSO" algorithms_folder="$ALGOFOLDER" -verbose >> "$LOGFILE" 2>&1
    if [ $? -ne 0 ]; then
        echo "FAILED: competition on $map with $GMSO and $ALGOFOLDER" >> "$LOGFILE"
    fi
    echo -e "\n\n" >> "$LOGFILE"
done

echo "Competition runs completed." >> "$LOGFILE"
