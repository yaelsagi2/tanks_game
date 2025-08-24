#include "HybridTankAlgorithm.h"
#include "../common/ActionRequest.h"
#include <cmath>
#include <queue>
#include <vector>

using namespace UserCommon_206480972_206899163;

#define INTERVAL        10
#define THREAT_RADIUS   20
#define INFO_INTERVAL   5

namespace Algorithm_206480972_206899163 {
    // Constructor: Initializes the HybridTankAlgorithm with configuration parameters
    HybridTankAlgorithm::HybridTankAlgorithm(int player_index, int tank_index)
        : player_index(player_index),
          tank_index(tank_index),
          recalculate_interval(INTERVAL),
          shell_threat_radius(THREAT_RADIUS),
          ask_for_info_interval(INFO_INTERVAL) {}

    int HybridTankAlgorithm::getIndex() {
        return tank_index;
    }

    void HybridTankAlgorithm::setDefensive() {
        m_defensive = true;
        m_offensive = false;
    }

    void HybridTankAlgorithm::setOffensive() {
        m_offensive = true;
        m_defensive = false;
    }

    // Updates the internal state of the algorithm based on the provided battle information
    void HybridTankAlgorithm::updateBattleInfo(BattleInfo &info)
    {
        auto *actual = dynamic_cast<SimpleBattleInfo *>(&info);
        if (actual == nullptr)
        {
            std::cerr << "[ERROR] HybridTankAlgorithm: expected SimpleBattleInfo but got unknown type.\n";
            return;
        }
        // Make a deep copy
        this->battle_info = SimpleBattleInfo(*actual);
    }

    // Simulates the effect of an action request on the internal battle_info state
    void HybridTankAlgorithm::updateStateAfterReq(ActionRequest req)
    {
        bool do_cool_down = true; // Flag to indicate if cooldown should be applied
        if (!this->battle_info.isObjectOnBoard(battle_info.getMyTank())) // No action if my tank is not set
        { return; }
        Tank *my_tank = battle_info.getMyTank();
        int cols = battle_info.getCols();
        int rows = battle_info.getRows();
        switch (req) {
            case ActionRequest::Shoot:
                battle_info.addShell(my_tank->shoot(cols, rows));
                my_tank->setAmmoCount(my_tank->getAmmoCount() - 1); // Decrease ammo count after shooting
                do_cool_down = false; // No cooldown after shooting
                break;
            case ActionRequest::MoveBackward:
                if (my_tank->getBackwardSteps() < 3) {
                    my_tank->IncreaseBackwardSteps(); // Increase backward steps count
                }
                else {
                    my_tank->moveBackward(cols, rows); // If limit reached, just move backward
                    my_tank->setBackwardSteps(0); // Reset backward steps if limit reached
                }
                break;
            case ActionRequest::MoveForward:
                if(my_tank->getBackwardSteps() > 0) {
                    my_tank->setBackwardSteps(0); // Reset backward steps if moving forward
                }
                else {my_tank->moveForward(cols, rows); }
                break;
            case ActionRequest::RotateLeft45:
                my_tank->rotateLeft(1);
                break;
            case ActionRequest::RotateLeft90:
                my_tank->rotateLeft(2);
                break;
            case ActionRequest::RotateRight45:
                my_tank->rotateRight(1);
                break;
            case ActionRequest::RotateRight90:
                my_tank->rotateRight(2);
                break;
            case ActionRequest::DoNothing:
                break;
            case ActionRequest::GetBattleInfo:
                if(my_tank->getBackwardSteps() > 0) {
                    my_tank->setBackwardSteps(0); // Reset backward steps if moving forward
                }
                break;
            default:
                break;
        }
        // Update the current step after processing the request
        if (do_cool_down) {
            battle_info.getMyTank()->cooldownModify(); // Apply cooldown after action
        }
        current_step++;
    }

