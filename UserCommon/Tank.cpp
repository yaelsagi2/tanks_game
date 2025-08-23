#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include "Tank.h"
#include "Shell.h"
#include "Point.h"
#include "Direction.h"

namespace UserCommon_206480972_206899163 {
// Constructor 
Tank::Tank(int x, int y, int id, int player_index, int ammo_count)
    : position(x, y),
      id(id),
      canon_dir(player_index == 1 ? Direction::L : Direction::R),
      player_index(player_index),
      shooting_cooldown(0),
      ammo_count(ammo_count), 
      backward_steps(0),
      is_alive(true),
      future_steps() {}

// Getters
Point Tank::getPosition() const {
    // Return the current position of the tank
    return position;
}

int Tank::getId() const {
    // return the local index of the tank of a player (1 or 2)
    return id;
}

Direction Tank::getCanonDir() const {
    // Return the current direction of the tank's cannon
    return canon_dir;
}

int Tank::getShootingCooldown() const {
    // Return the current shooting cooldown of the tank
    return shooting_cooldown;
}

int Tank::getAmmoCount() const {
    // Return the current ammo count of the tank
    return ammo_count;
}

int Tank::getBackwardSteps() const {
    // Return the number of backward steps taken by the tank
    return backward_steps;
}

int Tank::getPlayerIndex() const {
    // Return the player index of the tank
    return player_index;
}

const std::vector<Point>& Tank::getFutureSteps() const {
    // Return the vector of future steps for the tank
    return future_steps;
}

char Tank::toChar() const {
    // Return the character representation of the tank (1 or 2)
    return player_index == 1 ? '1' : '2';
}

// Setters
void Tank::setID(int id) {
    // Set the ID of the tank
    this->id = id;
}

void Tank::setFutureSteps(const std::vector<Point>& steps) {
    // Set the future steps for the tank
    future_steps = steps;
}

void Tank::IncreaseBackwardSteps() {
    // Increases the backward steps count, up to a maximum of 3
    if (backward_steps < 3) {
        backward_steps++;
    }
    else {
        backward_steps = 0; 
    }
}

void Tank::cooldownModify() {
    // Decreases the shooting cooldown if it's greater than 0 and less than 5
    if (this->shooting_cooldown < 5 && this->shooting_cooldown > 0) {
        this->shooting_cooldown--;
    } 
}

void Tank::setBackwardSteps(int steps) {
    // Sets the backward steps count, ensuring it does not exceed 3
    if (steps >= 0 && steps <= 3) {
        backward_steps = steps;
    } else {
        backward_steps = 0; // Reset to 0 if invalid input
    }
}

void Tank::setAmmoCount(int ammo_count) {
    // Set the ammo count for the tank
    this->ammo_count = ammo_count;
}

void Tank::setShootingCooldown(int cooldown) {
    // Set the shooting cooldown for the tank
    shooting_cooldown = cooldown;
}

// Moving methods
void Tank::rotateLeft(int rotationAmount){
    // Rotate the canon direction to the left - 1 for 1/8 , 2 for 1/4
    int newDirection = (static_cast<int>(canon_dir) - rotationAmount + 8) % 8;
    canon_dir = static_cast<Direction>(newDirection);
}

void Tank::rotateRight(int rotationAmount){
    // Rotate the canon direction to the right - 1 for 1/8 , 2 for 1/4
    int newDirection = (static_cast<int>(canon_dir) + rotationAmount) % 8;
    canon_dir = static_cast<Direction>(newDirection);
}

Shell Tank::shoot(int boardWidth, int boardHeight) {
    // Create a Shell object at the current position of the tank, in the direction of the cannon
    std::pair<int, int> offset = directionOffset(canon_dir);
    int newX = (position.getX() + offset.first + boardHeight) % boardHeight;
    int newY = (position.getY() + offset.second + boardWidth) % boardWidth;

    // Apply shooting cooldown and decrement ammo count
    ammo_count--;
    shooting_cooldown = 5;
    return Shell(Point(newX, newY), canon_dir, id);
}

void Tank::moveForward(int boardWidth, int boardHeight) {
    // Move the tank forward in the direction it is facing
    std::pair<int, int> offset = directionOffset(canon_dir); 
    int dx = offset.first;
    int dy = offset.second;

    int newX = (position.getX() + dx + boardHeight) % boardHeight;
    int newY = (position.getY() + dy + boardWidth) % boardWidth;
    position = Point(newX, newY);
}

void Tank::moveBackward(int boardWidth, int boardHeight) {
    // Move the tank backward in the direction it is facing 
    std::pair<int, int> offset = directionOffset(canon_dir);
    int dx = -offset.first;
    int dy = -offset.second;

    int newX = (position.getX() + dx + boardHeight) % boardHeight;
    int newY = (position.getY() + dy + boardWidth) % boardWidth;

    position = Point(newX, newY);
}

void Tank::decrementCooldown() {
    // Decrement the shooting cooldown if greater than 0
    if (shooting_cooldown > 0)
        shooting_cooldown--;
}

bool Tank::canShoot() const {
    // Check if the tank can shoot (ammo count and cooldown)
    return ammo_count > 0 && shooting_cooldown == 0;
}

void Tank::printFutureSteps(const Tank* tank) { 
    // Print future steps for a tank
    const std::vector<Point>& futureSteps = tank->getFutureSteps();
    std::cout << "Future Steps for Tank:" << std::endl;
    for (const Point& step : futureSteps) {
        std::cout << "Step -> x: " << step.getX() << ", y: " << step.getY() << std::endl;
    }
}

void Tank::setAlive() {
    // Set the tank as not alive (should be setDead? but matches header)
    is_alive = false;
}

bool Tank::getIsAlive() const {
    // Returns whether the tank is alive
    return is_alive;
}

void Tank::setPosition(const Point& new_position) {
    // Set the tank's position
    position = new_position;
}
} // namespace UserCommon_206480972_206899163