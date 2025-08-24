
#pragma once

#include "../common/Player.h"
#include "../common/SatelliteView.h"

namespace Player_206480972_206899163 {


/**
 * @class YaelNogaPlayer
 * @brief Implements a custom Player with Rule of 5 and tank info update logic.
 *
 * This class manages player state and provides logic to update tanks with battle info.
 * It implements the Rule of 5 for safe copying and moving.
 */
class YaelNogaPlayer : public Player {
public:
    /**
     * @brief Constructor
     * @param player_index Index of the player (1 or 2)
     * @param rows Number of rows in the map
     * @param cols Number of columns in the map
     * @param max_steps Maximum number of steps in the game
     * @param num_shells Number of shells per tank
     */
    YaelNogaPlayer(int player_index, size_t rows, size_t cols, size_t max_steps, size_t num_shells);

    /**
     * @brief Destructor
     */
    ~YaelNogaPlayer() override;

    /**
     * @brief Copy constructor
     */
    YaelNogaPlayer(const YaelNogaPlayer& other);

    /**
     * @brief Move constructor
     */
    YaelNogaPlayer(YaelNogaPlayer&& other) noexcept;

    /**
     * @brief Copy assignment operator
     */
    YaelNogaPlayer& operator=(const YaelNogaPlayer& other);

    /**
     * @brief Move assignment operator
     */
    YaelNogaPlayer& operator=(YaelNogaPlayer&& other) noexcept;

    /**
     * @brief Updates the tank with battle info from the satellite view.
     */
    void updateTankWithBattleInfo(TankAlgorithm& tank, ::SatelliteView& satellite_view) override;

private:
    int getSelfTankCount(const ::SatelliteView& view);
    int getOpponentTankCount(const ::SatelliteView& view);
    int getCharCount(const ::SatelliteView& view, char ch);

    int player_index;
    size_t rows, cols, max_steps, num_shells;
};

} // namespace Player_206480972_206899163