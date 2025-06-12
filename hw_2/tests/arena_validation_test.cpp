#include <gtest/gtest.h>
#include "../include/Board.h"
#include "../include/Tank.h"
#include "../include/Wall.h"
#include "../include/Mine.h"

/**
 * Test suite for validating the Arena (Board) functionality.
 * These tests verify the correctness of the game arena's core behaviors.
 */
class ArenaValidationTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Initialize a rectangular arena for testing
        testArena = new Board(12, 9);
    }

    void TearDown() override {
        // Clean up resources after each test
        delete testArena;
    }

    Board *testArena;
};

TEST_F(ArenaValidationTest, VerifyArenaDimensions) {
    // Ensure arena dimensions match what was specified during initialization
    ASSERT_EQ(testArena->getWidth(), 12) << "Arena width does not match expected value";
    ASSERT_EQ(testArena->getHeight(), 9) << "Arena height does not match expected value";
}

TEST_F(ArenaValidationTest, CoordinateWrappingBehavior) {
    // Test coordinate normalization in various scenarios
    
    // Case 1: Horizontal wrapping (positive overflow)
    Position overflowRight(15, 3);
    Position normalizedRight = testArena->wrapPosition(overflowRight);
    EXPECT_EQ(normalizedRight.x, 3) << "Failed to wrap x-coordinate correctly";
    EXPECT_EQ(normalizedRight.y, 3) << "Y-coordinate should remain unchanged";
    
    // Case 2: Horizontal wrapping (negative underflow)
    Position underflowLeft(-4, 5);
    Position normalizedLeft = testArena->wrapPosition(underflowLeft);
    EXPECT_EQ(normalizedLeft.x, 8) << "Failed to wrap negative x-coordinate correctly";
    EXPECT_EQ(normalizedLeft.y, 5) << "Y-coordinate should remain unchanged";
    
    // Case 3: Vertical wrapping (positive overflow)
    Position overflowBottom(6, 12);
    Position normalizedBottom = testArena->wrapPosition(overflowBottom);
    EXPECT_EQ(normalizedBottom.x, 6) << "X-coordinate should remain unchanged";
    EXPECT_EQ(normalizedBottom.y, 3) << "Failed to wrap y-coordinate correctly";
    
    // Case 4: Vertical wrapping (negative underflow)
    Position overflowTop(2, -3);
    Position normalizedTop = testArena->wrapPosition(overflowTop);
    EXPECT_EQ(normalizedTop.x, 2) << "X-coordinate should remain unchanged";
    EXPECT_EQ(normalizedTop.y, 6) << "Failed to wrap negative y-coordinate correctly";
    
    // Case 5: Diagonal wrapping (both coordinates out of bounds)
    Position diagonalOverflow(14, 11);
    Position normalizedDiagonal = testArena->wrapPosition(diagonalOverflow);
    EXPECT_EQ(normalizedDiagonal.x, 2) << "Failed to wrap x-coordinate in diagonal case";
    EXPECT_EQ(normalizedDiagonal.y, 2) << "Failed to wrap y-coordinate in diagonal case";
}

TEST_F(ArenaValidationTest, PlaceAndRemoveObjects) {
    // Test adding and removing game objects from the arena
    
    // Place a tank and verify its position
    auto* tank = testArena->placeObject(std::make_unique<Tank>(Position(3, 4), 1));
    ASSERT_NE(tank, nullptr) << "Failed to place tank in arena";
    EXPECT_EQ(tank->getPosition().x, 3);
    EXPECT_EQ(tank->getPosition().y, 4);
    
    // Place a wall and verify its position
    auto* wall = testArena->placeObject(std::make_unique<Wall>(Position(5, 6)));
    ASSERT_NE(wall, nullptr) << "Failed to place wall in arena";
    EXPECT_EQ(wall->getPosition().x, 5);
    EXPECT_EQ(wall->getPosition().y, 6);
    
    // Verify cell occupancy
    EXPECT_EQ(testArena->getCellType(3, 4), CellType::TANK);
    EXPECT_EQ(testArena->getCellType(5, 6), CellType::WALL);
    
    // Remove objects and verify cells are empty
    testArena->removeObject(tank);
    testArena->removeObject(wall);
    EXPECT_EQ(testArena->getCellType(3, 4), CellType::EMPTY);
    EXPECT_EQ(testArena->getCellType(5, 6), CellType::EMPTY);
}

TEST_F(ArenaValidationTest, CollisionDetection) {
    // Place multiple objects and test collision detection
    
    // Place a wall
    auto* wall = testArena->placeObject(std::make_unique<Wall>(Position(7, 3)));
    
    // Check if position is occupied
    EXPECT_TRUE(testArena->isPositionOccupied(7, 3));
    EXPECT_FALSE(testArena->isPositionOccupied(7, 4));
    
    // Add a tank in a different position
    auto* tank = testArena->placeObject(std::make_unique<Tank>(Position(9, 3), 1));
    EXPECT_TRUE(testArena->isPositionOccupied(9, 3));
    
    // Try adding a mine to an occupied position (should fail)
    auto minePtr = std::make_unique<Mine>(Position(7, 3));
    auto* mine = testArena->placeObject(std::move(minePtr));
    EXPECT_EQ(mine, nullptr) << "Should not be able to place object in occupied cell";
    
    // Add mine to unoccupied position (should succeed)
    mine = testArena->placeObject(std::make_unique<Mine>(Position(5, 5)));
    ASSERT_NE(mine, nullptr);
    EXPECT_TRUE(testArena->isPositionOccupied(5, 5));
}

TEST_F(ArenaValidationTest, ObjectIteration) {
    // Test iteration over all game objects in the arena
    
    // Add various objects
    testArena->placeObject(std::make_unique<Tank>(Position(1, 1), 1));
    testArena->placeObject(std::make_unique<Tank>(Position(10, 8), 2));
    testArena->placeObject(std::make_unique<Wall>(Position(5, 5)));
    testArena->placeObject(std::make_unique<Wall>(Position(6, 5)));
    testArena->placeObject(std::make_unique<Mine>(Position(3, 7)));
    
    // Count objects by type
    int tankCount = 0;
    int wallCount = 0;
    int mineCount = 0;
    
    for (const auto& obj : testArena->getGameObjects()) {
        if (obj->getCellType() == CellType::TANK) {
            tankCount++;
        } else if (obj->getCellType() == CellType::WALL) {
            wallCount++;
        } else if (obj->getCellType() == CellType::MINE) {
            mineCount++;
        }
    }
    
    // Verify counts
    EXPECT_EQ(tankCount, 2) << "Incorrect number of tanks";
    EXPECT_EQ(wallCount, 2) << "Incorrect number of walls";
    EXPECT_EQ(mineCount, 1) << "Incorrect number of mines";
}
