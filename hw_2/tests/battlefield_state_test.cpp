#include <gtest/gtest.h>
#include "../include/GameState.h"
#include "../include/Board.h"
#include "../include/Tank.h"
#include "../include/Wall.h"
#include "../include/Shell.h"
#include <vector>
#include <memory>

/**
 * Comprehensive test suite for the game state management system.
 * Tests state initialization, updates, and serialization/deserialization.
 */
class BattlefieldStateManagementTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create a small battlefield for testing
        battlefield = std::make_unique<Board>(8, 8);
        
        // Initialize the game state
        gameState = std::make_unique<GameState>();
        
        // Add some objects to the board for testing
        playerTank = battlefield->placeObject(std::make_unique<Tank>(Position(2, 2), 1));
        enemyTank = battlefield->placeObject(std::make_unique<Tank>(Position(5, 5), 2));
        
        // Add walls for obstacles
        for (int i = 0; i < 3; i++) {
            walls.push_back(battlefield->placeObject(std::make_unique<Wall>(Position(3, i + 2))));
        }
    }
    
    void TearDown() override {
        // Resources get cleaned up automatically via unique_ptr
    }
    
    std::unique_ptr<Board> battlefield;
    std::unique_ptr<GameState> gameState;
    Tank* playerTank;
    Tank* enemyTank;
    std::vector<Wall*> walls;
};

TEST_F(BattlefieldStateManagementTest, StateInitialization) {
    // Test that the game state initializes with default values
    EXPECT_EQ(gameState->getTurnNumber(), 0);
    EXPECT_FALSE(gameState->isGameOver());
    EXPECT_EQ(gameState->getWinner(), 0);
}

TEST_F(BattlefieldStateManagementTest, StateUpdateTracking) {
    // Test that game state updates properly track game progression
    
    // Simulate a few turns
    for (int i = 0; i < 5; i++) {
        gameState->incrementTurn();
    }
    
    EXPECT_EQ(gameState->getTurnNumber(), 5);
}

TEST_F(BattlefieldStateManagementTest, VictoryConditionDetection) {
    // Test that game state correctly detects when the game should end
    
    // Setup initial conditions - game not over
    EXPECT_FALSE(gameState->isGameOver());
    
    // Simulate player 1 winning
    gameState->setGameOver(1);
    EXPECT_TRUE(gameState->isGameOver());
    EXPECT_EQ(gameState->getWinner(), 1);
    
    // Reset game state
    gameState->reset();
    EXPECT_FALSE(gameState->isGameOver());
    EXPECT_EQ(gameState->getWinner(), 0);
    
    // Simulate player 2 winning
    gameState->setGameOver(2);
    EXPECT_TRUE(gameState->isGameOver());
    EXPECT_EQ(gameState->getWinner(), 2);
}

TEST_F(BattlefieldStateManagementTest, TurnManagement) {
    // Test turn management functionality
    
    // Setup some initial conditions
    gameState->setMaxTurns(100);
    
    // Check initial values
    EXPECT_EQ(gameState->getTurnNumber(), 0);
    EXPECT_EQ(gameState->getMaxTurns(), 100);
    
    // Increment turns and check if we're approaching the limit
    for (int i = 0; i < 98; i++) {
        gameState->incrementTurn();
        EXPECT_FALSE(gameState->isMaxTurnsReached());
    }
    
    // Now at turn 98, increment to 99
    gameState->incrementTurn();
    EXPECT_EQ(gameState->getTurnNumber(), 99);
    EXPECT_FALSE(gameState->isMaxTurnsReached());
    
    // Final turn, should reach max
    gameState->incrementTurn();
    EXPECT_EQ(gameState->getTurnNumber(), 100);
    EXPECT_TRUE(gameState->isMaxTurnsReached());
}

TEST_F(BattlefieldStateManagementTest, PlayerTankTracking) {
    // Test functionality that tracks player tanks and their status
    
    // Add tanks to game state
    gameState->addPlayerTank(playerTank, 1);
    gameState->addPlayerTank(enemyTank, 2);
    
    // Verify tanks were tracked correctly
    EXPECT_EQ(gameState->getPlayerTankCount(1), 1);
    EXPECT_EQ(gameState->getPlayerTankCount(2), 1);
    
    // Simulate destroying a tank
    enemyTank->destroy();
    gameState->updateTankStatus();
    
    // Verify the tracking updated
    EXPECT_EQ(gameState->getPlayerTankCount(1), 1);
    EXPECT_EQ(gameState->getPlayerTankCount(2), 0);
    
    // Verify game over condition is detected
    EXPECT_TRUE(gameState->isGameOver());
    EXPECT_EQ(gameState->getWinner(), 1);
}

TEST_F(BattlefieldStateManagementTest, StateSnapshot) {
    // Test ability to take a snapshot of the game state for replay/analysis
    
    // Setup initial conditions
    gameState->setMaxTurns(50);
    gameState->incrementTurn();  // Turn 1
    gameState->incrementTurn();  // Turn 2
    
    // Add player tanks to track
    gameState->addPlayerTank(playerTank, 1);
    gameState->addPlayerTank(enemyTank, 2);
    
    // Create a snapshot
    GameSnapshot snapshot = gameState->createSnapshot();
    
    // Verify snapshot contains expected data
    EXPECT_EQ(snapshot.turnNumber, 2);
    EXPECT_EQ(snapshot.maxTurns, 50);
    EXPECT_FALSE(snapshot.gameOver);
    
    // Make changes to game state
    gameState->incrementTurn();  // Turn 3
    enemyTank->destroy();
    gameState->updateTankStatus();
    
    // Verify game state changed
    EXPECT_EQ(gameState->getTurnNumber(), 3);
    EXPECT_TRUE(gameState->isGameOver());
    
    // Restore from snapshot
    gameState->restoreFromSnapshot(snapshot);
    
    // Verify restoration worked
    EXPECT_EQ(gameState->getTurnNumber(), 2);
    EXPECT_EQ(gameState->getMaxTurns(), 50);
    EXPECT_FALSE(gameState->isGameOver());
}

TEST_F(BattlefieldStateManagementTest, GameStateSerialization) {
    // Test serialization of game state to string format
    
    // Setup initial state
    gameState->setMaxTurns(75);
    gameState->incrementTurn();  // Turn 1
    gameState->incrementTurn();  // Turn 2
    
    gameState->addPlayerTank(playerTank, 1);
    gameState->addPlayerTank(enemyTank, 2);
    
    // Serialize the state
    std::string serialized = gameState->serialize();
    
    // Create a new game state and deserialize
    auto newState = std::make_unique<GameState>();
    newState->deserialize(serialized);
    
    // Verify deserialized state matches original
    EXPECT_EQ(newState->getTurnNumber(), gameState->getTurnNumber());
    EXPECT_EQ(newState->getMaxTurns(), gameState->getMaxTurns());
    EXPECT_EQ(newState->isGameOver(), gameState->isGameOver());
    EXPECT_EQ(newState->getWinner(), gameState->getWinner());
}
