#ifndef DEFENSIVE_H
#define DEFENSIVE_H

#include "../common/TankAlgorithm.h"
#include "../common/ActionRequest.h"
#include "../TankBattleInfo.h"
#include "../Position.h"
#include "../Directions.h"
#include <vector>

class Defensive : public TankAlgorithm {
private:
    int playerIndex;                   // 1 or 2
    int tankIndex;                     // Tank's index within its player
    std::vector<ActionRequest> actionSequence; // Sequence of actions to perform
    size_t currentActionIndex;         // Current position in the sequence
    Position position;                 // Last known position
    Direction direction;               // Last known direction
    bool needsBattleInfo;              // Whether to request battle info
    bool enemyDetected;                // Whether an enemy tank was detected
    int consecutiveShots;              // Count of consecutive shots (for limiting)

public:
    Defensive(int player_index, int tank_index);
    ActionRequest getAction() override;
    
    void updateBattleInfo(BattleInfo& info) override;
    
private:
    // Initialize a defensive action sequence based on player and tank index
    void initActionSequence();
    void ChaseBFS(const TankBattleInfo& info);
    // Determine if we should retreat based on battle info
    bool shouldRetreat(const TankBattleInfo& info) const;
    // Plan a defensive action when under threat
    ActionRequest planDefensiveAction(const TankBattleInfo& info);
};

#endif // DEFENSIVE_H