    // Main decision function: determines the next action for the tank
    ActionRequest HybridTankAlgorithm::getAction()
    {
        // Determine the action based on the current state of the tank and the game board
        ActionRequest req;
        if (!battle_info.isInitialized())
        {
            req = ActionRequest::GetBattleInfo;
            updateStateAfterReq(req);
            return req; // Request battle info
        }
        Tank *tank = battle_info.getMyTank();

        if(!battle_info.isObjectOnBoard(tank)) {
            return ActionRequest::DoNothing; // No action if tank is not on the board
        }
        if (current_step % ask_for_info_interval == 0)
        {
            req = ActionRequest::GetBattleInfo;
            updateStateAfterReq(req);
            return req; // Request battle info
        }
        else
        {
            Point pos = tank->getPosition();
            if (isShellClose(&pos))
            {
                req = getNextAvoidAction(tank); // Avoid Algorithm
            }
            else
            {
                // if( m_defensive) { req = ActionRequest::Shoot; }
                // else {
                     req = getNextChaseAction(tank); }
            }
            updateStateAfterReq(req);
            return req;
        }

    ActionRequest HybridTankAlgorithm::getNextAvoidAction(const Tank *tank)
    {
        // this function returns the next action for the tank
        Point pos = tank->getPosition();
        Direction escape_dir = findBestEscapeDirection(tank);
        // Rotate towards escape direction if needed
        if (tank->getCanonDir() != escape_dir)
        {
            int current_dir = static_cast<int>(tank->getCanonDir());
            int target_dir = static_cast<int>(escape_dir);
            int diff = (target_dir - current_dir + 8) % 8;
            if (diff == 1 || diff == -7)
                return ActionRequest::RotateRight45;
            if (diff == 2 || diff == 3 || diff == 4 || diff == -4 || diff == -5 || diff == -6)
                return ActionRequest::RotateRight90;
            if (diff == 6 || diff == 5 || diff == -2 || diff == -3)
                return ActionRequest::RotateLeft90;
            if (diff == 7 || diff == -1)
                return ActionRequest::RotateLeft45;
        }

        // if facing the right direction - move
        std::pair<int, int> offset = directionOffset(tank->getCanonDir());
        int cols = battle_info.getCols();
        int rows = battle_info.getRows();
        Point forward_pos((pos.getX() + offset.first + cols) % cols,
                          (pos.getY() + offset.second + rows) % rows);

        // Calculate the position we would reach if we move in escape direction
        std::pair<int, int> escape_offset = directionOffset(escape_dir);
        Point escape_pos((pos.getX() + escape_offset.first + cols) % cols,
                         (pos.getY() + escape_offset.second + rows) % rows);

        if (forward_pos == escape_pos)
        {
            return ActionRequest::MoveForward;
        }
        else
        {
            return ActionRequest::MoveBackward;
        }
    }

    Direction HybridTankAlgorithm::findBestEscapeDirection(const Tank *tank) const
    {
        // this function finds the best escape direction and returns the right direction
        Point pos = tank->getPosition();
        std::vector<Direction> possible_directions;
        // Check all 8 directions for getting valid and safe positions
        int cols = battle_info.getCols();
        int rows = battle_info.getRows();
        const auto &shells = battle_info.getShells();
        for (int i = 0; i < 8; ++i)
        {
            Direction dir = static_cast<Direction>(i);
            std::pair<int, int> offset = directionOffset(dir);
            Point new_pos((pos.getX() + offset.first + cols) % cols,
                          (pos.getY() + offset.second + rows) % rows);

            if (isPositionValid(new_pos) && isPositionSafe(new_pos))
            {
                possible_directions.push_back(dir);
            }
        }

        if (possible_directions.empty())
        {
            // No safe directions, choose the one with the least nearby shells
            Direction safest_dir = tank->getCanonDir();
            int min_shell_count = shells.size();

            for (int i = 0; i < 8; ++i)
            {
                Direction dir = static_cast<Direction>(i);
                std::pair<int, int> offset = directionOffset(dir);
                Point new_pos((pos.getX() + offset.first + cols) % cols,
                              (pos.getY() + offset.second + rows) % rows);

                if (!isPositionValid(new_pos))
                    continue;

                int shell_count = 0;
                for (const auto &shell : shells)
                {
                    if (shell && euclideanDistance(new_pos, shell->getPosition()) <= 2) // shell moves 2 step at time
                        shell_count++;
                }

                if (shell_count < min_shell_count)
                {
                    min_shell_count = shell_count;
                    safest_dir = dir;
                }
            }

            return safest_dir;
        }

        // Choose the direction that maximizes distance from shells
        Direction best_dir = possible_directions[0];
        double max_min_distance = -1.0;

        for (Direction dir : possible_directions)
        {
            std::pair<int, int> offset = directionOffset(dir);
            Point new_pos((pos.getX() + offset.first + cols) % cols,
                          (pos.getY() + offset.second + rows) % rows);

            double min_distance = cols + rows;
            for (const auto &shell_ptr : shells)
            {
                if (shell_ptr)
                {
                    double dist = euclideanDistance(new_pos, shell_ptr->getPosition());
                    min_distance = std::min(min_distance, dist);
                }
            }

            if (min_distance > max_min_distance)
            {
                max_min_distance = min_distance;
                best_dir = dir;
            }
        }

        return best_dir;
    }

