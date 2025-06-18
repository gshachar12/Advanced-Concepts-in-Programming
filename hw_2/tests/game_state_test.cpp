#include <gtest/gtest.h>
#include "Board.h"
#include "MySatelliteView.h"
#include "Tank.h"
#include "Wall.h"
#include "Mine.h"
#include "WeakWall.h"

TEST(SatelliteViewTest, ViewContents) {
    Board board("Test", 10, 5, 10, 10);
    
    // Place various objects
    Position tankPos(1, 1);
    auto tank = std::make_unique<Tank>(tankPos, 1, 0, 10);
    board.placeObject(std::move(tank));
    
    Position wallPos(2, 2);
    auto wall = std::make_unique<Wall>(wallPos);
    board.placeObject(std::move(wall));
    
    Position minePos(3, 3);
    auto mine = std::make_unique<Mine>(minePos);
    board.placeObject(std::move(mine));
    
    Position weakWallPos(4, 4);
    auto weakWall = std::make_unique<WeakWall>(weakWallPos);
    board.placeObject(std::move(weakWall));
    
    // Create satellite view
    MySatelliteView view(10, 10);
    board.fillSatelliteView(view);
    
    // Check view contents
    ASSERT_EQ(view.getObjectAt(1, 1), '1');
    ASSERT_EQ(view.getObjectAt(2, 2), '#');
    ASSERT_EQ(view.getObjectAt(3, 3), '@');
    ASSERT_EQ(view.getObjectAt(4, 4), 'W');
    ASSERT_EQ(view.getObjectAt(5, 5), ' ');
    
    // Check out of bounds
    ASSERT_EQ(view.getObjectAt(20, 20), '&');
}

TEST(SatelliteViewTest, ViewUpdatesAfterMove) {
    Board board("Test", 10, 5, 10, 10);
    
    // Place tank
    Position tankPos(5, 5);
    auto tank = std::make_unique<Tank>(tankPos, 1, 0, 10);
    Tank* tankPtr = board.placeObject(std::move(tank));
    
    // Initial view
    MySatelliteView viewBefore(10, 10);
    board.fillSatelliteView(viewBefore);
    ASSERT_EQ(viewBefore.getObjectAt(5, 5), '1');
    
    // Move tank
    board.moveObject(tankPtr->getPosition(), tankPtr->getDirection());
    board.finishMove();
    
    // Updated view
    MySatelliteView viewAfter(10, 10);
    board.fillSatelliteView(viewAfter);
    
    // Original position should now be empty
    ASSERT_EQ(viewAfter.getObjectAt(5, 5), ' ');
    
    // New position should have tank (tank moved left)
    ASSERT_EQ(viewAfter.getObjectAt(4, 5), '1');
}

TEST(GameStateTest, DetectPlayerVictory) {
    Board board("Test", 10, 5, 10, 10);
    
    // Player 1 has two tanks
    Position tank1Pos(1, 1);
    auto tank1 = std::make_unique<Tank>(tank1Pos, 1, 0, 10);
    board.placeObject(std::move(tank1));
    
    Position tank2Pos(2, 2);
    auto tank2 = std::make_unique<Tank>(tank2Pos, 1, 1, 10);
    board.placeObject(std::move(tank2));
    
    // Player 2 has one tank
    Position tank3Pos(3, 3);
    auto tank3 = std::make_unique<Tank>(tank3Pos, 2, 0, 10);
    Tank* tank3Ptr = board.placeObject(std::move(tank3));
    
    // Initial tank counts
    auto player1TanksBefore = board.getPlayerAliveTanks(1);
    auto player2TanksBefore = board.getPlayerAliveTanks(2);
    ASSERT_EQ(player1TanksBefore.size(), 2);
    ASSERT_EQ(player2TanksBefore.size(), 1);
    
    // Destroy player 2's tank
    tank3Ptr->destroy();
    
    // Check tank counts after destruction
    auto player1TanksAfter = board.getPlayerAliveTanks(1);
    auto player2TanksAfter = board.getPlayerAliveTanks(2);
    ASSERT_EQ(player1TanksAfter.size(), 2);
    ASSERT_EQ(player2TanksAfter.size(), 0);
    
    // Player 1 should have won
    bool player1Won = !player1TanksAfter.empty() && player2TanksAfter.empty();
    bool player2Won = !player2TanksAfter.empty() && player1TanksAfter.empty();
    bool tie = player1TanksAfter.empty() && player2TanksAfter.empty();
    
    ASSERT_TRUE(player1Won);
    ASSERT_FALSE(player2Won);
    ASSERT_FALSE(tie);
}

TEST(GameStateTest, DetectTie) {
    Board board("Test", 10, 5, 10, 10);
    
    // Player 1 has one tank
    Position tank1Pos(1, 1);
    auto tank1 = std::make_unique<Tank>(tank1Pos, 1, 0, 10);
    Tank* tank1Ptr = board.placeObject(std::move(tank1));
    
    // Player 2 has one tank
    Position tank2Pos(3, 3);
    auto tank2 = std::make_unique<Tank>(tank2Pos, 2, 0, 10);
    Tank* tank2Ptr = board.placeObject(std::move(tank2));
    
    // Destroy both tanks
    tank1Ptr->destroy();
    tank2Ptr->destroy();
    
    // Check tank counts after destruction
    auto player1Tanks = board.getPlayerAliveTanks(1);
    auto player2Tanks = board.getPlayerAliveTanks(2);
    ASSERT_EQ(player1Tanks.size(), 0);
    ASSERT_EQ(player2Tanks.size(), 0);
    
    // Should be a tie
    bool player1Won = !player1Tanks.empty() && player2Tanks.empty();
    bool player2Won = !player2Tanks.empty() && player1Tanks.empty();
    bool tie = player1Tanks.empty() && player2Tanks.empty();
    
    ASSERT_FALSE(player1Won);
    ASSERT_FALSE(player2Won);
    ASSERT_TRUE(tie);
}
