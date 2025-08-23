#pragma once
#include <memory>
#include "MatrixRecorder.h"
#include "../common/AbstractGameManager.h"
#include "../common/SatelliteView.h"
#include "../common/Player.h"
#include "../common/TankAlgorithm.h"
#include "../common/GameResult.h"
#include "GameBoard.h"
#include "Logger.h"
#include <list>
#include <vector>
#include <string>
using namespace UserCommon_206480972_206899163;

namespace GameManager_206480972_206899163 {

/**
 * @brief Structure holding data for each tank in the game.
 */
struct TankData
{
    std::unique_ptr<TankAlgorithm> algorithm; ///< The tank's AI algorithm
    int playerId;                            ///< The player ID owning this tank
    Tank *tank;                              ///< Pointer to the tank object
};

/**
 * @brief Manages the overall game flow, state, and logic.
 * This class is responsible for running the game, processing player actions,
 * and determining the game result.
 */
class GameManager : public AbstractGameManager {
private:
    std::unique_ptr<GameBoard> board; ///< The game board
    std::unique_ptr<MatrixRecorder> recorder; ///< Recorder for the game matrix
    std::unique_ptr<PlayerFactory> playerFactory; ///< Factory for creating players
    std::unique_ptr<TankAlgorithmFactory> tankFactory; ///< Factory for creating tank algorithms
    std::vector<TankData> tanks; ///< All tanks in the game
    std::vector<Player*> players; ///< All players in the game
    int remaining_step_after_amo = -1; ///< Steps remaining after ammo runs out
    int current_step = 1; ///< Current step number
    bool game_over = false; ///< Whether the game is over
    bool verbose; ///< Whether to log detailed information
    Logger logger; ///< Logger for game events

public:
    /**
     * @brief Constructs a GameManager.
     * @param verbose Whether to enable detailed logging.
     */
    GameManager(bool verbose);

    /**
     * @brief Runs the game with the given parameters.
     * @return The result of the game.
     */
    GameResult run(size_t map_width, size_t map_height, const SatelliteView& map,
        std::string map_name, size_t max_steps, size_t num_shells, Player& player1, std::string name1,
        Player& player2, std::string name2,
        TankAlgorithmFactory player1_tank_algo_factory, TankAlgorithmFactory player2_tank_algo_factory) override;

    // Rule of 5
    GameManager(const GameManager&) = delete;
    GameManager& operator=(const GameManager&) = delete;
    GameManager(GameManager&&) noexcept = default;
    GameManager& operator=(GameManager&&) noexcept = default;
    ~GameManager() = default;

    /**
     * @brief Prepares the logger for the game.
     * @param map_name The name of the map.
     * @param name1 Name of player 1.
     * @param name2 Name of player 2.
     */
    void prepareLogger(std::string map_name, std::string name1, std::string name2);

    /**
     * @brief Enables recording of the game matrix.
     * @param file_path Path to the output file.
     * @param rows Number of rows in the board.
     * @param cols Number of columns in the board.
     */
    void enableRecording(const std::string& file_path, std::size_t rows, std::size_t cols);

    /**
     * @brief Disables recording of the game matrix.
     */
    void disableRecording();

private:
    /**
     * @brief Initializes all tanks in sorted order.
     */
    void initAllTanksSorted(TankAlgorithmFactory player1_tank_algo_factory, TankAlgorithmFactory player2_tank_algo_factory);

    /**
     * @brief Gathers action requests from all tanks.
     * @return Vector of tank data and their requested actions.
     */
    std::vector<std::pair<TankData*, ActionRequest>> gatherRequests();

    /**
     * @brief Processes the gathered action requests.
     * @param actions The actions to process.
     * @return List of tuples containing tank data, action, and legality.
     */
    std::list<std::tuple<TankData*, ActionRequest, bool>> processRequests(const std::vector<std::pair<TankData*, ActionRequest>>& actions);

    /**
     * @brief Checks if an action is legal for a given tank.
     * @param tank The tank to check.
     * @param action The action to check.
     * @return True if the action is legal.
     */
    bool isActionLegal(Tank* tank, ActionRequest action) const;

    /**
     * @brief Gets the next position for a tank given a direction and number of steps.
     * @param pos The current position.
     * @param dir The direction to move.
     * @param steps Number of steps to move.
     * @return The next position.
     */
    Point getNextPosition(const Point& pos, Direction dir, int steps) const;

    /**
     * @brief Executes the given actions for all tanks.
     * @param actions The actions to execute.
     */
    void executeRequests(const std::list<std::tuple<TankData*, ActionRequest, bool>>& actions);

    /**
     * @brief Resets the game state to the initial state.
     */
    void resetGameState();

    /**
     * @brief Builds an immediate game result (e.g., for early termination).
     * @param p1_tanks Player 1's tanks.
     * @param p2_tanks Player 2's tanks.
     * @return The immediate game result.
     */
    GameResult buildImmediateResult(const std::vector<Tank*>& p1_tanks, const std::vector<Tank*>& p2_tanks) const;

    /**
     * @brief Checks if the game should end immediately.
     * @param p1_tanks Player 1's tanks.
     * @param p2_tanks Player 2's tanks.
     * @return True if the game should end.
     */
    bool checkImmediateEnd(const std::vector<Tank*>& p1_tanks, const std::vector<Tank*>& p2_tanks);

