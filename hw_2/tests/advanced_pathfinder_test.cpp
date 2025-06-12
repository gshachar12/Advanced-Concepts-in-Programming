#include "catch.hpp"

#include "../src/algorithms/AdvancedPathfinder.h"
#include "../src/algorithms/MyBattleStatus.h"
#include "../src/Position.h"
#include "../src/Direction.h"

/**
 * @brief Test suite for the AdvancedPathfinder algorithm
 * 
 * This test suite validates key behaviors of the AdvancedPathfinder algorithm.
 * The tests ensure that the pathfinding logic, targeting mechanisms, and 
 * tactical decision-making all function as expected under various conditions.
 */
TEST_CASE("AdvancedPathfinder functionality tests", "[tactical][pathfinding]") {
    SECTION("Algorithm correctly identifies itself") {
        AdvancedPathfinder pathfinder(1, 0);
        REQUIRE(pathfinder.getName() == "AdvancedPathfinder");
    }
    
    SECTION("Algorithm performs battlefield reconnaissance on first turn") {
        // Setup
        AdvancedPathfinder pathfinder(1, 0);
        auto& battle_status = pathfinder.getBattleStatus();
        battle_status.current_turn = 0;  // First turn
        
        // Execute
        ActionRequest request = ActionRequest::DoNothing;
        std::string request_title;
        pathfinder.calculateAction(&request, &request_title);
        
        // Verify
        REQUIRE(request == ActionRequest::GetBattleInfo);
        REQUIRE(request_title.find("surveillance") != std::string::npos);
    }
    
    SECTION("Algorithm responds to threats with appropriate actions") {
        // Setup a scenario with a nearby threat
        AdvancedPathfinder pathfinder(2, 1);
        auto& battle_status = pathfinder.getBattleStatus();
        battle_status.current_turn = 5;  // Not first turn
        
        // Configure mock battlefield with enemy in dangerous proximity
        battle_status.setTestCondition_Threatened(true);
        battle_status.setTestCondition_CanShoot(true);
        
        // Execute tactical decision
        ActionRequest request = ActionRequest::DoNothing;
        std::string request_title;
        pathfinder.calculateAction(&request, &request_title);
        
        // Verify response is aggressive when threat can be eliminated
        REQUIRE(request == ActionRequest::Shoot);
        REQUIRE(request_title.find("Counter-Attack") != std::string::npos);
        
        // Change scenario - can't shoot the threat
        battle_status.setTestCondition_CanShoot(false);
        pathfinder.calculateAction(&request, &request_title);
        
        // Verify response is evasive when threat can't be eliminated
        REQUIRE(request != ActionRequest::Shoot);
        REQUIRE(request_title.find("Evasion") != std::string::npos);
    }
    
    SECTION("Algorithm can calculate optimal routes to firing positions") {
        // Setup
        AdvancedPathfinder pathfinder(1, 0);
        auto& battle_status = pathfinder.getBattleStatus();
        battle_status.current_turn = 3;
        battle_status.arena_width = 10;
        battle_status.arena_height = 10;
        battle_status.vehicle_location = {5, 5};
        battle_status.vehicle_orientation = Direction::Up;
        
        // Mock scenario where enemy is reachable via path
        battle_status.setTestCondition_Threatened(false);
        battle_status.setTestCondition_CanShoot(false);
        battle_status.configureTestMap(10, 10);
        
        // Position enemy at a location that requires navigation
        battle_status.addTestEnemy({8, 8});
        
        // Execute tactical decision multiple times to allow path calculation
        ActionRequest request = ActionRequest::DoNothing;
        std::string request_title;
        
        // First action should be to get battle info since we haven't done so yet
        pathfinder.calculateAction(&request, &request_title);
        REQUIRE(request == ActionRequest::GetBattleInfo);
        
        // Mark that we've obtained battle info
        battle_status.last_requested_info_turn = battle_status.current_turn;
        
        // Next action should be to start navigation
        pathfinder.calculateAction(&request, &request_title);
        
        // Over several turns, the algorithm should navigate toward the firing position
        bool navigation_started = false;
        for (int i = 0; i < 10 && !navigation_started; i++) {
            if (request == ActionRequest::MoveForward || 
                request == ActionRequest::RotateRight45 || 
                request == ActionRequest::RotateLeft45) {
                navigation_started = true;
            } else {
                pathfinder.calculateAction(&request, &request_title);
            }
        }
        
        REQUIRE(navigation_started);
        REQUIRE(request_title.find("grid") != std::string::npos || 
                request_title.find("orientation") != std::string::npos);
    }
    
    SECTION("Algorithm correctly handles obstacles in path") {
        // Setup
        AdvancedPathfinder pathfinder(1, 0);
        auto& battle_status = pathfinder.getBattleStatus();
        battle_status.current_turn = 3;
        battle_status.arena_width = 5;
        battle_status.arena_height = 5;
        battle_status.vehicle_location = {2, 2};
        battle_status.vehicle_orientation = Direction::Up;
        
        // Configure a map with a wall ahead
        battle_status.setTestCondition_Threatened(false);
        battle_status.configureTestMap(5, 5);
        battle_status.setWallAt({2, 1});  // Wall directly ahead
        battle_status.setTestCondition_CanShoot(true);  // Can shoot the wall
        battle_status.last_requested_info_turn = battle_status.current_turn;
        
        // Execute decision with wall ahead and ready weapon
        ActionRequest request = ActionRequest::DoNothing;
        std::string request_title;
        
        // Simulate route determination
        battle_status.setupTestRoute({Direction::Up});
        
        // Calculate action - should decide to shoot the wall
        pathfinder.calculateAction(&request, &request_title);
        
        // Verify appropriate obstacle handling
        REQUIRE(request == ActionRequest::Shoot);
        REQUIRE(request_title.find("barrier") != std::string::npos);
        
        // Change scenario - weapon not ready
        battle_status.setTestCondition_CanShoot(false);
        pathfinder.calculateAction(&request, &request_title);
        
        // Verify waiting behavior when can't clear obstacle
        REQUIRE(request == ActionRequest::DoNothing);
        REQUIRE(request_title.find("awaiting") != std::string::npos);
    }
}

