#ifndef STRATEGIC_PLANNER_H
#define STRATEGIC_PLANNER_H

#include <vector>
#include <memory>
#include "TacticalStrategies.h"
#include "MyBattleStatus.h"

/**
 * @brief Strategic planning component for tank algorithms
 * 
 * The StrategicPlanner manages a prioritized collection of tactical strategies
 * and executes them in order until a decision is made. This allows tank algorithms
 * to use a compositional approach rather than inheriting specific behaviors.
 */
class StrategicPlanner {
public:
    /**
     * @brief Construct a new Strategic Planner with default strategies
     */
    StrategicPlanner() {
        // Add default strategies in priority order
        addStrategy("ThreatResponse");
        addStrategy("DirectEngagement");
        addStrategy("TargetAcquisition");
        addStrategy("ObstacleBreach");
    }
    
    /**
     * @brief Construct a Strategic Planner with specified strategy names
     * 
     * @param strategy_names Vector of strategy names in priority order
     */
    explicit StrategicPlanner(const std::vector<std::string>& strategy_names) {
        for (const auto& name : strategy_names) {
            addStrategy(name);
        }
    }
    
    /**
     * @brief Add a strategy to the planner
     * 
     * @param strategy_name The name of the strategy to add
     */
    void addStrategy(const std::string& strategy_name) {
        strategies.push_back(TacticalStrategyFactory::create(strategy_name));
    }
    
    /**
     * @brief Execute all strategies in priority order until one succeeds
     * 
     * @param battle_status Reference to the current battle status
     * @param request Pointer to store the resulting action request
     * @param request_title Pointer to store a description of the action
     * @return true if any strategy made a decision, false if all strategies passed
     */
    bool executeStrategies(
        const MyBattleStatus& battle_status,
        ActionRequest* request,
        std::string* request_title
    ) const {
        for (const auto& strategy : strategies) {
            if (strategy->execute(battle_status, request, request_title)) {
                return true;
            }
        }
        return false;
    }
    
    /**
     * @brief Clear all strategies from the planner
     */
    void clearStrategies() {
        strategies.clear();
    }
    
private:
    std::vector<std::unique_ptr<TacticalStrategy>> strategies;
};

#endif // STRATEGIC_PLANNER_H
