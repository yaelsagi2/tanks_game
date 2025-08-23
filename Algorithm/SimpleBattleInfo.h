#pragma once
#include "../common/BattleInfo.h"
#include "../common/SatelliteView.h"
#include "../UserCommon/Wall.h"
#include "../UserCommon/Shell.h"
#include "../UserCommon/Tank.h"
#include "../UserCommon/Mine.h"
#include <vector>
#include <memory>

using namespace UserCommon_206480972_206899163;

namespace Algorithm_206480972_206899163{
/*
 * @class SimpleBattleInfo
 * @brief Stores and manages the current state of the battle for a single turn, based on the SatelliteView.
 *
 * Holds all objects (walls, mines, shells, tanks) as unique pointers, as well as a board view and a pointer to the player's own tank.
 * Provides methods for querying and modifying the state, and for printing the current battle state.
 */
class SimpleBattleInfo : public BattleInfo {
private:
    size_t rows = 0;                            ///< Number of rows in the board.
    size_t cols = 0;                            ///< Number of columns in the board.
    int ammo_count;                             ///< Ammo count for the player's tank.
    std::vector<std::unique_ptr<::Wall>> walls;   ///< All wall objects on the board.
    std::vector<std::unique_ptr<::Mine>> mines;   ///< All mine objects on the board.
    std::vector<std::unique_ptr<::Shell>> shells; ///< All shell objects on the board.
    std::vector<std::unique_ptr<::Tank>> tanks1;  ///< All player 1 tanks (excluding myTank).
    std::vector<std::unique_ptr<::Tank>> tanks2;  ///< All player 2 tanks (excluding myTank).
    std::vector<std::vector<char>> boardView;   ///< Char representation of the board (for display/debug).
    std::unique_ptr<::Tank> myTank;               ///< The player's own tank.
    int player_asked_for_info = 0;              ///< Step when the player last asked for battle info.
public:
    /**
     * @brief Default constructor.
     */
    SimpleBattleInfo() = default;

    /**
     * @brief Constructs SimpleBattleInfo from a SatelliteView and player info.
     * @param view The satellite view of the board.
     * @param x Number of columns.
     * @param y Number of rows.
     * @param ammo The player's ammo count.
     */
    SimpleBattleInfo(const SatelliteView& view, size_t x, size_t y, int ammo, int player_asked);

    /**
     * @brief Destructor.
     */
    ~SimpleBattleInfo() override = default;
    
    // Rule of 5: delete copy constructor and copy assignment
    /**
     * @brief Copy constructor (deleted).
     */
    SimpleBattleInfo(const SimpleBattleInfo& other);

    /**
     * @brief Copy assignment operator (deleted).
     */
    SimpleBattleInfo& operator=(const SimpleBattleInfo& other) = delete;

    // Move constructor and move assignment
    /**
     * @brief Move constructor.
     */
    SimpleBattleInfo(SimpleBattleInfo&&) noexcept = default;

    /**
     * @brief Move assignment operator.
     */
    SimpleBattleInfo& operator=(SimpleBattleInfo&&) noexcept = default;

    /**
     * @brief Gets the number of rows in the board.
     * @return Number of rows.
     */
    size_t getRows() const;

    /**
     * @brief Gets the number of columns in the board.
     * @return Number of columns.
     */
    size_t getCols() const;

    /**
     * @brief Gets the player's ammo count.
     * @return Ammo count.
     */
    int getAmmoCount() const;

    /**
     * @brief Gets all wall pointers.
     * @return Vector of Wall pointers.
     */
    std::vector<::Wall*> getWalls() const;

    /**
     * @brief Gets all mine pointers.
     * @return Vector of Mine pointers.
     */
    std::vector<::Mine*> getMines() const;

    /**
     * @brief Gets all shell pointers.
     * @return Vector of Shell pointers.
     */
    std::vector<::Shell*> getShells() const;

    /**
     * @brief Gets all player 1 tank pointers (excluding myTank).
     * @return Vector of Tank pointers.
     */
    std::vector<::Tank*> getTanks1() const;

    /**
     * @brief Gets all player 2 tank pointers (excluding myTank).
     * @return Vector of Tank pointers.
     */
    std::vector<::Tank*> getTanks2() const;

    /**
     * @brief Gets the board view as a 2D char vector.
     * @return Reference to the board view.
     */
    const std::vector<std::vector<char>>& getBoardView() const;

    /**
     * @brief Gets a pointer to the player's own tank.
     * @return Pointer to myTank, or nullptr if not present.
     */
    ::Tank* getMyTank() const;

    /**
     * @brief Adds a shell to the board.
     * @param shell The shell to add.
     */
    void addShell(const ::Shell& shell);

    /**
     * @brief Removes a shell from the board.
     * @param shell Pointer to the shell to remove.
     */
    void removeShell(::Shell* shell);

    /**
     * @brief Removes a wall from the board.
     * @param wall Pointer to the wall to remove.
     */
    void removeWall(::Wall* wall);

    /**
     * @brief Removes a mine from the board.
     * @param mine Pointer to the mine to remove.
     */
    void removeMine(::Mine* mine);

    /**
     * @brief Removes a tank from the board (not myTank).
     * @param tank Pointer to the tank to remove.
     */
    void removeTank(::Tank* tank);

    /**
     * @brief Removes the player's own tank from the board.
     */
    void removeMyTank();

    /**
     * @brief Prints the current battle state to the console.
     */
    void printBattleState() const;

    /**
     * @brief Checks if the battle info is initialized.
     * @return True if initialized, false otherwise.
     */
    bool isInitialized() const;

    /**
     * @brief Checks if the Object is on the board.
     * @param object Pointer to the Object to check.
     * @return True if the Object is on the board, false otherwise.
     */
    bool isObjectOnBoard(const ::GameObject* object) const;
};
} // namespace Algorithm_206480972_206899163
