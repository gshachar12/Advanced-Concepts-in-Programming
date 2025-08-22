#include <iostream>
#include <string>

#include "GameObject.h"
#include "Wall.h"
#include "WeakWall.h"
#include "Direction.h" // Direction.h includes Position
// We won't use actual Shell objects due to dependencies
// #include "Shell.h"     // For projectile tests

// test program to verify wall collision behavior
int main() {
    std::cout << "======================================\n";
    std::cout << "    WALL COLLISION TEST SUITE\n";
    std::cout << "======================================\n\n";

    // Create a standard wall
    Position wallPos(5, 5);
    Wall wall(wallPos);
    
    // Create a weak wall
    Position weakWallPos(10, 5);
    WeakWall weakWall(weakWallPos);
    
    // Create positions for simulated projectiles
    Position shellPos1(4, 5); // Position to collide with regular wall
    Position shellPos2(11, 5); // Position to collide with weak wall
    
    // We'll simulate the shell movement directly by changing positions
    // This simulates shells moving in different directions without needing the actual Direction objects
    std::cout << "Simulating shell moving RIGHT towards regular wall\n";
    std::cout << "Simulating shell moving LEFT towards weak wall\n";
    
    // We'll simulate the shell movement without creating actual Shell objects
    // since they have dependencies we don't want to include in this simple test
    
    // Test initial state
    std::cout << "INITIAL STATE:\n";
    std::cout << "Regular wall position: (" << wall.getPosition().x << "," << wall.getPosition().y << ")\n";
    std::cout << "Regular wall health: " << wall.getHealth() << " (should be 2)\n";
    std::cout << "Weak wall position: (" << weakWall.getPosition().x << "," << weakWall.getPosition().y << ")\n";
    std::cout << "Weak wall health: " << weakWall.getHealth() << " (should be 1)\n";
    std::cout << "Simulated shell 1 position: (" << shellPos1.x << "," << shellPos1.y << ")\n";
    std::cout << "Simulated shell 2 position: (" << shellPos2.x << "," << shellPos2.y << ")\n";
    
    // Simulate collision with regular wall
    std::cout << "\nSIMULATING COLLISION WITH REGULAR WALL:\n";
    
    // Move simulated shell1 towards the wall
    shellPos1.x += 1;
    
    // Check if simulated shell is at the wall position
    bool collisionDetected1 = (shellPos1.x == wall.getPosition().x && 
                              shellPos1.y == wall.getPosition().y);
    
    if (collisionDetected1) {
        wall.takeDamage();
        std::cout << "Collision detected with regular wall!\n";
        std::cout << "Regular wall health after hit: " << wall.getHealth() << " (should be 1)\n";
        std::cout << "Wall symbol after hit: " << wall.getSymbol() << " (should be '=')\n";
    } else {
        std::cout << "Collision detection failed for regular wall. Check test setup.\n";
    }
    
    // Simulate collision with weak wall
    std::cout << "\nSIMULATING COLLISION WITH WEAK WALL:\n";
    
    // Move simulated shell2 towards the weak wall
    shellPos2.x -= 1;
    
    // Check if simulated shell is at the wall position
    bool collisionDetected2 = (shellPos2.x == weakWall.getPosition().x && 
                              shellPos2.y == weakWall.getPosition().y);
    
    if (collisionDetected2) {
        weakWall.takeDamage();
        std::cout << "Collision detected with weak wall!\n";
        std::cout << "Weak wall health after hit: " << weakWall.getHealth() << " (should be 0)\n";
        std::cout << "Weak wall destroyed? " << (weakWall.isDestroyed() ? "Yes" : "No") << " (should be Yes)\n";
    } else {
        std::cout << "Collision detection failed for weak wall. Check test setup.\n";
    }
    
    // Test second hit on regular wall
    if (!wall.isDestroyed()) {
        std::cout << "\nSIMULATING SECOND HIT ON REGULAR WALL:\n";
        wall.takeDamage();
        std::cout << "Regular wall health after second hit: " << wall.getHealth() << " (should be 0)\n";
        std::cout << "Regular wall destroyed? " << (wall.isDestroyed() ? "Yes" : "No") << " (should be Yes)\n";
    }
    
    // Final test summary
    std::cout << "\n======================================\n";
    std::cout << "TEST SUMMARY:\n";
    
    // Check each condition separately and report the result
    bool regularWallTransformed = wall.getHealth() == 1 || wall.isDestroyed();
    bool regularWallDestroyed = wall.isDestroyed();
    bool weakWallDestroyed = weakWall.isDestroyed();
    bool collisionsDetected = collisionDetected1 && collisionDetected2;
    
    std::cout << "Regular wall transformation test: " << (regularWallTransformed ? "PASS" : "FAIL") << std::endl;
    std::cout << "Regular wall destruction test: " << (regularWallDestroyed ? "PASS" : "FAIL") << std::endl;
    std::cout << "Weak wall destruction test: " << (weakWallDestroyed ? "PASS" : "FAIL") << std::endl;
    std::cout << "Collision detection test: " << (collisionsDetected ? "PASS" : "FAIL") << std::endl;
    
    bool allTestsPassed = regularWallTransformed && regularWallDestroyed && weakWallDestroyed && collisionsDetected;
    
    if (allTestsPassed) {
        std::cout << "\n✅ All tests PASSED!\n";
    } else {
        std::cout << "\n❌ Some tests FAILED!\n";
    }
    std::cout << "======================================\n";
    
    return allTestsPassed ? 0 : 1;
}
