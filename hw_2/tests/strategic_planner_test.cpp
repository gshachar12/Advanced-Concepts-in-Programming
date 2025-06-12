#include "catch.hpp"

#include "../src/algorithms/StrategicPlanner.h"
#include "../src/algorithms/MyBattleStatus.h"

/**
 * @brief Test fixture for strategic planner tests
 */
class StrategicPlannerTestFixture {
protected:
    // Set up a battle status for testing
    StrategicPlannerTestFixture() : battle_status(1, 0) {
        battle_status.arena_width = 10;
        battle_status.arena_height = 10;
        battle_status.vehicle_location = {5, 5};
        battle_status.vehicle_orientation = Direction::Up;
        battle_status.current_turn = 3;
        battle_status.ammo_count = 5;
        battle_status.weapon_cooldown = 0;
    }
    
    // Battle status for testing
    MyBattleStatus battle_status;
    
    // Request objects for strategy execution
    ActionRequest request = ActionRequest::DoNothing;
    std::string request_title;
};

/**
 * @brief Test suite for the StrategicPlanner component
 * 
 * These tests verify that the StrategicPlanner correctly manages and
 * executes tactical strategies according to their priority order.
 */
TEST_CASE_METHOD(StrategicPlannerTestFixture, "StrategicPlanner functionality tests", "[strategic][planning]") {
    SECTION("Default planner executes strategies in priority order") {
        StrategicPlanner planner;
        
        // Set up conditions for multiple strategies to be applicable
        battle_status.setTestCondition_Threatened(true);  // ThreatResponse applicable
        battle_status.setTestCondition_CanShoot(true);    // DirectEngagement applicable
        
        // Execute strategies
        bool decision_made = planner.executeStrategies(battle_status, &request, &request_title);
        
        // Verify that a decision was made and it was from the highest priority strategy
        REQUIRE(decision_made);
        REQUIRE(request == ActionRequest::Shoot);
        REQUIRE(request_title == "Defensive Counter-Fire");
    }
    
    SECTION("Planner executes next strategy when higher priority strategies pass") {
        // Create planner with customized strategies
        StrategicPlanner planner({"ThreatResponse", "DirectEngagement", "TargetAcquisition"});
        
        // Set up conditions where ThreatResponse doesn't apply but DirectEngagement does
        battle_status.setTestCondition_Threatened(false);  // ThreatResponse not applicable
        battle_status.setTestCondition_CanShoot(true);     // DirectEngagement applicable
        
        // Execute strategies
        bool decision_made = planner.executeStrategies(battle_status, &request, &request_title);
        
        // Verify that DirectEngagement made the decision
        REQUIRE(decision_made);
        REQUIRE(request == ActionRequest::Shoot);
        REQUIRE(request_title == "Direct Target Engagement");
    }
    
    SECTION("Planner returns false when no strategies apply") {
        StrategicPlanner planner;
        
        // Set up conditions where no strategies apply
        battle_status.setTestCondition_Threatened(false);
        battle_status.setTestCondition_CanShoot(false);
        
        // No wall in front for ObstacleBreach
        battle_status.configureTestMap(10, 10);
        
        // Execute strategies
        bool decision_made = planner.executeStrategies(battle_status, &request, &request_title);
        
        // Verify that no decision was made
        REQUIRE_FALSE(decision_made);
        REQUIRE(request == ActionRequest::DoNothing);
    }
    
    SECTION("Planner can be cleared and rebuilt") {
        StrategicPlanner planner({"DirectEngagement"});
        
        // Set up condition for DirectEngagement to apply
        battle_status.setTestCondition_CanShoot(true);
        
        // Execute initial strategy
        bool initial_decision = planner.executeStrategies(battle_status, &request, &request_title);
        REQUIRE(initial_decision);
        REQUIRE(request == ActionRequest::Shoot);
        
        // Clear strategies and verify no decisions are made
        planner.clearStrategies();
        request = ActionRequest::DoNothing;
        bool after_clear = planner.executeStrategies(battle_status, &request, &request_title);
        REQUIRE_FALSE(after_clear);
        
        // Add new strategy and verify it works
        planner.addStrategy("TargetAcquisition");
        battle_status.setTestCondition_CanShoot(false);
        battle_status.setTestCondition_CanShootFromDirection(Direction::Right);
        
        bool new_decision = planner.executeStrategies(battle_status, &request, &request_title);
        REQUIRE(new_decision);
        REQUIRE(request == ActionRequest::RotateRight45 || request == ActionRequest::RotateRight90);
    }
}

