// ArgsParser.h
#pragma once
#include <string>
#include <vector>

/**
 * @file ArgsParser.h
 * @brief Provides argument parsing utilities for the tank game simulator.
 *
 * This header defines the ParsedArgs structure, which holds all relevant command-line arguments,
 * and declares utility functions for parsing, validating, and handling command-line arguments.
 */

struct ParsedArgs {
    enum class Mode { Comparative, Competition };
    bool verbose = false;
    int num_threads = 1;
    Mode mode;

    // comparative mode
    std::string game_map;
    std::string game_managers_folder;
    std::string algorithm1;
    std::string algorithm2;

    // competition mode
    std::string game_maps_folder;
    std::string game_manager_so;
    std::string algorithms_folder;
};

/**
 * @brief Trims leading and trailing whitespace from a string in place.
 * @param s String to trim.
 */
inline void trim_inplace(std::string& s);

/**
 * @brief Splits a key-value pair from a string argument.
 * @param arg Argument string to split.
 * @return Pair of key and value strings.
 */
std::pair<std::string, std::string> splitKeyValue(const std::string& arg);

/**
 * @brief Prints usage instructions and exits the program.
 * @param missing Vector of missing arguments.
 * @param unsupported Vector of unsupported arguments.
 */
void printUsageAndExit(const std::vector<std::string>& missing, const std::vector<std::string>& unsupported);

/**
 * @brief Parses command line arguments and returns a ParsedArgs object.
 * @param argc Number of arguments.
 * @param argv Argument vector.
 * @return ParsedArgs object with parsed values.
 */
ParsedArgs parseArgs(int argc, char* argv[]);

/**
 * @brief Parses the values from argv into args, and fills unsupported vector.
 * @param argc Number of arguments.
 * @param argv Argument vector.
 * @param args ParsedArgs object to fill.
 * @param unsupported Vector to store unsupported arguments.
 */
void parseArgsValues(int argc, char* argv[], ParsedArgs& args, std::vector<std::string>& unsupported);

/**
 * @brief Validates the parsed args and fills missing arguments.
 * @param args ParsedArgs object to validate.
 * @param missing Vector to store missing arguments.
 */
void validateArgs(const ParsedArgs& args, std::vector<std::string>& missing);