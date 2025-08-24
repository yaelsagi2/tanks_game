#!/bin/bash
# test_num_threads.sh
# Test Simulator with different num_threads values (without -verbose)

SIMULATOR=./Simulator/simulator
MAP=maps/valid_input_3v3_P1_trapped.txt
ALGO1=Algorithm/Algorithm_206480972_206899163.so
ALGO2=Algorithm/Algorithm_206480972_206899163.so
GMFOLDER=GameManager

for n in 1 2 4 8; do
    echo "Testing with num_threads=$n..."
    $SIMULATOR -comparative algorithm1=$ALGO1 algorithm2=$ALGO2 game_managers_folder=$GMFOLDER game_map=$MAP num_threads=$n || { echo "Failed for num_threads=$n"; exit 1; }
done

echo "All num_threads tests passed."
