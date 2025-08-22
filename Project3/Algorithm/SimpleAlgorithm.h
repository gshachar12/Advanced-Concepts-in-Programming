#ifndef SIMPLEALGORITHM_H
#define SIMPLEALGORITHM_H

#include "../common/TankAlgorithm.h"
#include "../common/ActionRequest.h"
#include "../common/BattleInfo.h"
#include <string>

namespace Algorithm_123456789_987654321 {

class SimpleAlgorithm : public TankAlgorithm {
private:
    int player_index_;
    int tank_index_;
    int turn_count_;

public:
    SimpleAlgorithm(int player_index, int tank_index);
    
    ActionRequest getAction() override;
    void updateBattleInfo(BattleInfo& info) override;
};

} // namespace Algorithm_123456789_987654321

#endif // SIMPLEALGORITHM_H
