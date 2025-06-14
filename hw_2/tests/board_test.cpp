#include <gtest/gtest.h>
#include "Board.h"
#include "Tank.h"
#include "Wall.h"
#include "Mine.h"
#include "WeakWall.h"

TEST(BoardTest, BoardInitialization) {
    Board board("Test Board", 100, 20, 8, 10);
    
    ASSERT_EQ(board.getWidth(), 10);
    ASSERT_EQ(board.getHeight(), 8);
    ASSERT_EQ(board.getMaxSteps(), 100);
    ASSERT_EQ(board.getNumShells(), 20);
}

TEST(BoardTest, ObjectPlacement) {
    Board board("Test", 10, 5, 10, 10);
    
    // Place objects
    Position tankPos(1, 1);
    auto tank = std::make_unique<Tank>(tankPos, 1, 0, 10);
    Tank* tankPtr = board.placeObject(std::move(tank));
    
    Position wallPos(2, 2);
    auto wall = std::make_unique<Wall>(wallPos);
    Wall* wallPtr = board.placeObject(std::move(wall));
    
    Position minePos(3, 3);
    auto mine = std::make_unique<Mine>(minePos);
    Mine* minePtr = board.placeObject(std::move(mine));
    
    Position weakWallPos(4, 4);
    auto weakWall = std::make_unique<WeakWall>(weakWallPos);
    WeakWall* weakWallPtr = board.placeObject(std::move(weakWall));
    
    // Check object placement
    ASSERT_TRUE(board.isTank(tankPos));
    ASSERT_TRUE(board.isWall(wallPos));
    ASSERT_TRUE(board.isMine(minePos));
    ASSERT_TRUE(board.isWall(weakWallPos));
    
    // Check positions
    ASSERT_EQ(tankPtr->getPosition().x, 1);
    ASSERT_EQ(tankPtr->getPosition().y, 1);
    
    ASSERT_EQ(wallPtr->getPosition().x, 2);
    ASSERT_EQ(wallPtr->getPosition().y, 2);
    
    ASSERT_EQ(minePtr->getPosition().x, 3);
    ASSERT_EQ(minePtr->getPosition().y, 3);
    
    ASSERT_EQ(weakWallPtr->getPosition().x, 4);
    ASSERT_EQ(weakWallPtr->getPosition().y, 4);
}

TEST(BoardTest, ObjectRemoval) {
    Board board("Test", 10, 5, 10, 10);
    
    // Place object
    Position tankPos(5, 5);
    auto tank = std::make_unique<Tank>(tankPos, 1, 0, 10);
    board.placeObject(std::move(tank));
    
    // Verify placement
    ASSERT_TRUE(board.isTank(tankPos));
    
    // Remove object
    board.removeObject(tankPos);
    
    // Verify removal
    ASSERT_FALSE(board.isOccupied(tankPos));
}

TEST(BoardTest, PositionWrapping) {
    Board board("Test", 10, 5, 10, 10);
    
    // Test horizontal wrapping
    Position pos1(12, 5);
    Position wrapped1 = board.wrapPosition(pos1);
    ASSERT_EQ(wrapped1.x, 2);
    ASSERT_EQ(wrapped1.y, 5);
    
    // Test vertical wrapping
    Position pos2(5, 15);
    Position wrapped2 = board.wrapPosition(pos2);
    ASSERT_EQ(wrapped2.x, 5);
    ASSERT_EQ(wrapped2.y, 5);
    
    // Test negative coordinates
    Position pos3(-2, -3);
    Position wrapped3 = board.wrapPosition(pos3);
    ASSERT_EQ(wrapped3.x, 8);
    ASSERT_EQ(wrapped3.y, 7);
}

TEST(BoardTest, MultiplePlayerTanks) {
    Board board("Test", 10, 5, 10, 10);
    
    // Place multiple tanks for each player
    Position tank1Pos(1, 1);
    auto tank1 = std::make_unique<Tank>(tank1Pos, 1, 0, 10);
    board.placeObject(std::move(tank1));
    
    Position tank2Pos(2, 2);
    auto tank2 = std::make_unique<Tank>(tank2Pos, 1, 1, 10);
    board.placeObject(std::move(tank2));
    
    Position tank3Pos(3, 3);
    auto tank3 = std::make_unique<Tank>(tank3Pos, 2, 0, 10);
    board.placeObject(std::move(tank3));
    
    Position tank4Pos(4, 4);
    auto tank4 = std::make_unique<Tank>(tank4Pos, 2, 1, 10);
    board.placeObject(std::move(tank4));
    
    // Check tank counts
    auto player1Tanks = board.getPlayerTanks(1);
    auto player2Tanks = board.getPlayerTanks(2);
    
    ASSERT_EQ(player1Tanks.size(), 2);
    ASSERT_EQ(player2Tanks.size(), 2);
    
    // Check individual tanks by index
    auto tank1_0 = board.getPlayerTank(1, 0);
    auto tank1_1 = board.getPlayerTank(1, 1);
    auto tank2_0 = board.getPlayerTank(2, 0);
    auto tank2_1 = board.getPlayerTank(2, 1);
    
    ASSERT_NE(tank1_0, nullptr);
    ASSERT_NE(tank1_1, nullptr);
    ASSERT_NE(tank2_0, nullptr);
    ASSERT_NE(tank2_1, nullptr);
}
