#pragma once
#include <iostream>
namespace UserCommon_206480972_206899163 {
/**
 * @class Point
 * @brief Represents a point in 2D space with integer coordinates.
 *
 * The Point class provides basic operations for handling 2D coordinates,
 * including getters, setters, comparison operators, and a print utility.
 */
class Point {
private:
    int x;  ///< The x-coordinate of the point.
    int y;  ///< The y-coordinate of the point.

public:
    /**
     * @brief Default constructor. Initializes the point at (0, 0).
     */
    Point();

    /**
     * @brief Constructs a point with given x and y coordinates.
     * @param x The x-coordinate.
     * @param y The y-coordinate.
     */
    Point(int x, int y);

    /**
     * @brief Gets the x-coordinate.
     * @return The x-coordinate.
     */
    int getX() const; 

    /**
     * @brief Gets the y-coordinate.
     * @return The y-coordinate.
     */
    int getY() const;

    /**
     * @brief Sets the x-coordinate.
     * @param x The new x-coordinate.
     */
    void setX(int x); 

    /**
     * @brief Sets the y-coordinate.
     * @param y The new y-coordinate.
     */
    void setY(int y); 

    /**
     * @brief Equality comparison operator.
     * @param other The point to compare with.
     * @return True if both points have the same coordinates.
     */
    bool operator == (const Point& other) const;

    /**
     * @brief Inequality comparison operator.
     * @param other The point to compare with.
     * @return True if the points have different coordinates.
     */
    bool operator != (const Point& other) const;

    /**
     * @brief Less-than comparison operator.
     * @param other The point to compare with.
     * @return True if this point is less than the other point.
     * 
     * This operator is used for ordering points, primarily in
     * associative containers like std::map.
     */
    bool operator<(const Point& other) const {
        if (getX() != other.getX())
            return getX() < other.getX();
        return getY() < other.getY();
    }

    /**
     * @brief Prints the point's coordinates to the standard output.
     * @param point The point to print.
     */
    void printPoint(const Point& point);
};

/**
 * @brief Specialization of std::hash for Point to allow usage in unordered containers.
 */
} // namespace UserCommon_206480972_206899163

namespace std {
    template<>
    struct hash<UserCommon_206480972_206899163::Point> {
        size_t operator()(const UserCommon_206480972_206899163::Point& p) const {
            return std::hash<int>()(p.getX()) ^ (std::hash<int>()(p.getY()) << 1);
        }
    };
}