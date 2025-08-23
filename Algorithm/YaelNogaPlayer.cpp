//
// Created by geva on 12/08/2025.
//

#include "YaelNogaPlayer.h"
#include <cstddef>
using namespace std;

namespace Player_206480972_206899163 {
    YaelNogaPlayer::YaelNogaPlayer(int , size_t , size_t , size_t , size_t ) {
    }

    YaelNogaPlayer::~YaelNogaPlayer() = default;

    void YaelNogaPlayer::updateTankWithBattleInfo(TankAlgorithm&, SatelliteView&) {

        //tank.updateBattleInfo() SimpleBattleInfo
     /*   const int myCount  = view.GetPlayerTankCount();
        const int oppCount = view.GetOpponentTankCount();

        if (myCount <= 0) {
            // No friendlies visible → play safe.
            tank.setDefensive();
            return;
        }

        if (oppCount > myCount) {
            tank.setDefensive();
        } else if (myCount > oppCount) {
            tank.setOffensive();
        } else {
            // Tie: alternate roles by index to avoid everyone making the same choice.
            (tank.getIndex() % 2 == 0) ? tank.setOffensive() : tank.setDefensive();
        }*/
    }
}