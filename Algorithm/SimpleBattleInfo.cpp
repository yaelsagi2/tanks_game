#include "SimpleBattleInfo.h"
#include <memory>
#include <algorithm>

using namespace UserCommon_206480972_206899163;
namespace Algorithm_206480972_206899163{

// Constructor: Builds SimpleBattleInfo from a SatelliteView and player info
SimpleBattleInfo::SimpleBattleInfo(const SatelliteView& view, size_t x, size_t y, int ammo, int player_asked)
    : rows(y), cols(x), ammo_count(ammo), boardView(x, std::vector<char>(y, ' ')), player_asked_for_info(player_asked) {
    for (size_t i = 0; i < x; ++i) {
        for (size_t j = 0; j < y; ++j) {
            boardView[i][j] = view.getObjectAt(i, j);
            switch (boardView[i][j]) {
                case '#': // Wall
                    walls.push_back(std::make_unique<::Wall>(i, j));
                    break;
                case '@': // Mine
                    mines.push_back(std::make_unique<::Mine>(i, j));
                    break;
                case '*': // Shell
                    shells.push_back(std::make_unique<::Shell>(::Point(i, j), ::Direction::None, 0));
                    break;
                case '1': // Tank 1
                    tanks1.push_back(std::make_unique<::Tank>(i, j, 1 ,1, ammo));
                    break;
                case '2': // Tank 2
                    tanks2.push_back(std::make_unique<::Tank>(i, j, 2 ,2, ammo));
                    break;
                case '%': // My tank position
                    myTank = std::make_unique<::Tank>(i, j, 0, player_asked_for_info, ammo);
                    break;
                default:
                    break;
        }
    }
}
}

// Get the number of columns
size_t SimpleBattleInfo::getCols() const {
    return cols;
}

// Get the number of rows
size_t SimpleBattleInfo::getRows() const {
    return rows;
}

// Get the player's ammo count
int SimpleBattleInfo::getAmmoCount() const {
    return ammo_count;
}

// Get all wall pointers
std::vector<::Wall*> SimpleBattleInfo::getWalls() const {
    std::vector<::Wall*> result;
    for (const auto& w : walls) {
        result.push_back(w.get());
    }
    return result;
}

// Get all mine pointers
std::vector<::Mine*> SimpleBattleInfo::getMines() const {
    std::vector<::Mine*> result;
    for (const auto& m : mines) {
        result.push_back(m.get());
    }
    return result;
}

// Get all shell pointers
std::vector<::Shell*> SimpleBattleInfo::getShells() const {
    std::vector<::Shell*> result;
    for (const auto& s : shells) {
        result.push_back(s.get());
    }
    return result;
}

// Get all player 1 tank pointers (excluding myTank)
std::vector<::Tank*> SimpleBattleInfo::getTanks1() const {
    std::vector<::Tank*> result;
    for (const auto& t : tanks1) {
        result.push_back(t.get());
    }
    return result;
}

// Get all player 2 tank pointers (excluding myTank)
std::vector<::Tank*> SimpleBattleInfo::getTanks2() const {
    std::vector<::Tank*> result;
    for (const auto& t : tanks2) {
        result.push_back(t.get());
    }
    return result;
}

// Get a pointer to the player's own tank
Tank* SimpleBattleInfo::getMyTank() const {
    return myTank.get();
}

// Get the board view as a 2D char vector
const std::vector<std::vector<char>>& SimpleBattleInfo::getBoardView() const {
    return boardView;
}

// Add a shell to the board
void SimpleBattleInfo::addShell(const Shell& shell) {
    auto shell_ptr = std::make_unique<Shell>(shell);
    shells.push_back(std::move(shell_ptr));
}

// Remove a shell from the shells vector
void SimpleBattleInfo::removeShell(Shell* shell) {
    if (!shell) return;
    shells.erase(
        std::remove_if(shells.begin(), shells.end(),
            [shell](const std::unique_ptr<Shell>& s) {
                return s.get() == shell;
            }),
        shells.end()
    );
}

// Remove a wall from the walls vector
void SimpleBattleInfo::removeWall(Wall* wall) {
    if (!wall) return;
    walls.erase(std::remove_if(walls.begin(), walls.end(),
            [wall](const std::unique_ptr<Wall>& w) {
                return w.get() == wall;
            }),
        walls.end()
    );
}

// Remove a mine from the mines vector
void SimpleBattleInfo::removeMine(Mine* mine) {
    if (!mine) return;
    mines.erase(std::remove_if(mines.begin(), mines.end(),
            [mine](const std::unique_ptr<Mine>& m) {
                return m.get() == mine;
            }),
        mines.end()
    );
}

// Remove a tank from the appropriate tanks vector (tanks1 or tanks2)
void SimpleBattleInfo::removeTank(Tank* tank) {
    if (!tank) return;
    if (tank->getId() == 1) {
        tanks1.erase(std::remove_if(tanks1.begin(), tanks1.end(),
            [tank](const std::unique_ptr<Tank>& t) {
                return t.get() == tank;
            }),
            tanks1.end());
    } else if (tank->getId() == 2) {
        tanks2.erase(std::remove_if(tanks2.begin(), tanks2.end(),
            [tank](const std::unique_ptr<Tank>& t) {
                return t.get() == tank;
            }),
            tanks2.end());
    }
}

// Remove the player's own tank from SimpleBattleInfo
void SimpleBattleInfo::removeMyTank()
{
    if (myTank) {
        myTank.reset(); // Deletes the inner object and makes ptr == nullptr
    }
}

// Check if the battle info is initialized (nonzero board size)
bool SimpleBattleInfo::isInitialized() const{
    if (cols == 0 && rows == 0){
        return false;
    }
    return true;
}

// Copy constructor: deep copy all objects and board state
SimpleBattleInfo::SimpleBattleInfo(const SimpleBattleInfo& other)
    : rows(other.rows), cols(other.cols), ammo_count(other.ammo_count), boardView(other.boardView) 
{
    // Deep copy walls
    for (const auto& wall : other.walls) {
        walls.emplace_back(std::make_unique<Wall>(*wall));
    }
    // Deep copy mines
    for (const auto& mine : other.mines) {
        mines.emplace_back(std::make_unique<Mine>(*mine));
    }
    // Deep copy shells
    for (const auto& shell : other.shells) {
        shells.emplace_back(std::make_unique<Shell>(*shell));
    }
    // Deep copy tanks1
    for (const auto& tank : other.tanks1) {
        tanks1.emplace_back(std::make_unique<Tank>(*tank));
    }
    // Deep copy tanks2
    for (const auto& tank : other.tanks2) {
        tanks2.emplace_back(std::make_unique<Tank>(*tank));
    }
    // Deep copy myTank
    if (other.myTank) {
        myTank = std::make_unique<Tank>(*other.myTank);
    }
}

// Print the current battle state to the console
void SimpleBattleInfo::printBattleState() const {
    // Create an empty board
    std::vector<std::vector<char>> board(cols, std::vector<char>(rows, ' '));
    for (const auto& wall : walls) { // Place walls
        Point pos = wall->getPosition();
        board[pos.getX()][pos.getY()] = '#';
    }
    for (const auto& mine : mines) { // Place mines
        Point pos = mine->getPosition();
        board[pos.getX()][pos.getY()] ='@';
    }
    for (const auto& shell : shells) { // Place shells
        Point pos = shell->getPosition();
        board[pos.getX()][pos.getY()] = '*';
    }
    for (const auto& tank : tanks1) { // Place player 1 tanks (skip if myTank is at the same position)
        if (myTank && tank->getPosition() == myTank->getPosition()) {
            continue;
        }
        Point pos = tank->getPosition();
        board[pos.getX()][pos.getY()] = '1';
    }
    for (const auto& tank : tanks2) {    // Place player 2 tanks (skip if myTank is at the same position)
        if (myTank && tank->getPosition() == myTank->getPosition()) {
            continue;
        }
        Point pos = tank->getPosition();
        board[pos.getX()][pos.getY()] = '2';
    }
    if (myTank) {     // Place myTank (overwrites any other tank at the same position)
        Point pos = myTank->getPosition();
        board[pos.getX()][pos.getY()] = '%';
    }
    for (size_t y = 0; y < rows; ++y) {     // Print the board from top to bottom (0,0 is top-left)
        for (size_t x = 0; x < cols; ++x) {
            std::cout << board[x][y];
        }
        std::cout << '\n';
    }
}

bool SimpleBattleInfo::isObjectOnBoard(const GameObject* object) const {
    //This function checks if a given GameObject is present on the board
    if (!object) return false;

    for (const auto& w : walls) {
        if (w.get() == object) return true;
    }

    for (const auto& m : mines) {
        if (m.get() == object) return true;
    }

    for (const auto& s : shells) {
        if (s.get() == object) return true;
    }

    for (const auto& t : tanks1) {
        if (t.get() == object) return true;
    }

    for (const auto& t : tanks2) {
        if (t.get() == object) return true;
    }

    if (myTank && myTank.get() == object) return true;

    return false;
}

}