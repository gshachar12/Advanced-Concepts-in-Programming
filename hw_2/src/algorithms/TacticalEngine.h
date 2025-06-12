#ifndef TACTICAL_ENGINE_H
#define TACTICAL_ENGINE_H

#include "Direction.h"
#include "MyTankAlgorithm.h"
#include "StrategicPlanner.h"

/**
 * @brief Advanced tactical algorithm using the Strategy Pattern
 * 
 * TacticalEngine is an algorithm that fully leverages the Strategy Pattern
 * to make tactical decisions. Unlike other algorithms that have hardcoded
 * logic, this algorithm delegates most of its behavior to interchangeable
 * tactical strategies.
 */
class TacticalEngine final : public MyTankAlgorithm {
public:
    /**
     * @brief Construct a new Tactical Engine object
     * 
     * @param player_id The identifier of the controlling player
     * @param tank_index The identifier of the controlled vehicle
     */
    explicit TacticalEngine(int player_id, int tank_index) : MyTankAlgorithm(player_id, tank_index) {
        initializeStrategies();
        initializeTargetTracking();
    }

    /**
     * @brief Get the algorithm's identifier
     * 
     * @return std::string The unique name of this algorithm
     */
    std::string getName() const override { return "TacticalEngine"; }

    /**
     * @brief Primary decision engine for determining optimal actions
     * 
     * @param request Pointer to the action decision output
     * @param request_title Pointer to the description of the chosen action
     */
    void calculateAction(ActionRequest *request, std::string *request_title) override;

private:
    // Strategic planning components
    StrategicPlanner primary_planner;
    StrategicPlanner navigation_planner;
    
    // State tracking variables
    bool alert_status{false};
    std::vector<Position> target_position_log = {};
    std::vector<Direction::DirectionType> movement_plan;
    bool recalculate_route{false};
    
    // Target tracking methods
    void initializeTargetTracking();
    void updateTargetLog();
    bool haveTargetsRepositioned() const;
    
    // Navigation methods
    void updateNavigationPlan();
    void evaluateMovementOptions(ActionRequest *request, std::string *request_title);
    std::vector<Direction::DirectionType> calculateEfficientPath();
    
    // Strategy initialization
    void initializeStrategies();
};

#endif // TACTICAL_ENGINE_H
