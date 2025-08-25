#include "YaelNogaPlayer.h"
#include <cstddef>
#include <memory>

#include "HybridTankAlgorithm.h"

using namespace std;
using namespace Algorithm_206480972_206899163;

namespace Player_206480972_206899163 { 
    // constructor
    YaelNogaPlayer::YaelNogaPlayer(int player_index, size_t rows, size_t cols, size_t max_steps, size_t num_shells) :
            player_index(player_index), rows(rows), cols(cols), max_steps(max_steps), num_shells(num_shells) {
    }

    YaelNogaPlayer::~YaelNogaPlayer() = default;

    class SatelliteView {
    public:
        virtual ~SatelliteView() = default;
        virtual char getObjectAt(size_t x, size_t y) const = 0;
    };

    int YaelNogaPlayer::getCharCount(const ::SatelliteView& view, char ch) {
        // This function counts the occurrences of a character in the satellite view
        int count = 0;
        for (std::size_t row = 0; row < rows; ++row) {
            for (std::size_t col = 0; col < cols; ++col) {
                if (view.getObjectAt(col, row) == ch) {
                    ++count;
                }
            }
        }
        return count;
    }

    int YaelNogaPlayer::getSelfTankCount(const ::SatelliteView& view) {
        // This function counts the occurrences of the player's tank in the satellite view
        return getCharCount(view, player_index== 1 ? '1' : '2') + 1;
    }

    int YaelNogaPlayer::getOpponentTankCount(const ::SatelliteView& view) {
        // This function counts the occurrences of the opponent's tank in the satellite view
        return getCharCount(view, player_index== 1 ? '2' : '1') + 1;
    }

    void YaelNogaPlayer::updateTankWithBattleInfo(TankAlgorithm& tank, ::SatelliteView& view) {
        // Update the tank's battle information based on the satellite view
        SimpleBattleInfo battleInfo = SimpleBattleInfo(view, rows, cols, num_shells, player_index);
        int selfCount  = getSelfTankCount(view);
        int opponentCount = getOpponentTankCount(view);

        auto self_tank = static_cast<HybridTankAlgorithm*>(&tank);

        if (opponentCount > selfCount ) {
            self_tank->setDefensive();
        } else if (selfCount > opponentCount) {
            self_tank->setOffensive();
        } else {
            (self_tank->getIndex() % 2 == 0) ? self_tank->setOffensive() : self_tank->setDefensive();
        }
        tank.updateBattleInfo(battleInfo);
    }
}