/**
 * @brief Test suite for individual tactical strategies
 * 
 * These tests verify that each tactical strategy correctly handles
 * its specific responsibility and decision-making logic.
 */
TEST_CASE_METHOD(StrategicPlannerTestFixture, "Individual strategy tests", "[strategic][tactics]") {
    SECTION("DirectEngagementStrategy correctly identifies firing opportunities") {
        auto strategy = TacticalStrategyFactory::create("DirectEngagement");
        
        // Case 1: Can shoot
        battle_status.setTestCondition_CanShoot(true);
        bool decision1 = strategy->execute(battle_status, &request, &request_title);
        REQUIRE(decision1);
        REQUIRE(request == ActionRequest::Shoot);
        
        // Case 2: Cannot shoot
        battle_status.setTestCondition_CanShoot(false);
        request = ActionRequest::DoNothing;
        bool decision2 = strategy->execute(battle_status, &request, &request_title);
        REQUIRE_FALSE(decision2);
    }
    
    SECTION("TargetAcquisitionStrategy searches for optimal firing directions") {
        auto strategy = TacticalStrategyFactory::create("TargetAcquisition");
        
        // Case 1: No firing direction available
        battle_status.setTestCondition_CanShoot(false);
        battle_status.setTestCondition_CanShootFromDirection(Direction::Up); // Same as current orientation
        bool decision1 = strategy->execute(battle_status, &request, &request_title);
        REQUIRE_FALSE(decision1);
        
        // Case 2: Firing direction available
        battle_status.setTestCondition_CanShootFromDirection(Direction::Right);
        bool decision2 = strategy->execute(battle_status, &request, &request_title);
        REQUIRE(decision2);
        REQUIRE(request == ActionRequest::RotateRight45 || request == ActionRequest::RotateRight90);
    }
    
    SECTION("ThreatResponseStrategy responds to immediate dangers") {
        auto strategy = TacticalStrategyFactory::create("ThreatResponse");
        
        // Case 1: No threat
        battle_status.setTestCondition_Threatened(false);
        bool decision1 = strategy->execute(battle_status, &request, &request_title);
        REQUIRE_FALSE(decision1);
        
        // Case 2: Threat present and can shoot
        battle_status.setTestCondition_Threatened(true);
        battle_status.setTestCondition_CanShoot(true);
        bool decision2 = strategy->execute(battle_status, &request, &request_title);
        REQUIRE(decision2);
        REQUIRE(request == ActionRequest::Shoot);
        REQUIRE(request_title == "Defensive Counter-Fire");
    }
    
    SECTION("ObstacleBreachStrategy clears walls when necessary") {
        auto strategy = TacticalStrategyFactory::create("ObstacleBreach");
        
        // Set up a map with a wall in front
        battle_status.configureTestMap(10, 10);
        Position wall_position = battle_status.normalizeCoordinates(
            battle_status.vehicle_location + battle_status.vehicle_orientation);
        battle_status.setWallAt(wall_position);
        
        // Case 1: Can shoot the wall
        battle_status.setTestCondition_CanShoot(true);
        bool decision1 = strategy->execute(battle_status, &request, &request_title);
        REQUIRE(decision1);
        REQUIRE(request == ActionRequest::Shoot);
        REQUIRE(request_title == "Obstacle Clearance");
        
        // Case 2: Cannot shoot (cooldown)
        battle_status.weapon_cooldown = 2;
        request = ActionRequest::DoNothing;
        bool decision2 = strategy->execute(battle_status, &request, &request_title);
        REQUIRE_FALSE(decision2);
    }
}
