#pragma once
#include "Point.h"
#include "GameObject.h"
namespace UserCommon_206480972_206899163 {
/**
 * @class Wall
 * @brief Represents a wall object on the game board.
 *
 * The Wall class inherits from GameObject and represents a destructible or indestructible wall
 * at a specific position on the board. It tracks its position and the number of times it has been hit.
 */
class Wall : public GameObject {
private:
    Point position;   ///< The (x, y) position of the wall on the board.
    int cnt_hits;     ///< Number of times the wall has been hit.

public:
    /**
     * @brief Constructs a Wall at the given (x, y) position.
     * @param x The x-coordinate of the wall.
     * @param y The y-coordinate of the wall.
     */
    Wall(int x, int y);

    /**
     * @brief Gets the position of the wall.
     * @return The Point representing the wall's position.
     */
    Point getPosition() const override;

    /**
     * @brief Gets the character representation of the wall for display.
     * @return The character representing the wall.
     */
    char toChar() const override;

    /**
     * @brief Gets the number of times the wall has been hit.
     * @return The hit count.
     */
    int getHitCount() const;

    /**
     * @brief Increments the wall's hit count by one.
     */
    void incrementHitCount();

    /**
     * @brief Sets the position of the wall.
     * @param new_position The new position to set.
     */
    void setPosition(const Point& new_position) override;
    
};
} //namespace UserCommon_206480972_206899163