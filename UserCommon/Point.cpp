#include "Point.h"
#include <iostream>

namespace UserCommon_206480972_206899163 {
/**
* @brief Default constructor. Initializes the point at (0, 0).
*/
Point::Point() : x(0), y(0) {}

/**
* @brief Constructor with coordinates.
* @param x The x-coordinate.
* @param y The y-coordinate.
*/
Point::Point(int x, int y) : x(x), y(y) {}

// Get the x-coordinate
int Point::getX() const {
    return x;
}

// Get the y-coordinate
int Point::getY() const { 
    return y;
}

// Equality operator: returns true if both coordinates are equal
bool Point::operator==(const Point& other) const { 
    return x == other.x && y == other.y;
}

// Inequality operator: returns true if either coordinate is different
bool Point::operator!=(const Point& other) const {
    return getX() != other.getX() || getY() != other.getY();
}

// Set the x-coordinate
void Point::setX(int x) {
    this->x = x;
}

// Set the y-coordinate
void Point::setY(int y) {
    this->y = y;
}

// Print the point's coordinates to the console - Debugging use
void Point::printPoint(const Point& point) {
    std::cout << "Point (X, Y): " << "(" << static_cast<int>(point.getX()) << ", " << static_cast<int>(point.getY()) << ")" << std::endl;
}
} // namespace UserCommon_206480972_206899163