    bool HybridTankAlgorithm::isPositionValid(const Point &pos) const
    {
        // This function checks if the position is valid, not occupied by walls or mines
        // Adjust position for tunnel effect
        int cols = battle_info.getCols();
        int rows = battle_info.getRows();
        const auto &walls = battle_info.getWalls();
        const auto &mines = battle_info.getMines();
        Point adjusted_pos((pos.getX() + cols) % cols,
                           (pos.getY() + rows) % rows);
        // Check if position is occupied by a wall
        for (const auto &wall : walls)
        {
            if (this->battle_info.isObjectOnBoard(wall) && wall->getPosition() == adjusted_pos)
            {
                return false;
            }
        }
        // Check if position is occupied by a mine
        for (const auto &mine : mines)
        {
            if (this->battle_info.isObjectOnBoard(mine) && mine->getPosition() == adjusted_pos)
            {
                return false;
            }
        }
        return true;
    }

    bool HybridTankAlgorithm::isPositionSafe(const Point &pos) const
    {
        // This functions checking if shell is not chasing
        return !isShellClose(&pos);
    }

    bool HybridTankAlgorithm::isShellClose(const Point *pos) const
    {
        // This function checks if a shell is close to the tank
        // It returns true if a shell is within the threat radius of the tank
        const auto &shells = battle_info.getShells();
        for (const auto &shell : shells)
        {
            if (shell)
            {
                Point shell_pos = shell->getPosition();
                int distance = euclideanDistance(*pos, shell_pos);
                if (distance <= shell_threat_radius)
                {
                    return true; // A shell is close to the tank
                }
            }
        }
        return false; // No shells are close to the tank
    }

    double HybridTankAlgorithm::euclideanDistance(const Point &a, const Point &b) const
    {
        // This function calculates the Euclidean distance between two points with tunnel effect
        int dx = b.getX() - a.getX();
        int dy = b.getY() - a.getY();
        int cols = battle_info.getCols();
        int rows = battle_info.getRows();
        if (std::abs(dx) > cols / 2)
        {
            dx = (dx > 0) ? dx - cols : dx + cols;
        }
        if (std::abs(dy) > rows / 2)
        {
            dy = (dy > 0) ? dy - rows : dy + rows;
        }
        return std::sqrt(dx * dx + dy * dy);
    }
    std::vector<Tank *> HybridTankAlgorithm::getPlayerTanks(int player_index) const
    {
        // This function retrieves the player tanks based on the player index
        // and returns a vector of raw pointers to the player's tanks.
        std::vector<Tank *> player_tanks = (player_index == 1) ? battle_info.getTanks1() : battle_info.getTanks2();
        return player_tanks;
    }

