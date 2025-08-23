#pragma once
#include <string>
#include <vector>
#include "Simulator.h"
#include "MapData.h"

/**
 * @file MapParser.h
 * @brief Provides functions and data structures for parsing tank game map files.
 *
 * This header defines the MapData structure, which holds all relevant map information,
 * and declares utility functions for reading and validating map files, extracting header values,
 * normalizing map lines, and handling individual map cells.
 */


/**
 * @brief Reads and parses a map file, returning a MapData object on success.
 * @param filename The path to the map file.
 * @param errors Vector to store any error messages encountered during parsing.
 * @return A unique pointer to MapData if successful, nullptr otherwise.
 */
MapData readMapFile(const std::string& filename, std::vector<std::string>& errors);

/**
 * @brief Trims leading and trailing whitespace from a string.
 * @param str The string to trim.
 * @return The trimmed string.
 */
std::string trim(const std::string& str);

/**
 * @brief Parses a key-value pair from a line in the map file.
 * @param line The line to parse.
 * @param key The key to look for.
 * @param errors Vector to store any error messages.
 * @param num_line The line number in the file.
 * @param isValid Reference to a bool that will be set to true if parsing succeeds.
 * @return The parsed integer value for the key.
 */
int parseKeyValue(const std::string& line, std::string key, std::vector<std::string>& errors, int num_line, bool& isValid);

/**
 * @brief Reads and validates the header line of a map file.
 * @param input_file Input file stream for the map file.
 * @param maxSteps Output parameter for max steps.
 * @param numShells Output parameter for number of shells.
 * @param rows Output parameter for number of rows.
 * @param cols Output parameter for number of columns.
 * @param errors Vector to store any error messages.
 * @param isValid Reference to a bool that will be set to true if parsing succeeds.
 * @return True if the header is read and valid, false otherwise.
 */
bool readHeadersLine(std::ifstream& input_file, int& maxSteps, int& numShells, int& rows, int& cols, std::vector<std::string>& header_errors);

/**
 * @brief Normalizes a line from the map file to the expected length.
 * @param input_file Input file stream for the map file.
 * @param expected_len The expected length of the line.
 * @param line_num The line number in the file.
 * @param errors Vector to store any error messages.
 * @return The normalized line.
 */
std::string normalizeLine(std::string line, int expected_len, int line_num, std::vector<std::string>& errors);

/**
 * @brief Handles a single cell in the map grid, updating the grid and error list as needed.
 * @param cell The character representing the cell.
 * @param row The row index of the cell.
 * @param col The column index of the cell.
 * @param grid Reference to the map grid.
 * @param errors Vector to store any error messages.
 */
void handleCell(char cell, int row, int col, std::vector<std::vector<char>>& grid, std::vector<std::string>& errors);
