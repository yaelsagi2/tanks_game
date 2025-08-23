#include "Direction.h"
#include <iostream>
#include <string>
#include <utility> 
namespace UserCommon_206480972_206899163 {

std::string directionToString(Direction dir) {
    // Converts a Direction enum value to its string representation - use for debugging or logging
    switch (dir) {
        case Direction::U:  return "U";  // 0
        case Direction::UR: return "UR"; // 1
        case Direction::R:  return "R";  // 2
        case Direction::DR: return "DR"; // 3
        case Direction::D:  return "D";  // 4
        case Direction::DL: return "DL"; // 5
        case Direction::L:  return "L";  // 6
        case Direction::UL: return "UL"; // 7
        case Direction::None : return "None";
    }
    return "";
}

std::pair<int, int> directionOffset(Direction dir) {
    // Returns the offset (x, y) for a given Direction
    // The offset represents the change in position when moving in the specified direction
    switch (dir) {
        case Direction::D:  return {0, 1};
        case Direction::DR: return {1, 1};
        case Direction::R:  return {1, 0};
        case Direction::UR: return {1, -1};
        case Direction::U:  return {0, -1};
        case Direction::UL: return {-1, -1};
        case Direction::L:  return {-1, 0};
        case Direction::DL: return {-1, 1};
        case Direction::None: return {0,0};
    }
    return {0, 0}; // Default case
}

}
