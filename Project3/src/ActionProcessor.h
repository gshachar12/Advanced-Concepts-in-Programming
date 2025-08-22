#ifndef ACTION_PROCESSOR_H
#define ACTION_PROCESSOR_H

#include "../common/ActionRequest.h"
#include "GameState.h"
#include "CollisionDetector.h"
#include "TankBattleInfo.h"

/**
 * Processes tank actions and updates game state accordingly
 */
class ActionProcessor {
private:
    GameState* game_state_;
    CollisionDetector* collision_detector_;

public:
    ActionProcessor(GameState* game_state, CollisionDetector* collision_detector);
    
    /**
     * Process a tank action and update game state
     * @param tank The tank performing the action
     * @param action The action to perform
     * @return True if action was successfully executed
     */
    bool processAction(Tank* tank, ActionRequest action);
    
    /**
     * Create battle info for a specific tank
     */
    std::unique_ptr<TankBattleInfo> createBattleInfo(const Tank* tank) const;

private:
    bool processMoveForward(Tank* tank);
    bool processMoveBackward(Tank* tank);
    bool processRotateLeft90(Tank* tank);
    bool processRotateRight90(Tank* tank);
    bool processRotateLeft45(Tank* tank);
    bool processRotateRight45(Tank* tank);
    bool processShoot(Tank* tank);
    
    // Helper methods
    std::vector<Position> getVisibleEnemies(const Tank* tank) const;
    std::vector<Position> getVisibleShells(const Tank* tank) const;
    bool isPositionVisible(const Position& from, const Position& to) const;
};

#endif // ACTION_PROCESSOR_H
