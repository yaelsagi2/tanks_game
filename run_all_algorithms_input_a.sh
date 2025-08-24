#!/bin/bash

# Output log file
LOGFILE="all algorithm - input_a"

# Clear the log file if it exists
> "$LOGFILE"

for algo in algorithm_folder/*.so; do
    echo "==============================" >> "$LOGFILE"
    echo "Running: $algo" >> "$LOGFILE"
    echo "==============================" >> "$LOGFILE"
    ./Simulator/simulator -comparative algorithm1="$algo" algorithm2=Algorithm/Algorithm_206480972_206899163.so game_managers_folder=GameManager game_map=maps/input_a.txt -verbose >> "$LOGFILE" 2>&1
    echo -e "\n\n" >> "$LOGFILE"
done
