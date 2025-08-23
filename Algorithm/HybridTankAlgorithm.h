#pragma once
#include <vector> // for std::vector
#include "../common/BattleInfo.h"
#include "../common/TankAlgorithm.h"
#include "../common/ActionRequest.h"
#include "SimpleBattleInfo.h"

using namespace UserCommon_206480972_206899163;

namespace Algorithm_206480972_206899163 {
    /**
     * @class HybridTankAlgorithm
     * @brief Implements a hybrid tank control algorithm with threat avoidance and path planning.
     *
     * This class combines threat detection, path planning, and action selection for a tank.
     * It uses battle info to make decisions, plans ahead, and can avoid threats such as shells.
     * Configuration parameters allow tuning of planning and threat response.
     */
    class HybridTankAlgorithm : public TankAlgorithm {
    private:
        // configuration parameters

        int player_index; ///< The index of the player controlling this tank.
        int tank_index;   ///< The index of the tank for the player.
        int recalculate_interval; ///< Interval for recalculating the path.
        int shell_threat_radius;  ///< Distance to consider a shell as a threat.
        SimpleBattleInfo battle_info;  ///< Battle information for the tank.
        int ask_for_info_interval;    ///< Interval for asking for battle info.
        int current_step = 1;             ///< Current step in the game.

        // internal state
        std::vector<Point> future_steps; ///< Predicted future steps for the tank.

    public:
        /**
         * @brief Constructs a HybridTankAlgorithm with configuration parameters.
         * @param player_index The player index.
         * @param tank_index The tank index for the player.
         * @param interval Path recalculation interval.
         * @param threat_radius Shell threat detection radius.
         * @param max_steps Maximum planned steps ahead.
         */
        HybridTankAlgorithm(int player_index, int tank_index);

        // Rule of 5
        HybridTankAlgorithm(const HybridTankAlgorithm&) = delete;
        HybridTankAlgorithm& operator=(const HybridTankAlgorithm&) = delete;
        HybridTankAlgorithm(HybridTankAlgorithm&&) noexcept = default;
        HybridTankAlgorithm& operator=(HybridTankAlgorithm&&) noexcept = default;
        ~HybridTankAlgorithm() override = default;

        /**
         * @brief Updates the internal battle info with the latest state.
         * @param info The new battle info.
         */
        void updateBattleInfo(BattleInfo& info) override;

        /**
         * @brief Gets the next action for the tank based on current strategy.
         * @return The next ActionRequest.
         */
        ActionRequest getAction() override;

        /**
         * @brief Gets the next avoid action for the given tank.
         * @param tank The tank to avoid threats for.
         * @return The next ActionRequest to avoid threats.
         */
        ActionRequest getNextAvoidAction(const Tank* tank);

        /**
         * @brief Finds the best escape direction for the tank.
         * @param tank The tank to escape with.
         * @return The best Direction to escape.
         */
        Direction findBestEscapeDirection(const Tank* tank) const;

        /**
         * @brief Checks if a position is valid on the board.
         * @param pos The position to check.
         * @return True if valid, false otherwise.
         */
        bool isPositionValid(const Point& pos) const;

        /**
         * @brief Checks if a position is safe from threats.
         * @param pos The position to check.
         * @return True if safe, false otherwise.
         */
        bool isPositionSafe(const Point& pos) const;

        /**
         * @brief Checks if a shell is close to the given position.
         * @param pos The position to check.
         * @return True if a shell is close, false otherwise.
         */
        bool isShellClose(const Point* pos) const;

        /**
         * @brief Calculates the Euclidean distance between two points.
         * @param a First point.
         * @param b Second point.
         * @return The Euclidean distance.
         */
        double euclideanDistance(const Point& a, const Point& b) const;

        /**
         * @brief Gets all tanks for a given player index.
         * @param player_index The player index.
         * @return Vector of Tank pointers.
         */
        std::vector<Tank*> getPlayerTanks(int player_index) const;

        /**
         * @brief Gets the next chase action for the given tank.
         * @param my_tank The tank to chase with.
         * @return The next ActionRequest to chase an enemy.
         */
        ActionRequest getNextChaseAction(Tank* my_tank);

        /**
         * @brief Finds a path of steps to the enemy tank.
         * @param tank The tank to plan for.
         * @param enemy_tank The enemy tank to chase.
         */
        void findPathStepsToEnemy(Tank* tank, Tank* enemy_tank);

        /**
         * @brief Checks if a point is in a vector of points.
         * @param vec The vector of points.
         * @param point The point to check.
         * @return True if found, false otherwise.
         */
        bool isPointInVector(const std::vector<Point>& vec, const Point& point) const;

        /**
         * @brief Finds the parent point in a vector of point pairs.
         * @param vec The vector of (child, parent) pairs.
         * @param p The child point.
         * @return The parent point.
         */
        Point findParent(const std::vector<std::pair<Point, Point>>& vec, const Point& p) const;

        /**
         * @brief Checks if the tank is in shooting range of an enemy.
         * @param tank The tank to check.
         * @return True if in range, false otherwise.
         */
        bool isInShootingRange(const Tank* tank) const;

        /**
         * @brief Checks if there is a wall at the given position.
         * @param pos The position to check.
         * @return True if there is a wall, false otherwise.
         */
        bool isThereWall(const Point& pos) const;

        /**
         * @brief Finds the closest tank to a given point from a list of tanks.
         * @param from The starting point.
         * @param tanks The list of tanks.
         * @return Pointer to the closest tank.
         */
        Tank* findClosestTank(const Point& from, const std::vector<Tank*>& tanks) const;

        /**
         * @brief Calculates the direction from one point to another.
         * @param from The starting point.
         * @param to The destination point.
         * @return The direction from 'from' to 'to'.
         */
        Direction directionTo(const Point& from, const Point& to) const;

        /**
         * @brief Calculates the best direction between two tanks.
         * @param tank1 The first tank.
         * @param tank2 The second tank.
         * @return The best direction.
         */
        Direction calculateBestDirection(const Tank* tank1, const Tank* tank2) const;

    private:
        // internal strategy methods

        /**
         * @brief Gets the next action to avoid threats.
         * @return The next ActionRequest to avoid threats.
         */
        ActionRequest getAvoidAction();

        /**
         * @brief Gets the next action to chase an enemy.
         * @return The next ActionRequest to chase an enemy.
         */
        ActionRequest getChaseAction();

        /**
         * @brief Gets the next action to explore the board.
         * @return The next ActionRequest to explore.
         */
        ActionRequest getExploreAction();

        /**
         * @brief Checks if the tank is currently threatened.
         * @return True if threatened, false otherwise.
         */
        bool isThreatened() const;

        /**
         * @brief Checks if the tank can see an enemy.
         * @return True if an enemy is visible, false otherwise.
         */
        bool canSeeEnemy() const;

        /**
         * @brief Checks if the path should be recalculated.
         * @return True if recalculation is needed, false otherwise.
         */
        bool shouldRecalculatePath() const;

        /**
         * @brief Calculates a path to the enemy tank.
         * @return Vector of Points representing the path.
         */
        std::vector<Point> calculatePathToEnemy();

        /**
         * @brief Updates the internal state after an action request.
         * @param req The action request performed.
         */
        void updateStateAfterReq (ActionRequest req);
    };
}


