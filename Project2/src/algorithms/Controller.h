#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "../common/TankAlgorithm.h"
#include "../common/ActionRequest.h"
#include "../TankBattleInfo.h"
#include <vector>

class Controller : public TankAlgorithm {
private:
    int playerIndex;  // 1 or 2
    int tankIndex;    // Tank's index within its player
    std::vector<ActionRequest> actionSequence; // Sequence of actions to perform
    size_t currentActionIndex;                 // Current position in the sequence
    Position position;                         // Last known position
    Direction direction;                       // Last known direction
    bool needsBattleInfo;                      // Whether to request battle info

public:
    Controller(int player_index, int tank_index);
    ActionRequest getAction() override;
    void updateBattleInfo(BattleInfo& info) override;
    
private:
    // Initialize a deterministic action sequence based on player and tank index
    void initActionSequence();
};

#endif // CONTROLLER_H