/**
 * @brief Test suite for battlefield state interactions
 * 
 * This suite validates how the AdvancedPathfinder interacts with the
 * battlefield state information, including target tracking and
 * tactical reassessments.
 */
TEST_CASE("AdvancedPathfinder battlefield awareness tests", "[tactical][awareness]") {
    SECTION("Algorithm updates intelligence when enemy positions change") {
        // Setup
        AdvancedPathfinder pathfinder(1, 0);
        auto& battle_status = pathfinder.getBattleStatus();
        battle_status.current_turn = 5;
        battle_status.arena_width = 10;
        battle_status.arena_height = 10;
        battle_status.last_requested_info_turn = 4;  // Info is outdated
        
        // Execute decision
        ActionRequest request = ActionRequest::DoNothing;
        std::string request_title;
        pathfinder.calculateAction(&request, &request_title);
        
        // Verify intelligence update requested
        REQUIRE(request == ActionRequest::GetBattleInfo);
        REQUIRE(request_title.find("intelligence") != std::string::npos);
    }
    
    SECTION("Algorithm engages targets of opportunity") {
        // Setup
        AdvancedPathfinder pathfinder(1, 0);
        auto& battle_status = pathfinder.getBattleStatus();
        battle_status.current_turn = 5;
        battle_status.arena_width = 10;
        battle_status.arena_height = 10;
        battle_status.vehicle_location = {5, 5};
        battle_status.vehicle_orientation = Direction::Up;
        battle_status.last_requested_info_turn = 5;  // Info is current
        
        // Configure a scenario with an enemy in sight
        battle_status.setTestCondition_Threatened(false);
        battle_status.setTestCondition_CanShoot(true);  // Can hit enemy
        battle_status.configureTestMap(10, 10);
        battle_status.addTestEnemy({5, 2});  // Enemy straight ahead
        
        // Execute decision
        ActionRequest request = ActionRequest::DoNothing;
        std::string request_title;
        pathfinder.calculateAction(&request, &request_title);
        
        // Verify engagement
        REQUIRE(request == ActionRequest::Shoot);
        REQUIRE(request_title == "Target Engagement");
    }
    
    SECTION("Algorithm adjusts aim to acquire targets") {
        // Setup
        AdvancedPathfinder pathfinder(1, 0);
        auto& battle_status = pathfinder.getBattleStatus();
        battle_status.current_turn = 5;
        battle_status.arena_width = 10;
        battle_status.arena_height = 10;
        battle_status.vehicle_location = {5, 5};
        battle_status.vehicle_orientation = Direction::Up;
        battle_status.last_requested_info_turn = 5;  // Info is current
        
        // Configure a scenario with an enemy to the right
        battle_status.setTestCondition_Threatened(false);
        battle_status.setTestCondition_CanShoot(false);  // Can't hit enemy from current orientation
        battle_status.configureTestMap(10, 10);
        battle_status.addTestEnemy({8, 5});  // Enemy to the right
        
        // Set up the test to indicate target can be hit from Right orientation
        battle_status.setTestCondition_CanShootFromDirection(Direction::Right);
        
        // Execute decision
        ActionRequest request = ActionRequest::DoNothing;
        std::string request_title;
        pathfinder.calculateAction(&request, &request_title);
        
        // Verify rotation to acquire target
        REQUIRE(request == ActionRequest::RotateRight45 || request == ActionRequest::RotateRight90);
        REQUIRE(request_title.find("Adjusting aim") != std::string::npos);
    }
}