    ActionRequest HybridTankAlgorithm::getNextChaseAction(Tank *my_tank)
    {
        // this function returns the next action for chasing algorithm
        // First, attempts to shoot if enemy tank is within 2 steps and my_tank is facing the correct direction and able to shoot.
        // If shooting is not possible:
        //      - Checks if a path to enemy tank has already been calculated.
        //      - If no path exists, and at least recalculate_interval turns have passed since the last calculation,
        //          computes a new path.
        // Then:
        //   - If my_tank is facing the correct direction toward the next step in the path, it will attempt to move forward.
        //   - Otherwise, it will rotate toward the correct direction.
        //   - If there are no more planned steps remaining, my_tank will choose a different direction to face.
        if (isInShootingRange(my_tank) && my_tank->canShoot())
        {
            return ActionRequest::Shoot;
        }
        // each recalculate_interval steps calculating new way
        std::vector<Tank *> enemy_tanks = getPlayerTanks(player_index == 1 ? 2 : 1); // Get the enemy tanks
        Tank *closest_enemy_tank = findClosestTank(my_tank->getPosition(), enemy_tanks);
        if (!this->battle_info.isObjectOnBoard(closest_enemy_tank)) { return ActionRequest::DoNothing; }
        if (current_step % recalculate_interval == 1)
        {
            if (closest_enemy_tank)
            {
                findPathStepsToEnemy(my_tank, closest_enemy_tank);
            }
        }

        // we still have a path that we already calculated
        if (!my_tank->getFutureSteps().empty())
        {
            Point pos = my_tank->getPosition();
            std::vector<Point> steps = my_tank->getFutureSteps();
            Point next = steps.front();
            my_tank->setFutureSteps(steps);
            int cols = battle_info.getCols();
            int rows = battle_info.getRows();
            if (next != pos)
            {
                std::pair<int, int> offset = directionOffset(my_tank->getCanonDir());
                Point forward_pos((pos.getX() + offset.first + cols) % cols,
                                  (pos.getY() + offset.second + rows) % rows);
                if (forward_pos == next)
                {
                    steps.erase(steps.begin());
                    my_tank->setFutureSteps(steps);
                    return ActionRequest::MoveForward;
                }
                else
                {
                    // if next step is not needed direction
                    Direction needed_dir = directionTo(pos, next);
                    if (my_tank->getCanonDir() != needed_dir)
                    {
                        int current_dir = static_cast<int>(my_tank->getCanonDir());
                        int target_dir = static_cast<int>(needed_dir);
                        int diff = (target_dir - current_dir + 8) % 8;
                        // choose right direction
                        if (diff == 1 || diff == -7)
                            return ActionRequest::RotateRight45;
                        if (diff == 2 || diff == 3 || diff == 4 || diff == -4 || diff == -5 || diff == -6)
                            return ActionRequest::RotateRight90;
                        if (diff == 6 || diff == 5 || diff == -2 || diff == -3)
                            return ActionRequest::RotateLeft90;
                        if (diff == 7 || diff == -1)
                            return ActionRequest::RotateLeft45;
                    }
                    steps.erase(steps.begin());
                    my_tank->setFutureSteps(steps);
                    return ActionRequest::MoveForward;
                }
            }
        }
        // we don't have more steps
        Direction best_dir = calculateBestDirection(my_tank, closest_enemy_tank);
        if (my_tank->getCanonDir() != best_dir)
        {
            int current_dir = static_cast<int>(my_tank->getCanonDir());
            int target_dir = static_cast<int>(best_dir);
            int diff = (target_dir - current_dir + 8) % 8;
            if (diff == 1 || diff == -7)
                return ActionRequest::RotateRight45;
            if (diff == 2 || diff == 3 || diff == 4 || diff == -4 || diff == -5 || diff == -6)
                return ActionRequest::RotateRight90;
            if (diff == 6 || diff == 5 || diff == -2 || diff == -3)
                return ActionRequest::RotateLeft90;
            if (diff == 7 || diff == -1)
                return ActionRequest::RotateLeft45;
        }
        return ActionRequest::Shoot;
    }

