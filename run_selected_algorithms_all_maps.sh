#!/bin/bash

LOGFILE="Algorithm_206480972_123456789_vs_Algorithm_206480972_206899163_all_maps.log"
> "$LOGFILE"

algo1="Algorithm/Algorithm_206480972_206899163.so"
algo2="Algorithm/Algorithm_206480972_206899163.so"

for map in maps/*; do
    echo "==============================" >> "$LOGFILE"
    echo "Running: $algo1 vs $algo2 on $map" >> "$LOGFILE"
    echo "==============================" >> "$LOGFILE"
  echo "COMMAND: ./Simulator/simulator -comparative algorithm1=\"$algo1\" algorithm2=\"$algo2\" game_managers_folder=GameManager game_map=\"$map\" -verbose"
  echo "########################################"
    ./Simulator/simulator -comparative algorithm1="$algo1" algorithm2="$algo2" game_managers_folder=GameManager game_map="$map" -verbose >> "$LOGFILE" 2>&1
    if [ $? -ne 0 ]; then
      echo "FAILED: $algo1 vs $algo2 on $map" >> "$LOGFILE"
    fi
    echo -e "\n\n" >> "$LOGFILE"
done
