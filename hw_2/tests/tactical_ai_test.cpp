#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../include/Direction.h"
#include "../src/algorithms/SimpleAlgorithm.h"
#include "../src/algorithms/BfsAlgorithm.h"
#include "../include/MyBattleInfo.h"
#include "../include/Board.h"
#include "../include/Tank.h"
#include "../include/Shell.h"
#include "../include/Wall.h"
#include "../include/Mine.h"

/**
 * This test suite evaluates the core AI decision-making components used by tank agents.
 * It ensures that tanks respond appropriately to various battlefield scenarios.
 */
class TacticalAITest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create a battlefield for testing AI behaviors
        battlefield = new Board(15, 15);
        
        // Position the player tanks with initial orientations
        playerTank = battlefield->placeObject(std::make_unique<Tank>(Position(3, 3), Direction::RIGHT, 1));
        enemyTank = battlefield->placeObject(std::make_unique<Tank>(Position(11, 11), Direction::LEFT, 2));
        
        // Initialize the algorithms under test
        tacticalAlgorithm = new SimpleAlgorithm(1, 0);
        pathfindingAlgorithm = new BfsAlgorithm(1, 0);
        
        // Make algorithms aware of the battlefield
        updateAlgorithmState();
    }
    
    void TearDown() override {
        delete battlefield;
        delete tacticalAlgorithm;
        delete pathfindingAlgorithm;
    }
    
    // Update algorithm's knowledge of the battlefield
    void updateAlgorithmState() {
        // Create battle info from current state
        auto battleInfo = std::make_unique<MyBattleInfo>(battlefield);
        
        // Update algorithm's awareness of the battlefield
        if (tacticalAlgorithm) {
            tacticalAlgorithm->updateBattleInfo(*battleInfo);
        }
        
        if (pathfindingAlgorithm) {
            pathfindingAlgorithm->updateBattleInfo(*battleInfo);
        }
    }
    
    // Helper to place walls in various patterns
    void createWallBarrier(int startX, int startY, int length, bool isHorizontal) {
        for (int i = 0; i < length; i++) {
            if (isHorizontal) {
                battlefield->placeObject(std::make_unique<Wall>(Position(startX + i, startY)));
            } else {
                battlefield->placeObject(std::make_unique<Wall>(Position(startX, startY + i)));
            }
        }
        updateAlgorithmState();
    }
    
    // Helper to place a projectile on the board
    Shell* placeProjectile(int x, int y, Direction::DirectionType direction, int ownerId) {
        auto shell = battlefield->placeObject(std::make_unique<Shell>(
            Position(x, y), direction, ownerId
        ));
        updateAlgorithmState();
        return shell;
    }
    
    // Test components
    Board* battlefield;
    Tank* playerTank;
    Tank* enemyTank;
    SimpleAlgorithm* tacticalAlgorithm;
    BfsAlgorithm* pathfindingAlgorithm;
};

TEST_F(TacticalAITest, SimpleAlgorithmBasicBehavior) {
    // Test that the simple algorithm requests battle info on first turn
    ActionRequest request;
    std::string requestDesc;
    tacticalAlgorithm->calculateAction(&request, &requestDesc);
    
    // First turn should request battlefield information
    EXPECT_EQ(request, ActionRequest::GetBattleInfo) 
        << "Simple algorithm should request battlefield info on first turn";
}

TEST_F(TacticalAITest, DetectAndRespondToThreats) {
    // Place a shell near the player tank to create a threat
    placeProjectile(4, 3, Direction::LEFT, 2);  // Enemy shell approaching
    updateAlgorithmState();
    
    // Algorithm should detect the threat and respond
    ActionRequest request;
    std::string requestDesc;
    tacticalAlgorithm->calculateAction(&request, &requestDesc);
    
    // The algorithm should try to move away from the shell
    EXPECT_NE(request, ActionRequest::DoNothing) 
        << "Algorithm should take evasive action when threatened";
    EXPECT_NE(request, ActionRequest::GetBattleInfo)
        << "Algorithm should prioritize action over information when threatened";
}

