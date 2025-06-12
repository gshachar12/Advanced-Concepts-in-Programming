#ifndef TACTICAL_STRATEGIES_H
#define TACTICAL_STRATEGIES_H

#include <memory>
#include <string>
#include "Direction.h"
#include "ActionRequest.h"
#include "MyBattleStatus.h"

/**
 * @brief Abstract base class for tactical decision strategies
 * 
 * This class defines a strategy pattern interface for different tactical decision-making
 * approaches that can be used by various tank algorithms.
 */
class TacticalStrategy {
public:
    virtual ~TacticalStrategy() = default;
    
    /**
     * @brief Execute the tactical strategy to determine the optimal action
     * 
     * @param battle_status Reference to the current battle status
     * @param request Pointer to store the resulting action request
     * @param request_title Pointer to store a description of the action
     * @return true if the strategy decided on an action, false otherwise
     */
    virtual bool execute(
        const MyBattleStatus& battle_status,
        ActionRequest* request, 
        std::string* request_title
    ) const = 0;
    
    /**
     * @brief Get the name of this tactical strategy
     * 
     * @return std::string The strategy name
     */
    virtual std::string getName() const = 0;
};

/**
 * @brief Strategy for directly engaging enemies when they're in firing range
 */
class DirectEngagementStrategy : public TacticalStrategy {
public:
    bool execute(
        const MyBattleStatus& battle_status,
        ActionRequest* request, 
        std::string* request_title
    ) const override {
        // If we can directly hit a target, shoot
        if (battle_status.canHitTarget()) {
            *request = ActionRequest::Shoot;
            *request_title = "Direct Target Engagement";
            return true;
        }
        return false;
    }
    
    std::string getName() const override {
        return "DirectEngagement";
    }
};

/**
 * @brief Strategy for rotating to acquire firing solutions
 */
class TargetAcquisitionStrategy : public TacticalStrategy {
public:
    bool execute(
        const MyBattleStatus& battle_status,
        ActionRequest* request, 
        std::string* request_title
    ) const override {
        // Check all possible orientations for a firing solution
        for (int dir_index = 0; dir_index < 8; ++dir_index) {
            // Skip current orientation
            Direction::DirectionType candidate_direction = Direction::getDirectionFromIndex(dir_index);
            if (battle_status.vehicle_orientation == candidate_direction) {
                continue;
            }
            
            // If this orientation provides a firing solution, rotate toward it
            if (battle_status.canHitTarget(candidate_direction)) {
                *request = battle_status.calculateOptimalRotation(candidate_direction);
                *request_title = "Tactical Realignment to Position " + std::to_string(dir_index);
                return true;
            }
        }
        return false;
    }
    
    std::string getName() const override {
        return "TargetAcquisition";
    }
};

/**
 * @brief Strategy for clearing obstacles through offensive action
 */
class ObstacleBreachStrategy : public TacticalStrategy {
public:
    bool execute(
        const MyBattleStatus& battle_status,
        ActionRequest* request, 
        std::string* request_title
    ) const override {
        // If weapon is ready and there's no other action to take
        if (battle_status.isWeaponReady()) {
            // Look for a wall in our immediate field of view
            Position forward_pos = battle_status.normalizeCoordinates(
                battle_status.vehicle_location + battle_status.vehicle_orientation);
                
            if (battle_status.getTerrainAt(forward_pos) == '#') {
                *request = ActionRequest::Shoot;
                *request_title = "Obstacle Clearance";
                return true;
            }
        }
        return false;
    }
    
    std::string getName() const override {
        return "ObstacleBreach";
    }
};

/**
 * @brief Strategy for emergency response to immediate threats
 */
class ThreatResponseStrategy : public TacticalStrategy {
public:
    bool execute(
        const MyBattleStatus& battle_status,
        ActionRequest* request, 
        std::string* request_title
    ) const override {
        // If threatened by enemies or projectiles
        if (battle_status.isProjectileInVicinity() || battle_status.isOpponentNearby()) {
            // First try to eliminate the threat if possible
            if (battle_status.canHitTarget(true)) {
                *request = ActionRequest::Shoot;
                *request_title = "Defensive Counter-Fire";
                return true;
            }
            
            // Otherwise, try to move away from danger
            // Implementation depends on moveIfThreatened() in derived classes
            return false;
        }
        return false;
    }
    
    std::string getName() const override {
        return "ThreatResponse";
    }
};

/**
 * @brief Factory for creating tactical strategy objects
 */
class TacticalStrategyFactory {
public:
    /**
     * @brief Create a specific tactical strategy by name
     * 
     * @param strategy_name The name of the strategy to create
     * @return std::unique_ptr<TacticalStrategy> A unique pointer to the created strategy
     */
    static std::unique_ptr<TacticalStrategy> create(const std::string& strategy_name) {
        if (strategy_name == "DirectEngagement") {
            return std::make_unique<DirectEngagementStrategy>();
        } else if (strategy_name == "TargetAcquisition") {
            return std::make_unique<TargetAcquisitionStrategy>();
        } else if (strategy_name == "ObstacleBreach") {
            return std::make_unique<ObstacleBreachStrategy>();
        } else if (strategy_name == "ThreatResponse") {
            return std::make_unique<ThreatResponseStrategy>();
        }
        
        // Default to direct engagement if strategy not found
        return std::make_unique<DirectEngagementStrategy>();
    }
};

#endif // TACTICAL_STRATEGIES_H
