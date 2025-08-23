#pragma once 

#include <vector>
/**
 * @struct MapData
 * @brief Holds the parsed data for a single map file.
 */
struct MapData {
    int max_steps;                            ///< Maximum number of steps allowed in the game
    int num_shells;                           ///< Number of shells available
    int rows;                                ///< Number of rows in the map
    int cols;                               ///< Number of columns in the map
    std::vector<std::vector<char>> grid;    ///< 2D vector representing the map layout

    /**
     * @brief Constructs a MapData object with the given parameters.
     */

    MapData(int max_steps, int num_shells, int rows, int cols, std::vector<std::vector<char>> grid)
        : max_steps(max_steps), num_shells(num_shells), rows(rows), cols(cols), grid(grid){}
};