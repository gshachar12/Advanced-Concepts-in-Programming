#include <gtest/gtest.h>
#include "../include/Direction.h"
#include "../include/Position.h"

/**
 * Comprehensive test suite for the movement-related functionality in the game.
 * These tests validate position calculations, direction changes, and coordinate transformations.
 */
class MovementSystemTest : public ::testing::Test {
protected:
    // Test vectors for common directional movements
    std::vector<Direction::DirectionType> allDirections = {
        Direction::UP, Direction::UP_RIGHT, Direction::RIGHT, Direction::DOWN_RIGHT, 
        Direction::DOWN, Direction::DOWN_LEFT, Direction::LEFT, Direction::UP_LEFT
    };
    
    std::vector<std::string> directionNames = {
        "UP", "UP_RIGHT", "RIGHT", "DOWN_RIGHT", "DOWN", "DOWN_LEFT", "LEFT", "UP_LEFT"
    };
};

TEST_F(MovementSystemTest, CoordinateArithmetic) {
    // Test basic position arithmetic operations
    Position p1(5, 7);
    Position p2(3, 2);
    
    // Addition
    Position sum = p1 + p2;
    EXPECT_EQ(sum.x, 8);
    EXPECT_EQ(sum.y, 9);
    
    // Subtraction
    Position diff = p1 - p2;
    EXPECT_EQ(diff.x, 2);
    EXPECT_EQ(diff.y, 5);
    
    // Equality operators
    Position p3(5, 7);
    EXPECT_TRUE(p1 == p3);
    EXPECT_FALSE(p1 == p2);
    
    // Inequality operators
    EXPECT_TRUE(p1 != p2);
    EXPECT_FALSE(p1 != p3);
}

TEST_F(MovementSystemTest, PositionConstruction) {
    // Default construction
    Position defaultPos;
    EXPECT_EQ(defaultPos.x, 0);
    EXPECT_EQ(defaultPos.y, 0);
    
    // Standard construction
    Position pos(15, 23);
    EXPECT_EQ(pos.x, 15);
    EXPECT_EQ(pos.y, 23);
    
    // Copy construction
    Position original(7, 9);
    Position copy(original);
    EXPECT_EQ(copy.x, 7);
    EXPECT_EQ(copy.y, 9);
    
    // Assignment operation
    Position assigned = original;
    EXPECT_EQ(assigned.x, 7);
    EXPECT_EQ(assigned.y, 9);
}

TEST_F(MovementSystemTest, DirectionToOffsetConversion) {
    // Test that each direction correctly translates to the right offset
    std::vector<std::pair<int, int>> expectedOffsets = {
        {0, -1},  // UP
        {1, -1},  // UP_RIGHT
        {1, 0},   // RIGHT
        {1, 1},   // DOWN_RIGHT
        {0, 1},   // DOWN
        {-1, 1},  // DOWN_LEFT
        {-1, 0},  // LEFT
        {-1, -1}  // UP_LEFT
    };
    
    // Verify each direction produces the expected offset
    for (size_t i = 0; i < allDirections.size(); i++) {
        auto [dx, dy] = Direction::getDirectionOffset(allDirections[i]);
        EXPECT_EQ(dx, expectedOffsets[i].first) << "Failed on direction: " << directionNames[i];
        EXPECT_EQ(dy, expectedOffsets[i].second) << "Failed on direction: " << directionNames[i];
    }
}

TEST_F(MovementSystemTest, PositionMovementInAllDirections) {
    // Test moving a position in all 8 directions
    Position center(10, 10);
    
    // Expected positions after moving in each direction
    std::vector<Position> expectedPositions = {
        {10, 9},  // UP
        {11, 9},  // UP_RIGHT
        {11, 10}, // RIGHT
        {11, 11}, // DOWN_RIGHT
        {10, 11}, // DOWN
        {9, 11},  // DOWN_LEFT
        {9, 10},  // LEFT
        {9, 9}    // UP_LEFT
    };
    
    // Test movements
    for (size_t i = 0; i < allDirections.size(); i++) {
        Position result = center + allDirections[i];
        EXPECT_EQ(result.x, expectedPositions[i].x) 
            << "Failed X coordinate on direction: " << directionNames[i];
        EXPECT_EQ(result.y, expectedPositions[i].y) 
            << "Failed Y coordinate on direction: " << directionNames[i];
    }
}

