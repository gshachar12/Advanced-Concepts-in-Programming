#include "catch.hpp"

#include "../src/algorithms/TacticalStrategies.h"
#include "../src/algorithms/MyBattleStatus.h"

/**
 * @brief Test suite for tactical strategies implementation
 * 
 * This test suite verifies that individual tactical strategies correctly
 * implement their intended behavior and decision-making logic.
 */
TEST_CASE("Tactical strategies functionality tests", "[tactical][strategies]") {
    SECTION("Strategy factory correctly creates strategy objects") {
        // Test creating each type of strategy
        auto direct = TacticalStrategyFactory::create("DirectEngagement");
        REQUIRE(direct->getName() == "DirectEngagement");
        
        auto acquisition = TacticalStrategyFactory::create("TargetAcquisition");
        REQUIRE(acquisition->getName() == "TargetAcquisition");
        
        auto breach = TacticalStrategyFactory::create("ObstacleBreach");
        REQUIRE(breach->getName() == "ObstacleBreach");
        
        auto threat = TacticalStrategyFactory::create("ThreatResponse");
        REQUIRE(threat->getName() == "ThreatResponse");
        
        // Test default behavior with unknown strategy
        auto unknown = TacticalStrategyFactory::create("NonExistentStrategy");
        REQUIRE(unknown->getName() == "DirectEngagement"); // Should default to DirectEngagement
    }
    
    SECTION("DirectEngagementStrategy handles shooting decisions correctly") {
        // Setup
        DirectEngagementStrategy strategy;
        MyBattleStatus status(1, 0);
        ActionRequest request = ActionRequest::DoNothing;
        std::string request_title;
        
        // Test when shooting is possible
        status.setTestCondition_CanShoot(true);
        bool result = strategy.execute(status, &request, &request_title);
        
        REQUIRE(result);
        REQUIRE(request == ActionRequest::Shoot);
        REQUIRE(request_title == "Direct Target Engagement");
        
        // Test when shooting is not possible
        status.setTestCondition_CanShoot(false);
        request = ActionRequest::DoNothing;
        result = strategy.execute(status, &request, &request_title);
        
        REQUIRE_FALSE(result);
        REQUIRE(request == ActionRequest::DoNothing);
    }
    
    SECTION("TargetAcquisitionStrategy searches for optimal firing directions") {
        // Setup
        TargetAcquisitionStrategy strategy;
        MyBattleStatus status(1, 0);
        status.vehicle_orientation = Direction::Up;
        ActionRequest request = ActionRequest::DoNothing;
        std::string request_title;
        
        // Test when no firing direction is available
        status.setTestCondition_CanShootFromDirection(Direction::Up); // Same as current orientation
        bool result = strategy.execute(status, &request, &request_title);
        REQUIRE_FALSE(result);
        
        // Test when a firing direction is available to the right
        status.setTestCondition_CanShootFromDirection(Direction::Right);
        result = strategy.execute(status, &request, &request_title);
        
        REQUIRE(result);
        REQUIRE(request == ActionRequest::RotateRight45 || request == ActionRequest::RotateRight90);
        REQUIRE(request_title.find("Tactical Realignment") != std::string::npos);
        
        // Test when a firing direction is available to the left
        status.setTestCondition_CanShootFromDirection(Direction::Left);
        request = ActionRequest::DoNothing;
        result = strategy.execute(status, &request, &request_title);
        
        REQUIRE(result);
        REQUIRE(request == ActionRequest::RotateLeft45 || request == ActionRequest::RotateLeft90);
        REQUIRE(request_title.find("Tactical Realignment") != std::string::npos);
    }
    
    SECTION("ObstacleBreachStrategy correctly handles obstacles") {
        // Setup
        ObstacleBreachStrategy strategy;
        MyBattleStatus status(1, 0);
        status.configureTestMap(10, 10);
        status.vehicle_location = {5, 5};
        status.vehicle_orientation = Direction::Up;
        ActionRequest request = ActionRequest::DoNothing;
        std::string request_title;
        
        // Test when there's no wall ahead
        bool result = strategy.execute(status, &request, &request_title);
        REQUIRE_FALSE(result);
        
        // Test when there's a wall ahead and can shoot
        Position wall_pos = status.normalizeCoordinates(status.vehicle_location + status.vehicle_orientation);
        status.setWallAt(wall_pos);
        status.setTestCondition_CanShoot(true);
        
        result = strategy.execute(status, &request, &request_title);
        REQUIRE(result);
        REQUIRE(request == ActionRequest::Shoot);
        REQUIRE(request_title == "Obstacle Clearance");
        
        // Test when there's a wall ahead but can't shoot
        status.setTestCondition_CanShoot(false);
        request = ActionRequest::DoNothing;
        result = strategy.execute(status, &request, &request_title);
        REQUIRE_FALSE(result);
    }
    
    SECTION("ThreatResponseStrategy correctly responds to threats") {
        // Setup
        ThreatResponseStrategy strategy;
        MyBattleStatus status(1, 0);
        ActionRequest request = ActionRequest::DoNothing;
        std::string request_title;
        
        // Test when there's no threat
        bool result = strategy.execute(status, &request, &request_title);
        REQUIRE_FALSE(result);
        
        // Test when there's a threat and can shoot
        status.setTestCondition_Threatened(true);
        status.setTestCondition_CanShoot(true);
        
        result = strategy.execute(status, &request, &request_title);
        REQUIRE(result);
        REQUIRE(request == ActionRequest::Shoot);
        REQUIRE(request_title == "Defensive Counter-Fire");
        
        // Test when there's a threat but can't shoot
        status.setTestCondition_CanShoot(false);
        request = ActionRequest::DoNothing;
        result = strategy.execute(status, &request, &request_title);
        REQUIRE_FALSE(result);
    }
}