    void HybridTankAlgorithm::findPathStepsToEnemy(Tank *tank1, Tank *tank2)
    {
        // This function finds and sets the future movement steps for tank1 to approach tank2 using BFS.
        // It uses BFS to explore all possible moves in 8 directions (including diagonals and backward movement),
        // considering the board's tunnel effect behavior.
        //  Start BFS from tank1's current position.
        //  For each point visited, check if it matches tank2's position (target).
        //    - If a path is found:
        //      - Reconstruct the path from target to start using parent tracking.
        //      - Take up to recalculate_interval future steps ahead (or fewer if the path is shorter).
        //      - Set these steps in tank1 using `setFutureSteps`.
        //    - If no path to tank2 is found, set an empty future steps list for tank1.

        Point start = tank1->getPosition();
        Point target = tank2->getPosition();

        std::queue<Point> queue;
        std::vector<Point> visited;
        std::vector<std::pair<Point, Point>> parent; // pair of (child, parent)
        std::vector<Point> path;
        // adding first point
        queue.push(start);
        visited.push_back(start);
        int cols = battle_info.getCols();
        int rows = battle_info.getRows();
        while (!queue.empty())
        { // BFS
            Point current = queue.front();
            queue.pop();
            if (current == target)
            {
                // Path found, reconstruct the path
                std::vector<Point> reverse_path;
                Point next = current;
                while (next != start)
                {
                    reverse_path.push_back(next);
                    next = findParent(parent, next);
                }
                std::reverse(reverse_path.begin(), reverse_path.end());
                size_t steps = std::min(size_t(recalculate_interval), reverse_path.size());
                path.assign(reverse_path.begin(), reverse_path.begin() + steps);
                tank1->setFutureSteps(path);
                return;
            }

            // Check all 8 directions including backward movement
            for (int dx = -1; dx <= 1; ++dx)
            {
                for (int dy = -1; dy <= 1; ++dy)
                {
                    if (dx == 0 && dy == 0)
                        continue; // Skip current position
                    Point neighbor((current.getX() + dx + cols) % cols,
                                   (current.getY() + dy + rows) % rows);
                    if (!isPointInVector(visited, neighbor) && isPositionValid(neighbor))
                    {
                        visited.push_back(neighbor);
                        parent.push_back({neighbor, current});
                        queue.push(neighbor);
                    }
                }
            }
        }
        tank1->setFutureSteps({}); // no path
        return;
    }

    bool HybridTankAlgorithm::isPointInVector(const std::vector<Point> &vec, const Point &point) const
    {
        // Check if the point is in the vector
        for (const Point &p : vec)
        {
            if (p == point)
                return true;
        }
        return false;
    }

    Point HybridTankAlgorithm::findParent(const std::vector<std::pair<Point, Point>> &vec, const Point &p) const
    {
        // Find the parent of the point in the vector
        for (size_t i = 0; i < vec.size(); ++i)
        {
            if (vec[i].first.getX() == p.getX() && vec[i].first.getY() == p.getY())
            {
                return vec[i].second;
            }
        }
        return p;
    }

    Direction HybridTankAlgorithm::calculateBestDirection(const Tank *tank1, const Tank *tank2) const
    {
        // this function calculates the best direction to move the tank cannon to the enemy
        Point pos = tank1->getPosition();
        Point tank2_pos = tank2->getPosition();
        int cols = battle_info.getCols();
        int rows = battle_info.getRows();
        // Calculate direction to enemy
        int dx = tank2_pos.getX() - pos.getX();
        int dy = tank2_pos.getY() - pos.getY();
        // Adjust for tunnel effect
        if (std::abs(dx) > cols / 2)
        {
            dx = (dx > 0) ? dx - cols : dx + cols;
        }
        if (std::abs(dy) > rows / 2)
        {
            dy = (dy > 0) ? dy - rows : dy + rows;
        }
        // Determine best direction
        if (dx == 0 && dy < 0)
            return Direction::U;
        if (dx > 0 && dy < 0)
            return Direction::UR;
        if (dx > 0 && dy == 0)
            return Direction::R;
        if (dx > 0 && dy > 0)
            return Direction::DR;
        if (dx == 0 && dy > 0)
            return Direction::D;
        if (dx < 0 && dy > 0)
            return Direction::DL;
        if (dx < 0 && dy == 0)
            return Direction::L;
        return Direction::UL;
    }

