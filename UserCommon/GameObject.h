#pragma once

#include "Point.h"
namespace UserCommon_206480972_206899163 {
/**
 * @class GameObject
 * @brief Abstract base class for all objects that can exist on the game board.
 *
 * Provides a common interface for retrieving the position and character representation
 * of any game object (such as tanks, walls, mines, shells, etc.).
 */
class GameObject {
public:
    /**
     * @brief Virtual destructor for safe polymorphic deletion.
     */
    virtual ~GameObject() = default;

    /**
     * @brief Gets the position of the object on the board.
     * @return The position as a Point.
     */
    virtual Point getPosition() const = 0;

    /**
     * @brief Gets the character representation of the object.
     * @return The character representing the object.
     */
    virtual char toChar() const = 0; 

    /**
     * @brief Sets the position of the object on the board.
     * @param new_position The new position to set.
     */
    virtual void setPosition(const Point& new_position) = 0;
};

} // namespace UserCommon_206480972_206899163