#include <gtest/gtest.h>
#include "Board.h"
#include "WeakWall.h"

TEST(WeakWallTest, DestroyedAfterOneDamage) {
    Board board("Test", 10, 5, 10, 10);
    Position weakWallPos(3, 3);
    auto weakWall = std::make_unique<WeakWall>(weakWallPos);
    WeakWall* weakWallPtr = board.placeObject(std::move(weakWall));
    
    ASSERT_TRUE(weakWallPtr->isDestroyable());
    ASSERT_EQ(weakWallPtr->getHitPoints(), 1);
    
    Position shootFrom(2, 2);
    ASSERT_TRUE(board.shoot(shootFrom, weakWallPos));
    ASSERT_FALSE(board.isWall(weakWallPos));
}

TEST(WeakWallTest, WeakWallProperties) {
    WeakWall weakWall({1, 1});
    ASSERT_EQ(weakWall.getSymbol(), 'W');
    ASSERT_TRUE(weakWall.isDestroyable());
    ASSERT_TRUE(weakWall.isCollidable());
    ASSERT_FALSE(weakWall.isPassable());
    ASSERT_TRUE(weakWall.isWall());
}