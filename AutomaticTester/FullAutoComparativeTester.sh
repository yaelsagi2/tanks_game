#!/bin/bash
# FullAutoComparativeTester.sh - Test two algorithms with GameManager in comparative mode under various scenarios
# Usage: ./FullAutoComparativeTester.sh
# Place your map files in the 'maps' subfolder.


#Instructions running:
# cd AutomaticTester
# chmod +x FullAutoComparativeTester.sh
# ./FullAutoComparativeTester.sh

SIMULATOR="../Simulator/simulator_206480972_206899163"
ALG1="../Algorithm/Algorithm_206480972_206899163.so"
ALG2="../Algorithm/Algorithm_206480972_206899163.so"
GMS_FOLDER="../GameManager"
MAPS_DIR="maps"
LOG_DIR="logs"
mkdir -p "$LOG_DIR"

# 1. Test with all parameters (default, verbose, num_threads)
MODES=(
    "default" "" #without verbose, and num_threads
    "verbose" "-verbose"
    "threads2eq" "num_threads=2"
    "threads4eq" "num_threads=4"
    "threads8eq" "num_threads=8"
    "threads2spaceeq" "num_threads= 2"
    "threads4spaceeq" "num_threads= 4"
    "threads8spaceeq" "num_threads= 8"
    "verbose_threads2eqspaces" "-verbose num_threads=  2"
    "verbose_threads4eqspaces" "-verbose num_threads=  4"
    "verbose_threads8eqspaces" "-verbose num_threads=  8"
    "verbose_threads2spacewithout" "-verbose num_threads 2"
    "verbose_threads4spacewithout" "-verbose num_threads 4"
    "verbose_threads8spacewithout" "-verbose num_threads 8"
)


# Run all checks for each map, but skip threading for valid_input_complex

# Run all checks for each map, but for valid_input_complex only run one threading check (threads4eq)
for map in "$MAPS_DIR"/*.txt; do
    name=$(basename "$map" .txt)
    for ((i=0; i<${#MODES[@]}; i+=2)); do
        MODE_NAME="${MODES[i]}"
        MODE_FLAGS="${MODES[i+1]}"
        # For valid_input_complex, only run threads4eq for threading, skip all other threading modes
        if [[ "$name" == "valid_input_complex" ]]; then
            if [[ "$MODE_NAME" == threads* || "$MODE_NAME" == verbose_threads* ]]; then
                if [[ "$MODE_NAME" != "threads4eq" ]]; then continue; fi
            fi
        fi
        # Add a comment to the log file describing the check
        log_file="$LOG_DIR/${name}_${MODE_NAME}.log"
        {
          echo "# Test: $MODE_NAME"
          echo "# Map: $name"
          echo "# Command: $SIMULATOR -comparative algorithm1=$ALG1 algorithm2=$ALG2 game_managers_folder=$GMS_FOLDER game_map=$map $MODE_FLAGS"
        } > "$log_file"
        echo "Running: $SIMULATOR -comparative algorithm1=$ALG1 algorithm2=$ALG2 game_managers_folder=$GMS_FOLDER game_map=$map $MODE_FLAGS"
        $SIMULATOR -comparative algorithm1=$ALG1 algorithm2=$ALG2 game_managers_folder=$GMS_FOLDER game_map=$map $MODE_FLAGS >> "$log_file" 2>&1
        echo "  -> Output saved to $log_file"
    done
done


# 2. Test missing/invalid parameters
# Missing map file
$SIMULATOR -comparative algorithm1=$ALG1 algorithm2=$ALG2 game_managers_folder=$GMS_FOLDER > "$LOG_DIR/missing_map.log" 2>&1
# Missing algorithm1
$SIMULATOR -comparative algorithm2=$ALG2 game_managers_folder=$GMS_FOLDER game_map=$MAPS_DIR/input_a.txt > "$LOG_DIR/missing_alg1.log" 2>&1
# Missing algorithm2
$SIMULATOR -comparative algorithm1=$ALG1 game_managers_folder=$GMS_FOLDER game_map=$MAPS_DIR/input_a.txt > "$LOG_DIR/missing_alg2.log" 2>&1
    # Test: All parameters with a space after '='
    {
        log_file="$LOG_DIR/all_params_space_after_eq.log"
        echo "# Test: All parameters with a space after '='" > "$log_file"
        echo "# Command: $SIMULATOR -comparative algorithm1= $ALG1 algorithm2= $ALG2 game_managers_folder= $GMS_FOLDER game_map= $MAPS_DIR/input_a.txt num_threads= 4" >> "$log_file"
        echo "Running: $SIMULATOR -comparative algorithm1= $ALG1 algorithm2= $ALG2 game_managers_folder= $GMS_FOLDER game_map= $MAPS_DIR/input_a.txt num_threads= 4"
        $SIMULATOR -comparative algorithm1= $ALG1 algorithm2= $ALG2 game_managers_folder= $GMS_FOLDER game_map= $MAPS_DIR/input_a.txt num_threads= 4 >> "$log_file" 2>&1
        echo "  -> Output saved to $log_file"
    }

# Missing game_managers_folder
$SIMULATOR -comparative algorithm1=$ALG1 algorithm2=$ALG2 game_map=$MAPS_DIR/input_a.txt > "$LOG_DIR/missing_gms_folder.log" 2>&1

# Map folder exists but is empty
NO_MAPS_DIR="no_maps_inside"
mkdir -p "$NO_MAPS_DIR"
$SIMULATOR -comparative algorithm1=$ALG1 algorithm2=$ALG2 game_managers_folder=$GMS_FOLDER game_map=$NO_MAPS_DIR/ > "$LOG_DIR/no_maps_inside.log" 2>&1

# Test with a non-existent map file (no_map.txt)
{
    log_file="$LOG_DIR/no_map_txt.log"
    echo "# Test: non-existent map file (no_map.txt)" > "$log_file"
    echo "# Command: $SIMULATOR -comparative algorithm1=$ALG1 algorithm2=$ALG2 game_managers_folder=$GMS_FOLDER game_map=$MAPS_DIR/no_map.txt" >> "$log_file"
    echo "Running: $SIMULATOR -comparative algorithm1=$ALG1 algorithm2=$ALG2 game_managers_folder=$GMS_FOLDER game_map=$MAPS_DIR/no_map.txt"
    $SIMULATOR -comparative algorithm1=$ALG1 algorithm2=$ALG2 game_managers_folder=$GMS_FOLDER game_map=$MAPS_DIR/no_map.txt >> "$log_file" 2>&1
    echo "  -> Output saved to $log_file"
}



$SIMULATOR -comparative algorithm1=$ALG1 algorithm2=$ALG2 game_managers_folder=$GMS_FOLDER game_map=$MAPS_DIR/input_a.txt num_threads=4 > "$LOG_DIR/num_threads_eq4.log" 2>&1
$SIMULATOR -comparative algorithm1=$ALG1 algorithm2=$ALG2 game_managers_folder=$GMS_FOLDER game_map=$MAPS_DIR/input_a.txt "num_threads = 4" > "$LOG_DIR/num_threads_space_eq_4.log" 2>&1
# 3. Test with no maps in folder
NO_MAPS_DIR="no_maps_inside"
mkdir -p "$NO_MAPS_DIR"
$SIMULATOR -comparative algorithm1=$ALG1 algorithm2=$ALG2 game_managers_folder=$GMS_FOLDER game_map=$NO_MAPS_DIR/ > "$LOG_DIR/no_maps_inside.log" 2>&1

echo "\nAll comparative tests complete. See $LOG_DIR for results."
