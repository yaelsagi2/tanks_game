#include "Mine.h"
namespace UserCommon_206480972_206899163 {
// Constructor
Mine::Mine(int x, int y) : position(x, y) {}

// Getter
Point Mine::getPosition() const {
    return position;
}

char Mine::toChar() const {
    // This function returns the character representation of the mine
    return '@'; 
}

void Mine::setPosition(const Point& new_position) {
    position = new_position;
}

} // namespace UserCommon_206480972_206899163