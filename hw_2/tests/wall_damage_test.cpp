#include <gtest/gtest.h>
#include "Board.h"
#include "Wall.h"

TEST(WallDamageTest, WallTakesDamage) {
    Board board("Test", 10, 5, 10, 10);
    Position wallPos(2, 2);
    auto wall = std::make_unique<Wall>(wallPos);
    Wall* wallPtr = board.placeObject(std::move(wall));
    
    ASSERT_EQ(wallPtr->getHealth(), MAX_HEALTH);
    wallPtr->takeDamage();
    ASSERT_EQ(wallPtr->getHealth(), MAX_HEALTH - 1);
    wallPtr->takeDamage();
    ASSERT_TRUE(wallPtr->isDestroyed());
}

TEST(WallDamageTest, ShootWallFromDistance) {
    Board board("Test", 10, 5, 10, 10);
    Position wallPos(2, 2);
    auto wall = std::make_unique<Wall>(wallPos);
    Wall* wallPtr = board.placeObject(std::move(wall));
    
    ASSERT_EQ(wallPtr->getHealth(), MAX_HEALTH);
    Position shootFrom(1, 1);
    ASSERT_TRUE(board.shoot(shootFrom, wallPos));
    ASSERT_EQ(wallPtr->getHealth(), MAX_HEALTH - 1);
    ASSERT_TRUE(board.shoot(shootFrom, wallPos));
    ASSERT_TRUE(wallPtr->isDestroyed());
    ASSERT_FALSE(board.isWall(wallPos));
}