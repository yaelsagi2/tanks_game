
#pragma once

#include "../common/Player.h"

namespace Player_206480972_206899163 {

class YaelNogaPlayer : public Player {
public:
    YaelNogaPlayer(int player_index, size_t x, size_t y, size_t max_steps, size_t num_shells);
    ~YaelNogaPlayer() override;
    void updateTankWithBattleInfo(TankAlgorithm&, SatelliteView&) override;
};

}
