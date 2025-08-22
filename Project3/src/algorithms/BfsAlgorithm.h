#ifndef BFS_ALGORITHM_H
#define BFS_ALGORITHM_H

#include "../../common/TankAlgorithm.h"
#include "../../common/BattleInfo.h"
#include "../TankBattleInfo.h"
#include <queue>
#include <vector>
#include <set>

/**
 * BFS-based tank algorithm that uses pathfinding
 * - Analyzes the battlefield using BattleInfo
 * - Uses BFS to find paths to objectives
 * - Avoids obstacles and enemy fire
 */
class BfsAlgorithm : public TankAlgorithm {
private:
    struct Node {
        int x, y;
        int steps;
        ActionRequest action_taken;
        
        Node(int x, int y, int steps, ActionRequest action) 
            : x(x), y(y), steps(steps), action_taken(action) {}
    };
    
    TankBattleInfo* current_battle_info_;
    std::vector<ActionRequest> planned_actions_;
    size_t action_index_;
    int turn_counter_;
    
    // Strategy parameters
    static const int MAX_SEARCH_DEPTH = 10;
    static const int REPLANNING_FREQUENCY = 5;
    
public:
    BfsAlgorithm();
    
    void updateBattleInfo(BattleInfo& info) override;
    ActionRequest getAction() override;

private:
    void planActions();
    std::vector<ActionRequest> findPathToTarget(int target_x, int target_y);
    bool isValidPosition(int x, int y) const;
    bool isPositionSafe(int x, int y) const;
    ActionRequest getMovementAction(int from_x, int from_y, int to_x, int to_y) const;
    std::pair<int, int> findBestTarget() const;
    bool shouldShoot() const;
};

#endif // BFS_ALGORITHM_H
