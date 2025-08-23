#pragma once
#include <string>
#include <utility> 
namespace UserCommon_206480972_206899163 {

/**
 * @brief Enum class representing the 8 possible movement directions and a 'None' direction.
 * 
 * Directions:
 *  - U  : Up
 *  - UR : Up-Right
 *  - R  : Right
 *  - DR : Down-Right
 *  - D  : Down
 *  - DL : Down-Left
 *  - L  : Left
 *  - UL : Up-Left
 *  - None : No movement
 */
enum class Direction { 
    U,    ///< Up
    UR,   ///< Up-Right
    R,    ///< Right
    DR,   ///< Down-Right
    D,    ///< Down
    DL,   ///< Down-Left
    L,    ///< Left
    UL,   ///< Up-Left
    None  ///< No movement
};

/**
 * @brief Converts a Direction enum value to its string representation.
 * @param dir The direction to convert.
 * @return A string representing the direction.
 */
std::string directionToString(Direction dir); 

/**
 * @brief Returns the (dx, dy) offset for a given direction.
 * @param dir The direction.
 * @return A pair of integers representing the x and y offset.
 */
std::pair<int, int> directionOffset(Direction dir);

}