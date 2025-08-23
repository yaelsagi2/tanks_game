#pragma once
#include "Point.h"
#include "Direction.h"
#include "GameObject.h"
#include <utility> 
#include <iostream> 
namespace UserCommon_206480972_206899163 {
/**
 * @class Shell
 * @brief Class representing a shell (projectile) in the game.
 *
 * A Shell has a position, direction, owner ID, and a flag indicating if it is newly created.
 * It can move on the board and provides access to its properties.
 */
class Shell : public GameObject {
private:
    Point position;         ///< The current position of the shell on the board.
    Direction direction;    ///< The direction the shell is moving.
    int id;                 ///< The ID of the tank that fired the shell.
    bool new_shell;         ///< Flag indicating if the shell is newly created.

public:
    /**
     * @brief Constructs a shell with a position and owner ID.
     * @param position The initial position of the shell.
     * @param id The ID of the tank that fired the shell.
     */
    Shell(Point position, int id);

    /**
     * @brief Constructs a shell with a position (direction and id defaulted).
     * @param position The initial position of the shell.
     */
    Shell(Point position);

    /**
     * @brief Constructs a shell with a position, direction, and owner ID.
     * @param position The initial position of the shell.
     * @param dir The direction of the shell.
     * @param id The ID of the tank that fired the shell.
     */
    Shell(Point position, Direction dir, int id);

    // Getters

    /**
     * @brief Gets the current position of the shell.
     * @return The position as a Point.
     */
    Point getPosition() const override;

    /**
     * @brief Gets the character representation of the shell.
     * @return The character representing the shell.
     */
    char toChar() const override;

    /**
     * @brief Gets the direction of the shell.
     * @return The direction.
     */
    Direction getDirection() const; 

    /**
     * @brief Gets the ID of the tank that fired the shell.
     * @return The tank's ID.
     */
    int getId() const; 

    /**
     * @brief Returns whether the shell is newly created.
     * @return True if the shell is new, false otherwise.
     */
    bool getNewShell() const; 

    /**
     * @brief Moves the shell one step in its direction, wrapping around the board if necessary.
     * @param boardWidth The width of the board.
     * @param boardHeight The height of the board.
     */
    void move(int boardWidth, int boardHeight);

    // Setters

    /**
     * @brief Sets the position of the shell.
     * @param newPosition The new position.
     */
    void setPosition(const Point& newPosition);

    /**
     * @brief Sets the shell as a newly created shell.
     */
    void setNewShell();
};

} // namespace UserCommon_206480972_206899163
