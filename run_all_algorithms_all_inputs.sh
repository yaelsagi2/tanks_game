#!/bin/bash

LOGFILE="all_algorithm_all_inputs.log"
> "$LOGFILE"

# List of valid map files (excluding known invalid/test/error files)
map_files=(
  input_a.txt
  valid_extra_cols.txt
  valid_extra_rows.txt
  valid_input_2v7.txt
  valid_input_3v3_P1_trapped.txt
  valid_input_3v3_P2_trapped.txt
  valid_input_7v2.txt
  valid_input_complex.txt
  valid_input_minefield.txt
  valid_input_simple.txt
  valid_missing_rows.txt
  valid_mixed_size.txt
  valid_short_rows.txt
)

for algo in algorithm_folder/*.so; do
  for map in "${map_files[@]}"; do
    echo "==============================" >> "$LOGFILE"
    echo "Running: $algo on maps/$map" >> "$LOGFILE"
    echo "==============================" >> "$LOGFILE"
    ./Simulator/simulator -comparative algorithm1="$algo" algorithm2=Algorithm/Algorithm_206480972_206899163.so game_managers_folder=GameManager game_map="maps/$map" -verbose >> "$LOGFILE" 2>&1
    # Check if last command failed (nonzero exit code)
    if [ $? -ne 0 ]; then
      echo "FAILED: $algo on $map" >> "$LOGFILE"
    fi
    echo -e "\n\n" >> "$LOGFILE"
  done
done