    /**
     * @brief Handles immediate lose or tie conditions.
     * @param p1_tanks Player 1's tanks.
     * @param p2_tanks Player 2's tanks.
     */
    void immediateLoseOrTie(const std::vector<Tank*>& p1_tanks, const std::vector<Tank*>& p2_tanks);

    /**
     * @brief Runs the main game loop.
     * @return The result of the game.
     */
    GameResult runGameLoop();

    /**
     * @brief Logs the initial positions of all tanks.
     */
    void logInitialPositions();

    /**
     * @brief Executes a single action for a tank.
     * @param td The tank data.
     * @param action The action to execute.
     */
    void executeAction(TankData* td, const ActionRequest& action);

    /**
     * @brief Executes a move forward action for a tank.
     * @param td The tank data.
     */
    void executeMoveForward(TankData* td);

    /**
     * @brief Executes a move backward action for a tank.
     * @param td The tank data.
     */
    void executeMoveBackward(TankData* td);

    /**
     * @brief Executes a rotate left action for a tank.
     * @param td The tank data.
     * @param angle The angle to rotate.
     */
    void executeRotateLeft(TankData* td, int angle);

    /**
     * @brief Executes a rotate right action for a tank.
     * @param td The tank data.
     * @param angle The angle to rotate.
     */
    void executeRotateRight(TankData* td, int angle);

    /**
     * @brief Executes a shoot action for a tank.
     * @param td The tank data.
     */
    void executeShoot(TankData* td);

    /**
     * @brief Executes a get battle info action for a tank.
     * @param td The tank data.
     */
    void executeGetBattleInfo(TankData* td);

    /**
     * @brief Counts the number of alive tanks for a player.
     * @param playerId The player ID.
     * @return The number of alive tanks.
     */
    int countAliveTanks(int playerId);

    /**
     * @brief Consolidates actions after execution.
     * @param actions The actions to consolidate.
     */
    void consolidateActions(const std::list<std::tuple<TankData*, ActionRequest, bool>>& actions);

    /**
     * @brief Checks for all types of collisions.
     */
    void checkCollisions();

    /**
     * @brief Checks for future shell collisions at a given offset.
     * @param square The offset to check.
     */
    void checkShellFutureCollisions(int square);

    /**
     * @brief Checks for shell-shell collisions.
     */
    void checkShellShellCollisions();

    /**
     * @brief Checks for shell-tank collisions.
     */
    void checkShellTankCollisions();

    /**
     * @brief Checks for tank-tank collisions.
     */
    void checkTankTankCollision();

    /**
     * @brief Checks for tank-wall collisions.
     */
    void checkTankWallCollisions();

    /**
     * @brief Checks for shell-wall collisions.
     */
    void checkShellWallCollisions();

    /**
     * @brief Checks for tank-mine collisions.
     */
    void checkTankMineCollisions();

    /**
     * @brief Updates the location of all shells.
     */
    void updateShellsLocation();

    /**
     * @brief Checks if at least one tank is alive for a player.
     * @param playerId The player ID.
     * @return True if at least one tank is alive.
     */
    bool isAtLeastOneTankAlive(int playerId) const;

    /**
     * @brief Checks if the game is over.
     * @return True if the game is over.
     */
    bool isGameOver();

    /**
     * @brief Gets the current game result.
     * @return The game result.
     */
    GameResult getGameResult();

    /**
     * @brief Checks if all tanks are out of ammo.
     * @return True if all tanks are out of ammo.
     */
    bool allTanksOutOfAmmo();

    /**
     * @brief Updates the game status.
     */
    void updateGameStatus();

    /**
     * @brief Gathers requests from all tanks.
     * @param actions The vector to fill with requests.
     */
    void gatherRequests(std::vector<std::pair<TankData*, ActionRequest>>& actions);

    /**
     * @brief Moves all shells two points forward.
     */
    void moveShellTwoPoints();

    /**
     * @brief Removes all items that have collided.
     * @param shells_to_remove Shells to remove.
     * @param tanks_to_remove Tanks to remove.
     * @param walls_to_remove Walls to remove.
     */
    void removeCollidedItems(const std::vector<Shell*>& shells_to_remove,  const std::vector<Tank*>& tanks_to_remove, const std::vector<Wall*>& walls_to_remove);

    /**
     * @brief Returns a short name for an action.
     * @param req The action request.
     * @return The short name.
     */
    std::string shortActionName(const ActionRequest& req) const;

    /**
     * @brief Gets the reason for ignoring an action.
     * @param tank The tank.
     * @param action The action.
     * @return The reason string.
     */
    std::string getIgnoreReason(Tank* tank, const ActionRequest& action);

    /**
     * @brief Records the current frame if needed.
     */
    void recordFrameIfNeeded() noexcept;

    /**
     * @brief Sets the fields of the game result.
     * @param result The game result to set.
     * @param result_str The result string to set.
     */
    void setGameResultFields(GameResult& result, std::string& result_str);

    /**
     * @brief Converts the board to an ASCII grid.
     * @param board The game board.
     * @return The ASCII grid.
     */
    static std::vector<std::string> toAsciiGridFromBoard(GameBoard* board);
    // Helper function to log dead tanks
    void logDeadTanks(const std::vector<int>& dead_tanks, int& i, int size_tanks);

};

} // namespace GameManager_206480972_206899163