#include "MapParser.h"
#include "../common/SatelliteView.h"
#include <fstream>

// This file contains functions for parsing map files.

std::string trim(const std::string& str) {
    // This function removes leading and trailing whitespace from a string.
    size_t start = str.find_first_not_of(" \t\n\r\f\v");

    if (start == std::string::npos) { // String is all whitespace
        return ""; 
    }
    size_t end = str.find_last_not_of(" \t\n\r\f\v");
    return (start == std::string::npos) ? "" : str.substr(start, end - start + 1);
}


int parseKeyValue(const std::string& line, std::string key , std::vector<std::string>& errors, int num_line, bool& isValid) {
    // This function parsea a key-value pair from a line.
    size_t equal_pos = line.find('=');
    std::string trimmed_line = trim(line);
    if (trimmed_line.empty()) {
        errors.push_back("Line " + std::to_string(num_line) + ": expected header '" + key + "=...', found empty line");
        isValid = false;
        return -1;
    }
    if (equal_pos == std::string::npos) {
        errors.push_back("Line " + std::to_string(num_line) +
                         ": expected header '" + key + "=...', found '" + trimmed_line + "'");
        isValid = false;
        return -1;
    }
    std::string k = trim(line.substr(0, equal_pos));  // Extract key
    std::string v = trim(line.substr(equal_pos + 1)); // Extract value
    if ( k != key ) {
        errors.push_back("Line " + std::to_string(num_line) + ": expected key '" + key + "', found '" + k + "'");
        isValid = false;
        return -1;
    } 
    if (v.empty()) {
        errors.push_back("Line " + std::to_string(num_line) +
                         ": invalid or missing value for key '" + key + "'");
        isValid = false;
        return -1;
    }
    try {
        return std::stoi(v); // Convert value to integer
    } catch (...) {
        errors.push_back("Line " + std::to_string(num_line) + ": invalid value '" + v + "' for key '" + key + "'");
        isValid = false;
        return -1; // Return -1 on failure
    }
}

MapData readMapFile(const std::string& filename, std::vector<std::string>& errors) {
    // Reads and parses the game board from a file.
    std::ifstream input_file(filename);
    std::vector<std::string> header_errors;
    if (!input_file.is_open()) {
        std::cerr << "Failed to open file " + filename + " File does not exist or is not accessible." << std::endl;
        throw std::runtime_error("map file does not exist");
    }
    int max_steps, num_shells, rows, cols, actual_row = 0;
    std::string skip_line;
    if(!std::getline(input_file, skip_line)) // Skip the first line (map name)
    {
        std::cerr << "Invalid map: missing map name" << std::endl;
        throw std::runtime_error("map name is missing");
    }
    if (!readHeadersLine(input_file, max_steps, num_shells, rows, cols, header_errors)) {
        std::cerr << "Invalid map headers." << std::endl;
        for (const auto& msg : header_errors) { std::cerr << msg << std::endl; }
        throw std::runtime_error("invalid map headers");
    }
    std::string line;
    std::vector<std::vector<char>> grid(cols, std::vector<char>(rows, ' '));
    while (actual_row < rows && std::getline(input_file, line)) {
        line = normalizeLine(line, cols, actual_row + 6, errors);
        for (int col = 0; col < cols; ++col) { handleCell(line[col], actual_row, col, grid, errors);}
        actual_row++;
    }
    for (; actual_row < rows; ++actual_row) { // Fill remaining rows with spaces
        errors.push_back("Line " + std::to_string(actual_row + 6) + ": missing, padding with spaces.");
        for (int col = 0; col < cols; ++col) { grid[actual_row][col] = ' ';}
    }
    if (std::getline(input_file, line)) { errors.push_back("Extra lines beyond declared Rows ignored.");}
    if (!errors.empty()) {
        std::ofstream errorFile("input_errors.txt");
        if (errorFile.is_open()) {
            for (const std::string& msg : errors) { errorFile << msg << std::endl; }
            errorFile.close();
        } else { std::cerr << "Failed to write to input_errors.txt" << std::endl;}
    }
    return MapData(max_steps, num_shells, rows, cols, grid);
}

bool readHeadersLine(std::ifstream& input_file,int& maxSteps, int& numShells, int& rows, int& cols, std::vector<std::string>& header_errors) {
    // Reads the header line of the map file.
    std::string line;
    bool ok = true;
    maxSteps = parseKeyValue((std::getline(input_file, line), line), "MaxSteps", header_errors, 2, ok);
    if (!ok) {
        header_errors.push_back("Missing MaxSteps value.");
        return false; // Return early if MaxSteps is invalid
    }
    if (maxSteps <= 0) {
        header_errors.push_back("Invalid MaxSteps value, must be > 0");
        return false; // Return early if MaxSteps is invalid
    }
    numShells = parseKeyValue((std::getline(input_file, line), line), "NumShells", header_errors, 3, ok);
    if (!ok) {
        header_errors.push_back("Missing NumShells value.");
        return false; // Return early if NumShells is invalid
    }
    if (numShells < 0) {
        header_errors.push_back("Invalid NumShells value, must be >= 0");
        return false; // Return early if NumShells is invalid
    }
    rows = parseKeyValue((std::getline(input_file, line), line), "Rows", header_errors, 4, ok);
    if (!ok) {
        header_errors.push_back("Missing Rows value.");
        return false; // Return early if Rows is invalid
    }
    if (rows <= 0) {
        header_errors.push_back("Invalid Rows value, must be > 0");
        return false; // Return early if Rows is invalid
    }
    cols = parseKeyValue((std::getline(input_file, line), line), "Cols", header_errors, 5, ok);
    if (!ok) {
        header_errors.push_back("Missing Cols value.");
        return false; // Return early if Cols is invalid
    }
    if (cols <= 0) { header_errors.push_back("Invalid Cols value, must be > 0");
        return false; // Return early if Cols is invalid
    }
    return true;
}


std::string normalizeLine(std::string line, int expected_len, int line_num, std::vector<std::string>& errors) {
    // This function normalizes a line to the expected length.
    if (!line.empty() && line.back() == '\r') line.pop_back();

    if ((int)line.size() < expected_len) {
        errors.push_back("Line " + std::to_string(line_num) + " is too short, padding with spaces.");
        line += std::string(expected_len - line.size(), ' ');
    } else if ((int)line.size() > expected_len) {
        errors.push_back("Line " + std::to_string(line_num) + " is too long, trimming.");
        line.resize(expected_len);
    }
    return line;
}


void handleCell(char cell, int row, int col, std::vector<std::vector<char>>& grid, std::vector<std::string>& errors) {
    // This function handles a single cell in the map and creates the corresponding game object.

    if (cell == '#') {
        grid[col][row] = '#'; // Wall
    } else if (cell == '@') {
        grid[col][row] = '@'; // Mine
    } else if (cell == '1') {
        grid[col][row] = '1'; // Player 1 tank
    } else if (cell == '2') {
        grid[col][row] = '2'; // Player 2 tank
    } else {
        if (!std::isspace(cell)) {
            errors.push_back("Line " + std::to_string(row + 6) +
                            ": illegal character '" + std::string(1, cell) + "' ignored.");
        }
    }
}