    bool HybridTankAlgorithm::isInShootingRange(const Tank *tank) const
    {
        // this function checks if the enemy is in a straight line in the direction of the tank1 cannon
        Point pos = tank->getPosition();
        // Point tank2_pos = tank2->getPosition();
        Direction dir = tank->getCanonDir();
        std::pair<int, int> offset = directionOffset(dir);

        std::vector<Tank *> enemy_tanks = getPlayerTanks(player_index == 1 ? 2 : 1); // Get enemy tanks
        std::vector<Tank *> ally_tanks = getPlayerTanks(player_index == 2 ? 2 : 1);  // Get ally tanks
        // Check if enemy is in line of sight
        Point current = pos;
        int cols = battle_info.getCols();
        int rows = battle_info.getRows();
        for (int step = 0; step < 3; ++step)
        {
            // Move one step forward in direction
            int next_x = (current.getX() + offset.first + cols) % cols;
            int next_y = (current.getY() + offset.second + rows) % rows;
            current = Point(next_x, next_y);
            for (const auto &ally_t : ally_tanks)
            {
                if (ally_t->getPosition() == current)
                {
                    return false; // Ally is blocking the shot
                }
            }

            if (isThereWall(current))
            {
                return false;
            }
            for (const auto &enemy_t : enemy_tanks)
            {
                if (enemy_t->getPosition() == current)
                {
                    return true; // enemy is in shooting range
                }
            }
        }
        return false; // enemy is not in shooting range
    }

    bool HybridTankAlgorithm::isThereWall(const Point &pos) const
    {
        // This function checks if there's a wall object at the given position
        const auto &walls = battle_info.getWalls();
        for (const auto &wall : walls)
        {
            if (battle_info.isObjectOnBoard(wall)){
                if (wall->getPosition() == pos)
                {
                    return true; // Wall found at the position
                }
            }
        }
        return false; // No wall at the position
    }

    Tank *HybridTankAlgorithm::findClosestTank(const Point &from, const std::vector<Tank *> &tanks) const
    {
        // This function finds the closest tank to a given position
        Tank *closest = nullptr;
        double min_dist = std::numeric_limits<double>::max();
        for (Tank *t : tanks)
        {
            if (t)
            {
                double dist = euclideanDistance(from, t->getPosition());
                if (!closest || dist < min_dist)
                {
                    closest = t;
                    min_dist = dist;
                }
            }
        }
        return closest;
    }

    Direction HybridTankAlgorithm::directionTo(const Point &from, const Point &to) const
    {
        // Determines the direction from one point to another, considering the board's tunnel effect
        int dx = to.getX() - from.getX();
        int dy = to.getY() - from.getY();
        int cols = battle_info.getCols();
        int rows = battle_info.getRows();
        if (std::abs(dx) > cols / 2)
            dx = (dx > 0) ? dx - cols : dx + cols;
        if (std::abs(dy) > rows / 2)
            dy = (dy > 0) ? dy - rows : dy + rows;

        if (dx == 0 && dy < 0)
            return Direction::U;
        if (dx > 0 && dy < 0)
            return Direction::UR;
        if (dx > 0 && dy == 0)
            return Direction::R;
        if (dx > 0 && dy > 0)
            return Direction::DR;
        if (dx == 0 && dy > 0)
            return Direction::D;
        if (dx < 0 && dy > 0)
            return Direction::DL;
        if (dx < 0 && dy == 0)
            return Direction::L;
        return Direction::UL;
    }
}// namespace Algorithm_206480972_206899163

