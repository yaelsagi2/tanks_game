#!/bin/bash

SIMULATOR="../Simulator/simulator_206480972_206899163"
ALGS_FOLDER="Algorithm_folder"
GAME_MANAGER_SO="../GameManager/GameManager_206480972_206899163.so"
MAPS_DIR="maps"
LOG_DIR="logs_competition"
mkdir -p "$LOG_DIR"

# 1. Run as is
log_file="$LOG_DIR/as_is.log"
echo "[RUN] as is" | tee "$log_file"
$SIMULATOR -competition algorithms_folder=$ALGS_FOLDER game_manager=$GAME_MANAGER_SO game_maps_folder=$MAPS_DIR >> "$log_file" 2>&1

echo "" | tee -a "$log_file"

# 2. Run with spaces after =
log_file="$LOG_DIR/with_spaces.log"
echo "[RUN] with spaces after =" | tee "$log_file"
$SIMULATOR -competition "algorithms_folder = $ALGS_FOLDER" "game_manager = $GAME_MANAGER_SO" "game_maps_folder = $MAPS_DIR" >> "$log_file" 2>&1

echo "" | tee -a "$log_file"

echo "" | tee -a "$log_file"
echo "" | tee -a "$log_file"

# 3. Run with -verbose
log_file="$LOG_DIR/with_verbose.log"
echo "[RUN] with -verbose" | tee "$log_file"
$SIMULATOR -competition algorithms_folder=$ALGS_FOLDER game_manager=$GAME_MANAGER_SO game_maps_folder=$MAPS_DIR -verbose >> "$log_file" 2>&1
echo "" | tee -a "$log_file"

# 4. Run with num_threads=2,4,8
for n in 2 4 8; do
  log_file="$LOG_DIR/num_threads_${n}.log"
  echo "[RUN] with num_threads=$n" | tee "$log_file"
  $SIMULATOR -competition algorithms_folder=$ALGS_FOLDER game_manager=$GAME_MANAGER_SO game_maps_folder=$MAPS_DIR num_threads=$n >> "$log_file" 2>&1
  echo "" | tee -a "$log_file"
done
echo "" | tee -a "$log_file"

# 5. Run with -verbose and num_threads
for n in 2 4 8; do
  log_file="$LOG_DIR/verbose_num_threads_${n}.log"
  echo "[RUN] with -verbose and num_threads=$n" | tee "$log_file"
  $SIMULATOR -competition algorithms_folder=$ALGS_FOLDER game_manager=$GAME_MANAGER_SO game_maps_folder=$MAPS_DIR -verbose num_threads=$n >> "$log_file" 2>&1
  echo "" | tee -a "$log_file"
done
echo "" | tee -a "$log_file"

# 6. Run with missing parameters
# Missing game manager
log_file="$LOG_DIR/missing_game_manager.log"
echo "[RUN] missing game_manager" | tee "$log_file"
$SIMULATOR -competition algorithms_folder=$ALGS_FOLDER game_maps_folder=$MAPS_DIR >> "$log_file" 2>&1
echo "" | tee -a "$log_file"

# Missing maps folder
log_file="$LOG_DIR/missing_maps_folder.log"
echo "[RUN] missing game_maps_folder" | tee "$log_file"
$SIMULATOR -competition algorithms_folder=$ALGS_FOLDER game_manager=$GAME_MANAGER_SO >> "$log_file" 2>&1
echo "" | tee -a "$log_file"

# Missing algorithm folder
log_file="$LOG_DIR/missing_algorithms_folder.log"
echo "[RUN] missing algorithms_folder" | tee "$log_file"
$SIMULATOR -competition game_manager=$GAME_MANAGER_SO game_maps_folder=$MAPS_DIR >> "$log_file" 2>&1
echo "" | tee -a "$log_file"

# 5. Run with empty algorithm folder
EMPTY_ALGS_FOLDER="AutomaticTester/algorithms_empty"
mkdir -p "$EMPTY_ALGS_FOLDER"
log_file="$LOG_DIR/empty_algos.log"
echo "[RUN] with empty algorithm folder" | tee "$log_file"
$SIMULATOR -competition algorithms_folder=$EMPTY_ALGS_FOLDER game_manager=$GAME_MANAGER_SO game_maps_folder=$MAPS_DIR >> "$log_file" 2>&1

echo "All competition tests complete. See $LOG_DIR for results."
