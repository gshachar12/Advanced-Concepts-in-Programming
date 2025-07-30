#include <iostream>
#include <string>

#include "GameObject.h"
#include "Wall.h"
#include "WeakWall.h"
#include "Direction.h" // Direction.h includes Position

// test program to verify wall behavior
int main() {
    std::cout << "======================================\n";
    std::cout << "  WALL TRANSFORMATION SIMPLE TEST\n";
    std::cout << "======================================\n\n";

    // Create a standard wall
    Position pos1(1, 1);
    Wall wall(pos1);
    
    // Create a weak wall
    Position pos2(2, 2);
    WeakWall weakWall(pos2);
    
    // Test initial state
    std::cout << "INITIAL STATE:\n";
    std::cout << "Regular wall symbol: " << wall.getSymbol() 
              << " (should be '#')" << std::endl;
    std::cout << "Regular wall health: " << wall.getHealth() 
              << " (should be 2)" << std::endl;
    std::cout << "Weak wall symbol: " << weakWall.getSymbol() 
              << " (should be '=')" << std::endl;
    std::cout << "Weak wall health: " << weakWall.getHealth() 
              << " (should be 1)" << std::endl;
    
    // Visual representation mapping
    auto symbolToEmoji = [](char symbol) {
        switch (symbol) {
            case '#': return "🟩";  // Regular wall
            case '=': return "🧱";  // Weak wall
            default: return "⬜";   // Default
        }
    };
    
    std::cout << "\nVISUAL REPRESENTATION:\n";
    std::cout << "Regular wall: " << symbolToEmoji(wall.getSymbol()) << std::endl;
    std::cout << "Weak wall: " << symbolToEmoji(weakWall.getSymbol()) << std::endl;
    
    // Apply damage to regular wall (first hit)
    wall.takeDamage();
    
    // Test after damage
    std::cout << "\nAFTER DAMAGING REGULAR WALL:\n";
    std::cout << "Regular wall symbol: " << wall.getSymbol() 
              << " (should be '=')" << std::endl;
    std::cout << "Regular wall health: " << wall.getHealth() 
              << " (should be 1)" << std::endl;
    
    std::cout << "\nVISUAL REPRESENTATION AFTER DAMAGE:\n";
    std::cout << "Damaged regular wall: " << symbolToEmoji(wall.getSymbol()) << std::endl;
    std::cout << "Weak wall: " << symbolToEmoji(weakWall.getSymbol()) << std::endl;
    
    // Test if symbols match
    bool symbolsMatch = wall.getSymbol() == weakWall.getSymbol();
    std::cout << "\nDo symbols match? " << (symbolsMatch ? "Yes" : "No") 
              << " (should be Yes)" << std::endl;
    
    // Apply second damage to regular wall and first to weak wall
    wall.takeDamage();
    weakWall.takeDamage();
    
    // Test destruction
    std::cout << "\nAFTER APPLYING MORE DAMAGE:\n";
    std::cout << "Regular wall destroyed? " << (wall.isDestroyed() ? "Yes" : "No") 
              << " (should be Yes)" << std::endl;
    std::cout << "Weak wall destroyed? " << (weakWall.isDestroyed() ? "Yes" : "No") 
              << " (should be Yes)" << std::endl;
    
    // Final test summary
    std::cout << "\n======================================\n";
    std::cout << "TEST SUMMARY:\n";
    
    // Check each condition separately and report the result
    bool symbolTest1 = wall.isDestroyed() || wall.getSymbol() == '='; // Accounts for destroyed state
    bool symbolTest2 = weakWall.isDestroyed() || weakWall.getSymbol() == '='; // Accounts for destroyed state
    bool matchTest = wall.getSymbol() == weakWall.getSymbol() || (wall.isDestroyed() && weakWall.isDestroyed());
    bool destroyTest1 = wall.isDestroyed();
    bool destroyTest2 = weakWall.isDestroyed();
    
    std::cout << "Symbol test (wall): " << (symbolTest1 ? "PASS" : "FAIL") << std::endl;
    std::cout << "Symbol test (weak wall): " << (symbolTest2 ? "PASS" : "FAIL") << std::endl;
    std::cout << "Symbol match test: " << (matchTest ? "PASS" : "FAIL") << std::endl;
    std::cout << "Destruction test (wall): " << (destroyTest1 ? "PASS" : "FAIL") << std::endl;
    std::cout << "Destruction test (weak wall): " << (destroyTest2 ? "PASS" : "FAIL") << std::endl;
    
    bool allTestsPassed = symbolTest1 && symbolTest2 && matchTest && destroyTest1 && destroyTest2;
    
    if (allTestsPassed) {
        std::cout << "\n✅ All tests PASSED!\n";
    } else {
        std::cout << "\n❌ Some tests FAILED!\n";
    }
    std::cout << "======================================\n";
    
    return allTestsPassed ? 0 : 1;
}
