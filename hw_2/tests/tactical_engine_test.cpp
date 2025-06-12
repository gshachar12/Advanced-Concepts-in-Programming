#include "catch.hpp"

#include "../src/algorithms/TacticalEngine.h"
#include "../src/algorithms/MyBattleStatus.h"
#include "../src/Position.h"
#include "../src/Direction.h"

/**
 * @brief Test suite for the TacticalEngine algorithm
 * 
 * This test suite validates key behaviors of the TacticalEngine algorithm,
 * ensuring that it correctly leverages the Strategy Pattern for tactical
 * decision making.
 */
TEST_CASE("TacticalEngine functionality tests", "[tactical][strategy]") {
    SECTION("Algorithm correctly identifies itself") {
        TacticalEngine engine(1, 0);
        REQUIRE(engine.getName() == "TacticalEngine");
    }
    
    SECTION("Algorithm performs reconnaissance on first turn") {
        // Setup
        TacticalEngine engine(1, 0);
        auto& battle_status = engine.getBattleStatus();
        battle_status.current_turn = 0;  // First turn
        
        // Execute
        ActionRequest request = ActionRequest::DoNothing;
        std::string request_title;
        engine.calculateAction(&request, &request_title);
        
        // Verify
        REQUIRE(request == ActionRequest::GetBattleInfo);
        REQUIRE(request_title.find("reconnaissance") != std::string::npos);
    }
    
    SECTION("Algorithm responds to threats with appropriate actions") {
        // Setup a scenario with a threat
        TacticalEngine engine(1, 0);
        auto& battle_status = engine.getBattleStatus();
        battle_status.current_turn = 5;
        
        // Configure threat scenario with firing capability
        battle_status.setTestCondition_Threatened(true);
        battle_status.setTestCondition_CanShoot(true);
        
        // Execute decision
        ActionRequest request = ActionRequest::DoNothing;
        std::string request_title;
        engine.calculateAction(&request, &request_title);
        
        // Verify response is to shoot the threat
        REQUIRE(request == ActionRequest::Shoot);
        REQUIRE(request_title.find("Counter") != std::string::npos);
    }
    
    SECTION("Algorithm engages targets when possible") {
        // Setup
        TacticalEngine engine(1, 0);
        auto& battle_status = engine.getBattleStatus();
        battle_status.current_turn = 3;
        battle_status.last_requested_info_turn = 3;  // Info is current
        
        // Configure a scenario with a shootable target
        battle_status.setTestCondition_Threatened(false);
        battle_status.setTestCondition_CanShoot(true);
        
        // Execute decision
        ActionRequest request = ActionRequest::DoNothing;
        std::string request_title;
        engine.calculateAction(&request, &request_title);
        
        // Verify engagement
        REQUIRE(request == ActionRequest::Shoot);
        REQUIRE(request_title.find("Target") != std::string::npos || 
                request_title.find("Engagement") != std::string::npos);
    }
    
    SECTION("Algorithm rotates to acquire targets") {
        // Setup
        TacticalEngine engine(1, 0);
        auto& battle_status = engine.getBattleStatus();
        battle_status.current_turn = 3;
        battle_status.last_requested_info_turn = 3;
        battle_status.vehicle_orientation = Direction::Up;
        
        // Configure a scenario with a target that requires rotation
        battle_status.setTestCondition_Threatened(false);
        battle_status.setTestCondition_CanShoot(false);
        battle_status.setTestCondition_CanShootFromDirection(Direction::Right);
        
        // Execute decision
        ActionRequest request = ActionRequest::DoNothing;
        std::string request_title;
        engine.calculateAction(&request, &request_title);
        
        // Verify rotation action
        REQUIRE(request == ActionRequest::RotateRight45 || request == ActionRequest::RotateRight90);
        REQUIRE(request_title.find("Tactical") != std::string::npos || 
                request_title.find("Aligning") != std::string::npos);
    }
    
    SECTION("Algorithm initiates navigation when no direct actions are possible") {
        // Setup
        TacticalEngine engine(1, 0);
        auto& battle_status = engine.getBattleStatus();
        battle_status.current_turn = 3;
        battle_status.last_requested_info_turn = 3;
        battle_status.arena_width = 10;
        battle_status.arena_height = 10;
        battle_status.vehicle_location = {5, 5};
        battle_status.vehicle_orientation = Direction::Up;
        
        // Configure a scenario with no immediate action possible
        battle_status.setTestCondition_Threatened(false);
        battle_status.setTestCondition_CanShoot(false);
        battle_status.configureTestMap(10, 10);
        
        // Add enemy at a position requiring navigation
        battle_status.addTestEnemy({8, 8});
        
        // Execute multiple decisions to allow path calculation
        ActionRequest request = ActionRequest::DoNothing;
        std::string request_title;
        
        // First decision should start navigation or movement
        engine.calculateAction(&request, &request_title);
        
        // If the engine needs more setup, give it another cycle
        if (request == ActionRequest::GetBattleInfo) {
            battle_status.last_requested_info_turn = battle_status.current_turn + 1;
            engine.calculateAction(&request, &request_title);
        }
        
        // Verify navigation action
        bool is_navigation_action = 
            (request == ActionRequest::MoveForward) ||
            (request == ActionRequest::RotateLeft45) ||
            (request == ActionRequest::RotateRight45) ||
            (request == ActionRequest::RotateLeft90) ||
            (request == ActionRequest::RotateRight90);
            
        REQUIRE(is_navigation_action);
    }
    
    SECTION("Algorithm handles obstacles correctly") {
        // Setup
        TacticalEngine engine(1, 0);
        auto& battle_status = engine.getBattleStatus();
        battle_status.current_turn = 3;
        battle_status.last_requested_info_turn = 3;
        battle_status.arena_width = 5;
        battle_status.arena_height = 5;
        battle_status.vehicle_location = {2, 2};
        battle_status.vehicle_orientation = Direction::Up;
        
        // Configure a map with a wall ahead
        battle_status.setTestCondition_Threatened(false);
        battle_status.configureTestMap(5, 5);
        battle_status.setWallAt({2, 1});  // Wall directly ahead
        battle_status.setTestCondition_CanShoot(true);
        
        // Execute decision
        ActionRequest request = ActionRequest::DoNothing;
        std::string request_title;
        engine.calculateAction(&request, &request_title);
        
        // Verify obstacle handling (should shoot or navigate around)
        bool appropriate_action = 
            (request == ActionRequest::Shoot && request_title.find("obstacle") != std::string::npos) ||
            (request == ActionRequest::RotateLeft45) ||
            (request == ActionRequest::RotateRight45);
            
        REQUIRE(appropriate_action);
    }
}
