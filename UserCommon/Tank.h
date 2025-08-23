#pragma once

#include <iostream>
#include "Point.h"
#include "Direction.h" 
#include "GameObject.h"
#include "Shell.h"
#include <vector>

namespace UserCommon_206480972_206899163 {
/**
 * @class Tank
 * @brief Represents a tank in the game, with position, direction, ammo, and actions.
 */
class Tank : public GameObject {
private:
    Point position;                ///< The position of the tank on the board.
    int id;                        ///< The local index of the tank of a player (1 or 2).   
    Direction canon_dir;           ///< The direction the tank's cannon is facing.
    int player_index;              ///< The player ID (1 or 2) for this tank.
    int shooting_cooldown;         ///< Cooldown time before the tank can shoot again.
    int ammo_count;                ///< Number of shells/ammo the tank has.
    int backward_steps;            ///< Number of backward steps taken.
    bool is_alive = true;          ///< Flag indicating if the tank is alive.
    std::vector<Point> future_steps; ///< Predicted future steps for the tank.

public:
    /**
     * @brief Constructs a Tank object.
     * @param x The x-coordinate of the tank.
     * @param y The y-coordinate of the tank.
     * @param id The player ID (1 or 2).
     * @param ammo_count The initial ammo count.
     * @param player_index The index of the player (1 or 2).
     */
    Tank(int x, int y, int id, int player_index,int ammo_count);

    // Getters

    /**
     * @brief Gets the position of the tank.
     * @return The current position as a Point.
     */
    Point getPosition() const override;

    /**
     * @brief Sets the position of the tank.
     * @param new_position The new position to set.
     */
    void setPosition(const Point& new_position) override;
    /**
     * @brief Gets the character representation of the tank.
     * @return The character representing the tank.
     */
    char toChar() const override;

    /**
     * @brief Gets the player ID of the tank.
     * @return The tank's player ID.
     */
    int getId() const; 

    /**
     * @brief Gets the direction of the tank's cannon.
     * @return The current cannon direction.
     */
    Direction getCanonDir() const;

    /**
     * @brief Gets the shooting cooldown.
     * @return The current shooting cooldown value.
     */
    int getShootingCooldown() const; 

    /**
     * @brief Gets the current ammo count.
     * @return The number of shells/ammo.
     */
    int getAmmoCount() const;

    /**
     * @brief Gets the number of backward steps taken.
     * @return The backward steps count.
     */
    int getBackwardSteps() const; 

    /**
     * @brief Increases the backward steps count, up to a maximum of 3.
     */
    void IncreaseBackwardSteps(); 

    /**
     * @brief Gets the tank's predicted future steps.
     * @return A const reference to the vector of future steps.
     */
    const std::vector<Point>& getFutureSteps() const; 

    /**
     * @brief Returns whether the tank is alive.
     * @return True if alive, false otherwise.
     */
    bool getIsAlive() const;

    /**
     * @brief Gets the player index of the tank.
     * @return The player index (1 or 2).
     */
    int getPlayerIndex() const;

    // Setters

    /**
     * @brief Sets the tank as alive.
     */
    void setAlive();

    /**
     * @brief Sets the shooting cooldown.
     * @param cooldown The new cooldown value.
     */
    void setShootingCooldown(int cooldown); 

    /**
     * @brief Sets the ammo count.
     * @param ammo_count The new ammo count.
     */
    void setAmmoCount(int ammo_count); 

    /**
     * @brief Sets the tank's future steps.
     * @param steps The new vector of future steps.
     */
    void setFutureSteps(const std::vector<Point>& steps); 

    /**
     * @brief Sets the number of backward steps.
     * @param steps The new backward steps count.
     */
    void setBackwardSteps(int steps);

    /**
     * @brief Sets the tank's player ID.
     * @param id The new player ID.
     */
    void setID(int id); 

    // Other methods

    /**
     * @brief Modifies the shooting cooldown.
     */
    void cooldownModify(); 

    // Tank Actions

    /**
     * @brief Moves the tank forward in the direction it is facing.
     * @param boardWidth The width of the board.
     * @param boardHeight The height of the board.
     */
    void moveForward(int boardWidth, int boardHeight); 

    /**
     * @brief Moves the tank backward.
     * @param boardWidth The width of the board.
     * @param boardHeight The height of the board.
     */
    void moveBackward(int boardWidth, int boardHeight); 

    /**
     * @brief Rotates the tank left by a specified amount.
     * @param rotationAmount The amount to rotate (in 45-degree increments).
     */
    void rotateLeft(int rotationAmount);

    /**
     * @brief Rotates the tank right by a specified amount.
     * @param rotationAmount The amount to rotate (in 45-degree increments).
     */
    void rotateRight(int rotationAmount); 

    /**
     * @brief Shoots a shell in the direction the tank is facing.
     * @param boardWidth The width of the board.
     * @param boardHeight The height of the board.
     * @return The created Shell object.
     */
    Shell shoot(int boardWidth, int boardHeight); 

    /**
     * @brief Checks if the tank can shoot.
     * @return True if the tank can shoot, false otherwise.
     */
    bool canShoot() const; 

    /**
     * @brief Decrements the shooting cooldown.
     */
    void decrementCooldown(); 

    /**
     * @brief Prints the tank's future steps.
     * @param tank Pointer to the tank whose steps to print.
     */
    void printFutureSteps(const Tank* tank); 
};
}
