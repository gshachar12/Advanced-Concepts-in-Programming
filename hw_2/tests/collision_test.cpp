#include <gtest/gtest.h>
#include "Board.h"
#include "Tank.h"
#include "Mine.h"
#include "Collision.h"

TEST(CollisionTest, TankCollisionWithMine) {
    Board board("Test", 10, 5, 10, 10);
    
    // Place mine
    Position minePos(5, 5);
    auto mine = std::make_unique<Mine>(minePos);
    board.placeObject(std::move(mine));
    
    // Place tank adjacent to mine
    Position tankPos(6, 5);
    auto tank = std::make_unique<Tank>(tankPos, 1, 0, 10);
    Tank* tankPtr = board.placeObject(std::move(tank));
    
    // Move tank into mine position
    tankPtr->setDirection(Direction::LEFT);
    board.moveObject(tankPtr->getPosition(), tankPtr->getDirection());
    
    // Check for collision (should destroy both tank and mine)
    board.finishMove();
    
    // Both mine and tank should be gone
    ASSERT_FALSE(board.isMine(minePos));
    ASSERT_FALSE(board.isOccupied(minePos));
    ASSERT_FALSE(board.isOccupied(tankPos));
}

TEST(CollisionTest, TankTankCollision) {
    Board board("Test", 10, 5, 10, 10);
    
    // Place first tank
    Position tank1Pos(5, 5);
    auto tank1 = std::make_unique<Tank>(tank1Pos, 1, 0, 10);
    board.placeObject(std::move(tank1));
    
    // Place second tank adjacent to first
    Position tank2Pos(6, 5);
    auto tank2 = std::make_unique<Tank>(tank2Pos, 2, 1, 10);
    Tank* tank2Ptr = board.placeObject(std::move(tank2));
    
    // Try to move second tank into first tank's position
    tank2Ptr->setDirection(Direction::LEFT);
    board.moveObject(tank2Ptr->getPosition(), tank2Ptr->getDirection());
    
    // Check positions after move attempt (should not have moved)
    board.finishMove();
    ASSERT_TRUE(board.isTank(tank1Pos));
    ASSERT_TRUE(board.isTank(tank2Pos));
}

TEST(CollisionTest, ShellShellCollision) {
    Board board("Test", 10, 5, 10, 10);
    
    // Create two tanks
    Position tank1Pos(1, 5);
    auto tank1 = std::make_unique<Tank>(tank1Pos, 1, 0, 10);
    board.placeObject(std::move(tank1));
    
    Position tank2Pos(9, 5);
    auto tank2 = std::make_unique<Tank>(tank2Pos, 2, 1, 10);
    board.placeObject(std::move(tank2));
    
    // Shells meeting in the middle should destroy each other
    Position shootPos1(5, 5);
    Position shootPos2(5, 5);
    
    // Both tanks shoot
    board.shoot(tank1Pos, shootPos1);
    board.shoot(tank2Pos, shootPos2);
    
    // Shells should destroy each other
    board.finishMove();
    
    // Position where shells met should be empty
    ASSERT_FALSE(board.isShell(Position(5, 5)));
}

TEST(CollisionTest, TankMovesIntoShell) {
    Board board("Test", 10, 5, 10, 10);
    
    // Place tank
    Position tankPos(5, 5);
    auto tank = std::make_unique<Tank>(tankPos, 1, 0, 10);
    Tank* tankPtr = board.placeObject(std::move(tank));
    
    // Create shell adjacent to tank
    Position shellPos(6, 5);
    
    // Move tank into shell position (should result in tank destruction)
    tankPtr->setDirection(Direction::RIGHT);
    board.moveObject(tankPtr->getPosition(), tankPtr->getDirection());
    
    // Tank should be destroyed
    board.shoot(Position(7, 5), shellPos);
    board.finishMove();
    
    // Tank should be gone
    ASSERT_FALSE(board.isTank(tankPos));
    ASSERT_FALSE(board.isTank(shellPos));
}
