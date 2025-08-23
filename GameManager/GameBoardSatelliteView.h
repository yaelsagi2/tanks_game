#pragma once

#include "../common/SatelliteView.h"
#include "../UserCommon/Tank.h"
#include <vector>
#include <memory>

using namespace UserCommon_206480972_206899163;

namespace GameManager_206480972_206899163 {

class GameBoard;

/**
 * @brief Provides a satellite view of the game board for a specific tank.
 *        Allows querying the board state and printing a debug view.
 */
class GameBoardSatelliteView : public SatelliteView {
public:
    /**
     * @brief Constructs a satellite view for the given board and tank.
     * @param board Reference to the game board.
     * @param selfTank Pointer to the player's own tank.
     */
    GameBoardSatelliteView(GameBoard* board, Tank* selfTank);

    /**
     * @brief Returns a character representing the object at the specified coordinates.
     * @param x The x-coordinate.
     * @param y The y-coordinate.
     * @return Character representing the object at (x, y).
     */
    char getObjectAt(size_t x, size_t y) const override;

    /**
     * @brief returns the grid as a string
     */
    std::string getView() const;

private:
    GameBoard* board;   ///< pointer to the game board
    Tank* selfTank;     ///< Pointer to the player's own tank
};
}