TEST_F(TacticalAITest, EngageEnemyWhenPossible) {
    // Position tanks to be in line of sight
    delete playerTank;
    delete enemyTank;
    
    // Place tanks in direct line of sight
    playerTank = battlefield->placeObject(std::make_unique<Tank>(
        Position(7, 7), Direction::RIGHT, 1
    ));
    enemyTank = battlefield->placeObject(std::make_unique<Tank>(
        Position(10, 7), Direction::LEFT, 2
    ));
    updateAlgorithmState();
    
    // Configure the tank with ammunition
    tacticalAlgorithm->getBattleStatus().ammo_count = 5;
    tacticalAlgorithm->getBattleStatus().weapon_cooldown = 0;
    
    // Get the algorithm's decision
    ActionRequest request;
    std::string requestDesc;
    tacticalAlgorithm->calculateAction(&request, &requestDesc);
    
    // Should choose to shoot when enemy is in direct line of fire
    EXPECT_EQ(request, ActionRequest::Shoot)
        << "Algorithm should shoot when enemy is in line of sight";
}

TEST_F(TacticalAITest, NavigateAroundObstacles) {
    // Create a wall barrier between tanks
    createWallBarrier(5, 3, 5, true);  // Horizontal wall
    updateAlgorithmState();
    
    // Configure the pathfinding algorithm for testing
    pathfindingAlgorithm->getBattleStatus().ammo_count = 3;
    pathfindingAlgorithm->getBattleStatus().weapon_cooldown = 0;
    
    bool rotationObserved = false;
    bool movementObserved = false;
    
    // Monitor several turns of pathfinding behavior
    for (int i = 0; i < 10; i++) {
        ActionRequest request;
        std::string requestDesc;
        
        // Skip the first turn which typically just gathers info
        if (i == 0) {
            pathfindingAlgorithm->calculateAction(&request, &requestDesc);
            continue;
        }
        
        // Get the algorithm's decision
        pathfindingAlgorithm->calculateAction(&request, &requestDesc);
        
        // Track if we observed rotation or movement
        switch (request) {
            case ActionRequest::RotateLeft45:
            case ActionRequest::RotateRight45:
            case ActionRequest::RotateLeft90:
            case ActionRequest::RotateRight90:
                rotationObserved = true;
                break;
            case ActionRequest::MoveForward:
                movementObserved = true;
                break;
            default:
                break;
        }
        
        // Update state for next iteration
        pathfindingAlgorithm->getBattleStatus().updateBattleStatusBaseAction(request);
    }
    
    // The pathfinding algorithm should attempt to navigate around obstacles
    EXPECT_TRUE(rotationObserved || movementObserved)
        << "Pathfinding algorithm should attempt to navigate around obstacles";
}

TEST_F(TacticalAITest, WeaponCooldownManagement) {
    // Position tanks to be in line of sight
    delete playerTank;
    delete enemyTank;
    playerTank = battlefield->placeObject(std::make_unique<Tank>(
        Position(7, 7), Direction::RIGHT, 1
    ));
    enemyTank = battlefield->placeObject(std::make_unique<Tank>(
        Position(10, 7), Direction::LEFT, 2
    ));
    updateAlgorithmState();
    
    // Set initial state - ready to fire
    tacticalAlgorithm->getBattleStatus().ammo_count = 5;
    tacticalAlgorithm->getBattleStatus().weapon_cooldown = 0;
    
    // First action should be to shoot
    ActionRequest request1;
    std::string requestDesc1;
    tacticalAlgorithm->calculateAction(&request1, &requestDesc1);
    EXPECT_EQ(request1, ActionRequest::Shoot);
    
    // Apply the effect of shooting
    tacticalAlgorithm->getBattleStatus().updateBattleStatusBaseAction(ActionRequest::Shoot);
    
    // Next action should NOT be to shoot due to cooldown
    ActionRequest request2;
    std::string requestDesc2;
    tacticalAlgorithm->calculateAction(&request2, &requestDesc2);
    EXPECT_NE(request2, ActionRequest::Shoot) 
        << "Algorithm should not attempt to shoot during weapon cooldown";
}
