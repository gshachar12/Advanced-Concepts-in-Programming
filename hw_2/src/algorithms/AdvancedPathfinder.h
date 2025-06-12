#ifndef ADVANCED_PATHFINDER_H
#define ADVANCED_PATHFINDER_H

#include "Direction.h"
#include "MyTankAlgorithm.h"
#include "StrategicPlanner.h"

/**
 * @brief Advanced tactical algorithm that uses graph traversal techniques for optimal path determination
 * 
 * AdvancedPathfinder employs a breadth-first traversal strategy to identify the most efficient
 * route to a position allowing successful target engagement. The algorithm continuously adapts
 * to changing battlefield conditions by reassessing paths when obstacles or threats are encountered.
 */
class AdvancedPathfinder final : public MyTankAlgorithm {
public:
    /**
     * @brief Construct a new Advanced Pathfinder object
     * 
     * @param player_id The identifier of the controlling player
     * @param tank_index The identifier of the controlled vehicle
     */
    explicit AdvancedPathfinder(int player_id, int tank_index) : MyTankAlgorithm(player_id, tank_index) {
        initTargetPositionMonitoring();
    }

    /**
     * @brief Get the algorithm's identifier
     * 
     * @return std::string The unique name of this algorithm
     */
    std::string getName() const override { return "AdvancedPathfinder"; }

    /**
     * @brief Primary tactical decision engine for determining optimal actions
     * 
     * @param request Pointer to the action decision output
     * @param request_title Pointer to the description of the chosen action
     */
    void calculateAction(ActionRequest *request, std::string *request_title) override;

private:
    // State monitoring variables
    bool threat_alert{false};
    std::vector<Position> target_position_history = {};

    // Target position monitoring methods
    void initTargetPositionMonitoring();
    void updateTargetPositions();
    bool haveTargetsMoved() const;

    // Tactical response methods
    void executeThreatResponse(ActionRequest *request, std::string *request_title);
    void attemptTargetAcquisition(ActionRequest *request, std::string *request_title);
    bool adjustAimingVector(ActionRequest *request, std::string *request_title) const;

    // Route planning and execution methods
    void reassessNavigationRoute();
    void handleNavigationFailure(ActionRequest *request, std::string *request_title) const;
    void executeNavigationStep(ActionRequest *request, std::string *request_title);
    
    // Navigation algorithm core
    std::vector<Direction::DirectionType> calculateOptimalRoute();
};


#endif //ADVANCED_PATHFINDER_H
