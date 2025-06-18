#include <gtest/gtest.h>
#include "Board.h"
#include "Tank.h"
#include "Shell.h"
#include "Wall.h"
#include "WeakWall.h"

TEST(TankShootingTest, TankFiresShell) {
    Board board("Test", 10, 5, 10, 10);
    Position tankPos(5, 5);
    auto tank = std::make_unique<Tank>(tankPos, 1, 0, 10);
    Tank* tankPtr = board.placeObject(std::move(tank));
    
    // Check initial ammunition
    ASSERT_EQ(tankPtr->getAmmunition(), 10);
    
    // Shoot
    tankPtr->decrementAmmunition();
    
    // Check ammunition after shooting
    ASSERT_EQ(tankPtr->getAmmunition(), 9);
    
    // Check cooldown after shooting
    tankPtr->shoot();
    ASSERT_GT(tankPtr->getCooldown(), 0);
}

TEST(TankShootingTest, ShellDestroysWeakWall) {
    Board board("Test", 10, 5, 10, 10);
    
    // Place weak wall
    Position weakWallPos(5, 5);
    auto weakWall = std::make_unique<WeakWall>(weakWallPos);
    board.placeObject(std::move(weakWall));
    
    // Tank shoots at weak wall
    Position shootFrom(4, 5);
    ASSERT_TRUE(board.shoot(shootFrom, weakWallPos));
    
    // Check if wall was destroyed
    ASSERT_FALSE(board.isWall(weakWallPos));
}

TEST(TankShootingTest, ShellHitsMine) {
    Board board("Test", 10, 5, 10, 10);
    
    // Place mine
    Position minePos(5, 5);
    auto mine = std::make_unique<Mine>(minePos);
    board.placeObject(std::move(mine));
    
    // Tank shoots at mine
    Position shootFrom(4, 5);
    ASSERT_TRUE(board.shoot(shootFrom, minePos));
    
    // Check if mine was destroyed
    ASSERT_FALSE(board.isMine(minePos));
}

TEST(TankShootingTest, TankShootingLimits) {
    Board board("Test", 10, 3, 10, 10);
    Position tankPos(5, 5);
    auto tank = std::make_unique<Tank>(tankPos, 1, 0, 3);
    Tank* tankPtr = board.placeObject(std::move(tank));
    
    // Use all ammunition
    tankPtr->decrementAmmunition();
    tankPtr->decrementAmmunition();
    tankPtr->decrementAmmunition();
    
    // Check that ammunition is depleted
    ASSERT_EQ(tankPtr->getAmmunition(), 0);
}