TEST_F(MovementSystemTest, DirectionRotationOperations) {
    // Test rotation from one direction to another
    
    // Test 45 degree rotations
    EXPECT_EQ(Direction::rotateClockwise45(Direction::UP), Direction::UP_RIGHT);
    EXPECT_EQ(Direction::rotateClockwise45(Direction::RIGHT), Direction::DOWN_RIGHT);
    EXPECT_EQ(Direction::rotateClockwise45(Direction::DOWN), Direction::DOWN_LEFT);
    EXPECT_EQ(Direction::rotateClockwise45(Direction::LEFT), Direction::UP_LEFT);
    
    EXPECT_EQ(Direction::rotateCounterClockwise45(Direction::UP), Direction::UP_LEFT);
    EXPECT_EQ(Direction::rotateCounterClockwise45(Direction::RIGHT), Direction::UP_RIGHT);
    EXPECT_EQ(Direction::rotateCounterClockwise45(Direction::DOWN), Direction::DOWN_RIGHT);
    EXPECT_EQ(Direction::rotateCounterClockwise45(Direction::LEFT), Direction::DOWN_LEFT);
    
    // Test 90 degree rotations
    EXPECT_EQ(Direction::rotateClockwise90(Direction::UP), Direction::RIGHT);
    EXPECT_EQ(Direction::rotateClockwise90(Direction::RIGHT), Direction::DOWN);
    EXPECT_EQ(Direction::rotateClockwise90(Direction::DOWN), Direction::LEFT);
    EXPECT_EQ(Direction::rotateClockwise90(Direction::LEFT), Direction::UP);
    
    EXPECT_EQ(Direction::rotateCounterClockwise90(Direction::UP), Direction::LEFT);
    EXPECT_EQ(Direction::rotateCounterClockwise90(Direction::RIGHT), Direction::UP);
    EXPECT_EQ(Direction::rotateCounterClockwise90(Direction::DOWN), Direction::RIGHT);
    EXPECT_EQ(Direction::rotateCounterClockwise90(Direction::LEFT), Direction::DOWN);
}

TEST_F(MovementSystemTest, DirectionAfterAction) {
    // Test direction changes after different actions
    
    // Test rotate actions
    EXPECT_EQ(Direction::getDirectionAfterAction(Direction::UP, ActionRequest::RotateLeft45), Direction::UP_LEFT);
    EXPECT_EQ(Direction::getDirectionAfterAction(Direction::UP, ActionRequest::RotateRight45), Direction::UP_RIGHT);
    EXPECT_EQ(Direction::getDirectionAfterAction(Direction::UP, ActionRequest::RotateLeft90), Direction::LEFT);
    EXPECT_EQ(Direction::getDirectionAfterAction(Direction::UP, ActionRequest::RotateRight90), Direction::RIGHT);
    
    // Test non-rotate actions (should not change direction)
    EXPECT_EQ(Direction::getDirectionAfterAction(Direction::UP, ActionRequest::MoveForward), Direction::UP);
    EXPECT_EQ(Direction::getDirectionAfterAction(Direction::UP, ActionRequest::MoveBackward), Direction::UP);
    EXPECT_EQ(Direction::getDirectionAfterAction(Direction::UP, ActionRequest::Shoot), Direction::UP);
    EXPECT_EQ(Direction::getDirectionAfterAction(Direction::UP, ActionRequest::GetBattleInfo), Direction::UP);
    EXPECT_EQ(Direction::getDirectionAfterAction(Direction::UP, ActionRequest::DoNothing), Direction::UP);
}

TEST_F(MovementSystemTest, StringRepresentations) {
    // Test string conversion for debugging purposes
    
    // Direction to string
    EXPECT_EQ(Direction::directionToString(Direction::UP), "UP");
    EXPECT_EQ(Direction::directionToString(Direction::RIGHT), "RIGHT");
    EXPECT_EQ(Direction::directionToString(Direction::DOWN), "DOWN");
    EXPECT_EQ(Direction::directionToString(Direction::LEFT), "LEFT");
    
    // Position to string
    Position pos(12, 34);
    EXPECT_EQ(pos.toString(), "Position(12, 34)");
}

TEST_F(MovementSystemTest, DirectionIndexing) {
    // Test direction index conversions
    
    // Get index from direction
    EXPECT_EQ(Direction::getDirectionIndex(Direction::UP), 0);
    EXPECT_EQ(Direction::getDirectionIndex(Direction::RIGHT), 2);
    EXPECT_EQ(Direction::getDirectionIndex(Direction::DOWN), 4);
    EXPECT_EQ(Direction::getDirectionIndex(Direction::LEFT), 6);
    
    // Get direction from index
    EXPECT_EQ(Direction::getDirectionFromIndex(0), Direction::UP);
    EXPECT_EQ(Direction::getDirectionFromIndex(2), Direction::RIGHT);
    EXPECT_EQ(Direction::getDirectionFromIndex(4), Direction::DOWN);
    EXPECT_EQ(Direction::getDirectionFromIndex(6), Direction::LEFT);
    
    // Test index wrapping for out-of-range indices
    EXPECT_EQ(Direction::getDirectionFromIndex(8), Direction::UP);
    EXPECT_EQ(Direction::getDirectionFromIndex(9), Direction::UP_RIGHT);
    EXPECT_EQ(Direction::getDirectionFromIndex(-1), Direction::UP_LEFT);
    EXPECT_EQ(Direction::getDirectionFromIndex(-2), Direction::LEFT);
}
