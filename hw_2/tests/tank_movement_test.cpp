#include <gtest/gtest.h>
#include "Board.h"
#include "Tank.h"

TEST(TankMovementTest, TankMoveForward) {
    Board board("Test", 10, 5, 10, 10);
    Position tankPos(5, 5);
    auto tank = std::make_unique<Tank>(tankPos, 1, 0, 10);
    Tank* tankPtr = board.placeObject(std::move(tank));
    
    // Initial position
    ASSERT_EQ(tankPtr->getPosition().x, 5);
    ASSERT_EQ(tankPtr->getPosition().y, 5);
    
    // Tank initially points to the left (for player 1)
    ASSERT_EQ(tankPtr->getDirection(), Direction::LEFT);
    
    // Move forward (should move left)
    GameObject* movedTank = board.moveObject(tankPtr->getPosition(), tankPtr->getDirection());
    
    // Check new position
    ASSERT_EQ(movedTank->getPosition().x, 4);
    ASSERT_EQ(movedTank->getPosition().y, 5);
}

TEST(TankMovementTest, TankRotation) {
    Board board("Test", 10, 5, 10, 10);
    Position tankPos(5, 5);
    auto tank = std::make_unique<Tank>(tankPos, 1, 0, 10);
    Tank* tankPtr = board.placeObject(std::move(tank));
    
    // Initial direction (LEFT for player 1)
    ASSERT_EQ(tankPtr->getDirection(), Direction::LEFT);
    
    // Rotate 45 degrees counterclockwise
    tankPtr->setDirection(Direction::rotateDirection(tankPtr->getDirection(), false, false));
    ASSERT_EQ(tankPtr->getDirection(), Direction::DOWN_LEFT);
    
    // Rotate 90 degrees clockwise
    tankPtr->setDirection(Direction::rotateDirection(tankPtr->getDirection(), true, true));
    ASSERT_EQ(tankPtr->getDirection(), Direction::UP_LEFT);
}

TEST(TankMovementTest, TankCollisionWithWall) {
    Board board("Test", 10, 5, 10, 10);
    
    // Place a wall
    Position wallPos(4, 5);
    auto wall = std::make_unique<Wall>(wallPos);
    board.placeObject(std::move(wall));
    
    // Place a tank adjacent to the wall
    Position tankPos(5, 5);
    auto tank = std::make_unique<Tank>(tankPos, 1, 0, 10);
    Tank* tankPtr = board.placeObject(std::move(tank));
    
    // Try to move into the wall (should fail)
    board.moveObject(tankPtr->getPosition(), tankPtr->getDirection());
    
    // Tank should remain in original position
    ASSERT_EQ(tankPtr->getPosition().x, 5);
    ASSERT_EQ(tankPtr->getPosition().y, 5);
}

TEST(TankMovementTest, TankWrapsAroundBoard) {
    Board board("Test", 10, 5, 10, 10);
    
    // Place tank at edge of board
    Position tankPos(0, 5);
    auto tank = std::make_unique<Tank>(tankPos, 1, 0, 10);
    Tank* tankPtr = board.placeObject(std::move(tank));
    
    // Move west (should wrap around to the east side)
    tankPtr->setDirection(Direction::LEFT);
    GameObject* movedTank = board.moveObject(tankPtr->getPosition(), tankPtr->getDirection());
    
    // Check new position (should be on the opposite side)
    ASSERT_EQ(movedTank->getPosition().x, 9);
    ASSERT_EQ(movedTank->getPosition().y, 5);
}
