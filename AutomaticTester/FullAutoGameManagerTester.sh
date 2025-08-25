
# 0. Test with empty Game_Manager_folder (no .so files)
EMPTY_GM_FOLDER="empty_gm_folder"
mkdir -p "$EMPTY_GM_FOLDER"
for map in "$MAPS_DIR"/*.txt; do
  name=$(basename "$map" .txt)
  if [[ "$name" == "valid_input_complex" ]]; then continue; fi
  log_file="$LOG_DIR/${name}_gm_emptyfolder.log"
  {
    echo "# Test: Empty Game_Manager_folder (no .so files) on $name"
    echo "# Command: $SIMULATOR -comparative algorithm1=$ALG1 algorithm2=$ALG2 game_managers_folder=$EMPTY_GM_FOLDER game_map=$map"
  } > "$log_file"
  echo "Running: $SIMULATOR -comparative algorithm1=$ALG1 algorithm2=$ALG2 game_managers_folder=$EMPTY_GM_FOLDER game_map=$map"
  $SIMULATOR -comparative algorithm1=$ALG1 algorithm2=$ALG2 game_managers_folder=$EMPTY_GM_FOLDER game_map=$map >> "$log_file" 2>&1
  echo "  -> Output saved to $log_file"
done


# 0b. Test with empty maps folder
EMPTY_MAPS_DIR="empty_maps_folder"
mkdir -p "$EMPTY_MAPS_DIR"
GM_FOLDER="Game_Manager_folder"
if [ -d "$GM_FOLDER" ]; then
  log_file="$LOG_DIR/gm_${GM_FOLDER}_maps_empty.log"
  {
    echo "# Test: $GM_FOLDER with empty maps folder"
    echo "# Command: $SIMULATOR -comparative algorithm1=$ALG1 algorithm2=$ALG2 game_managers_folder=$GM_FOLDER game_map=$EMPTY_MAPS_DIR/"
  } > "$log_file"
  echo "Running: $SIMULATOR -comparative algorithm1=$ALG1 algorithm2=$ALG2 game_managers_folder=$GM_FOLDER game_map=$EMPTY_MAPS_DIR/"
  $SIMULATOR -comparative algorithm1=$ALG1 algorithm2=$ALG2 game_managers_folder=$GM_FOLDER game_map=$EMPTY_MAPS_DIR/ >> "$log_file" 2>&1
  echo "  -> Output saved to $log_file"
else
  echo "Game_Manager_folder not found in AutomaticTester. Please create it and add your GameManager files."
fi
#!/bin/bash
# FullAutoGameManagerTester.sh - Test your algorithms with different GameManager folders and map scenarios
# Usage: ./FullAutoGameManagerTester.sh

SIMULATOR="../Simulator/simulator_206480972_206899163"
ALG1="../Algorithm/Algorithm_206480972_206899163.so"
ALG2="../Algorithm/Algorithm_206480972_206899163.so"
MAPS_DIR="maps"
LOG_DIR="logs_gm"
mkdir -p "$LOG_DIR"

# Only use Game_Manager_folder inside AutomaticTester
GM_FOLDER="Game_Manager_folder"
if [ -d "$GM_FOLDER" ]; then
  for map in "$MAPS_DIR"/*.txt; do
    name=$(basename "$map" .txt)
  if [[ "$name" == "valid_input_complex" ]]; then continue; fi
    gm_name=$(basename "$GM_FOLDER")
    log_file="$LOG_DIR/${name}_gm_${gm_name}.log"
    {
      echo "# Test: $gm_name on $name"
      echo "# Command: $SIMULATOR -comparative algorithm1=$ALG1 algorithm2=$ALG2 game_managers_folder=$GM_FOLDER game_map=$map"
    } > "$log_file"
    echo "Running: $SIMULATOR -comparative algorithm1=$ALG1 algorithm2=$ALG2 game_managers_folder=$GM_FOLDER game_map=$map"
    $SIMULATOR -comparative algorithm1=$ALG1 algorithm2=$ALG2 game_managers_folder=$GM_FOLDER game_map=$map >> "$log_file" 2>&1
    echo "  -> Output saved to $log_file"
  done
else
  echo "Game_Manager_folder not found in AutomaticTester. Please create it and add your GameManager files."
fi

# 2. Test with empty GameManager folder value
for map in "$MAPS_DIR"/*.txt; do
  name=$(basename "$map" .txt)
  if [[ "$name" == "valid_input_complex" ]]; then continue; fi
  log_file="$LOG_DIR/${name}_gm_empty.log"
  {
    echo "# Test: Empty GameManager folder on $name"
    echo "# Command: $SIMULATOR -comparative algorithm1=$ALG1 algorithm2=$ALG2 game_managers_folder= game_map=$map"
  } > "$log_file"
  echo "Running: $SIMULATOR -comparative algorithm1=$ALG1 algorithm2=$ALG2 game_managers_folder= game_map=$map"
  $SIMULATOR -comparative algorithm1=$ALG1 algorithm2=$ALG2 game_managers_folder= game_map=$map >> "$log_file" 2>&1
  echo "  -> Output saved to $log_file"
done


# 3. Test with empty map value

# Only use Game_Manager_folder for empty map value test
GM_FOLDER="Game_Manager_folder"
if [ -d "$GM_FOLDER" ]; then
  gm_name=$(basename "$GM_FOLDER")
  log_file="$LOG_DIR/gm_${gm_name}_map_empty.log"
  {
    echo "# Test: $gm_name with empty map value"
    echo "# Command: $SIMULATOR -comparative algorithm1=$ALG1 algorithm2=$ALG2 game_managers_folder=$GM_FOLDER game_map="
  } > "$log_file"
  echo "Running: $SIMULATOR -comparative algorithm1=$ALG1 algorithm2=$ALG2 game_managers_folder=$GM_FOLDER game_map="
  $SIMULATOR -comparative algorithm1=$ALG1 algorithm2=$ALG2 game_managers_folder=$GM_FOLDER game_map= >> "$log_file" 2>&1
  echo "  -> Output saved to $log_file"
else
  echo "Game_Manager_folder not found in AutomaticTester. Please create it and add your GameManager files."
fi

echo "\nAll GameManager comparative tests complete. See $LOG_DIR for results."
