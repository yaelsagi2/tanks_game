#pragma once
#include "Point.h"
#include "GameObject.h"
namespace UserCommon_206480972_206899163 {

/**
 * @class Mine
 * @brief Class representing a mine in the game.
 *
 * A Mine has a position on the board and provides access to its properties.
 */
class Mine : public GameObject {
private:
    Point position; ///< The position of the mine on the board.

public:
    /**
     * @brief Constructs a Mine object at the specified coordinates.
     * @param x The x-coordinate of the mine.
     * @param y The y-coordinate of the mine.
     */
    Mine(int x, int y);

    /**
     * @brief Gets the position of the mine.
     * @return The position as a Point.
     */
    Point getPosition() const override;

    /**
     * @brief Gets the character representation of the mine.
     * @return The character representing the mine.
     */
    char toChar() const override;

    /**
     * @brief Sets the position of the mine.
     * @param new_position The new position to set.
     */
    void setPosition(const Point& new_position) override;

};
} // namespace UserCommon_206480972_206899163
