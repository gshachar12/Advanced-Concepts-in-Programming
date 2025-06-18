#include <gtest/gtest.h>
#include "../src/gameManager/Wall.h"
#include "../src/gameManager/WeakWall.h"
#include "../src/gameManager/GameManager.h"
#include "../src/gameManager/Board.h"
#include "../src/gameManager/GameObjectFactory.h"
#include <memory>

// Test Wall's getSymbol logic when health is reduced to 1
TEST(WallDisplayTest, WallChangesSymbolWhenDamaged) {
    // Create a wall
    Position wallPos(1, 1);
    Wall wall(wallPos);
    
    // Wall should start with symbol '#'
    ASSERT_EQ(wall.getSymbol(), '#');
    ASSERT_EQ(wall.getHealth(), 2); // MAX_HEALTH = 2
    
    // Take damage, health should be 1 and symbol should change to '='
    wall.takeDamage();
    ASSERT_EQ(wall.getHealth(), 1);
    ASSERT_EQ(wall.getSymbol(), '=');
    
    // Further damage should destroy the wall
    wall.takeDamage();
    ASSERT_EQ(wall.getHealth(), 0);
    ASSERT_TRUE(wall.isDestroyed());
}

// Test that the visualization works properly
TEST(WallDisplayTest, VisualBoardShowsCorrectSymbols) {
    // Create a mock GameManager with the visual mode enabled
    GameManager gm(true);
    
    // Create a board with walls
    auto board = std::make_unique<Board>("Test", 10, 5, 5, 5);
    
    // Place a regular wall
    Position regularWallPos(1, 1);
    auto regularWall = std::make_unique<Wall>(regularWallPos);
    Wall* regularWallPtr = static_cast<Wall*>(board->placeObject(std::move(regularWall)));
    
    // Place a weak wall
    Position weakWallPos(2, 2);
    auto weakWall = std::make_unique<WeakWall>(weakWallPos);
    WeakWall* weakWallPtr = static_cast<WeakWall*>(board->placeObject(std::move(weakWall)));
    
    // Damage the regular wall to make it weak
    regularWallPtr->takeDamage();
    
    // Verify symbols
    ASSERT_EQ(regularWallPtr->getSymbol(), '='); // Regular wall is now weak
    ASSERT_EQ(weakWallPtr->getSymbol(), '=');    // Weak wall is already '='
    
    // Access the protected initVisualBoard method to mock what happens in the GameManager
    gm.setBoard(std::move(board));
    
    // Call public displayGame
    gm.displayGame();
}

// Main function to run the tests
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
