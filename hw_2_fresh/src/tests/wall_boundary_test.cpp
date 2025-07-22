#include <iostream>
#include <string>
#include <array>

#include "GameObject.h"
#include "Wall.h"
#include "WeakWall.h"
#include "Direction.h" // Direction.h includes Position

// test program to verify wall boundary behavior
int main() {
    std::cout << "======================================\n";
    std::cout << "    WALL BOUNDARY CASE TEST SUITE\n";
    std::cout << "======================================\n\n";

    // Test Case 1: Create and verify walls at boundary positions
    std::cout << "TEST CASE 1: WALLS AT BOUNDARIES\n";
    
    // Create walls at various boundary positions (each one individually)
    Wall topLeftWall(Position(0, 0));           // Top-left corner
    Wall topEdgeWall(Position(0, 20));          // Top edge
    Wall leftEdgeWall(Position(20, 0));         // Left edge
    Wall bottomRightWall(Position(20, 20));     // Bottom-right corner
    Wall bottomEdgeWall(Position(10, 20));      // Bottom edge
    
    // Create weak walls at various boundary positions (each one individually)
    WeakWall leftWeakWall(Position(0, 10));     // Left edge
    WeakWall rightWeakWall(Position(20, 10));   // Right edge
    WeakWall topWeakWall(Position(10, 0));      // Top edge
    WeakWall middleWeakWall(Position(10, 10));  // Middle (not boundary, for comparison)
    
    // Verify initial state of all walls individually
    bool boundaryWallsValid = true;
    
    // Check each wall
    if (topLeftWall.getSymbol() != '#' || topLeftWall.getHealth() != 2) {
        boundaryWallsValid = false;
        std::cout << "Top-left wall has invalid state.\n";
    }
    
    if (topEdgeWall.getSymbol() != '#' || topEdgeWall.getHealth() != 2) {
        boundaryWallsValid = false;
        std::cout << "Top edge wall has invalid state.\n";
    }
    
    if (leftEdgeWall.getSymbol() != '#' || leftEdgeWall.getHealth() != 2) {
        boundaryWallsValid = false;
        std::cout << "Left edge wall has invalid state.\n";
    }
    
    if (bottomRightWall.getSymbol() != '#' || bottomRightWall.getHealth() != 2) {
        boundaryWallsValid = false;
        std::cout << "Bottom-right wall has invalid state.\n";
    }
    
    if (bottomEdgeWall.getSymbol() != '#' || bottomEdgeWall.getHealth() != 2) {
        boundaryWallsValid = false;
        std::cout << "Bottom edge wall has invalid state.\n";
    }
    
    // Check each weak wall
    bool boundaryWeakWallsValid = true;
    
    if (leftWeakWall.getSymbol() != '=' || leftWeakWall.getHealth() != 1) {
        boundaryWeakWallsValid = false;
        std::cout << "Left weak wall has invalid state.\n";
    }
    
    if (rightWeakWall.getSymbol() != '=' || rightWeakWall.getHealth() != 1) {
        boundaryWeakWallsValid = false;
        std::cout << "Right weak wall has invalid state.\n";
    }
    
    if (topWeakWall.getSymbol() != '=' || topWeakWall.getHealth() != 1) {
        boundaryWeakWallsValid = false;
        std::cout << "Top weak wall has invalid state.\n";
    }
    
    if (middleWeakWall.getSymbol() != '=' || middleWeakWall.getHealth() != 1) {
        boundaryWeakWallsValid = false;
        std::cout << "Middle weak wall has invalid state.\n";
    }
    
    std::cout << "All boundary walls valid: " << (boundaryWallsValid ? "Yes" : "No") << "\n";
    std::cout << "All boundary weak walls valid: " << (boundaryWeakWallsValid ? "Yes" : "No") << "\n\n";
    
    // Test Case 2: Damage propagation for adjacent walls
    std::cout << "TEST CASE 2: ADJACENT WALL INDEPENDENCE\n";
    
    // Create a 3x3 grid of walls individually
    Wall topLeftGrid(Position(0, 0));
    Wall topCenterGrid(Position(1, 0));
    Wall topRightGrid(Position(2, 0));
    Wall middleLeftGrid(Position(0, 1));
    Wall centerGrid(Position(1, 1));     // This is our center wall
    Wall middleRightGrid(Position(2, 1));
    Wall bottomLeftGrid(Position(0, 2));
    Wall bottomCenterGrid(Position(1, 2));
    Wall bottomRightGrid(Position(2, 2));
    
    // Damage the center wall
    centerGrid.takeDamage();
    
    // Verify only the center wall was damaged
    bool adjacencyIndependent = true;
    
    // Check if center wall was damaged correctly
    if (centerGrid.getHealth() != 1 || centerGrid.getSymbol() != '=') {
        std::cout << "Center wall was not damaged correctly.\n";
        adjacencyIndependent = false;
    }
    
    // Check that surrounding walls were not damaged
    if (topLeftGrid.getHealth() != 2 || topLeftGrid.getSymbol() != '#') {
        std::cout << "Top left wall was incorrectly damaged.\n";
        adjacencyIndependent = false;
    }
    
    if (topCenterGrid.getHealth() != 2 || topCenterGrid.getSymbol() != '#') {
        std::cout << "Top center wall was incorrectly damaged.\n";
        adjacencyIndependent = false;
    }
    
    if (topRightGrid.getHealth() != 2 || topRightGrid.getSymbol() != '#') {
        std::cout << "Top right wall was incorrectly damaged.\n";
        adjacencyIndependent = false;
    }
    
    if (middleLeftGrid.getHealth() != 2 || middleLeftGrid.getSymbol() != '#') {
        std::cout << "Middle left wall was incorrectly damaged.\n";
        adjacencyIndependent = false;
    }
    
    if (middleRightGrid.getHealth() != 2 || middleRightGrid.getSymbol() != '#') {
        std::cout << "Middle right wall was incorrectly damaged.\n";
        adjacencyIndependent = false;
    }
    
    if (bottomLeftGrid.getHealth() != 2 || bottomLeftGrid.getSymbol() != '#') {
        std::cout << "Bottom left wall was incorrectly damaged.\n";
        adjacencyIndependent = false;
    }
    
    if (bottomCenterGrid.getHealth() != 2 || bottomCenterGrid.getSymbol() != '#') {
        std::cout << "Bottom center wall was incorrectly damaged.\n";
        adjacencyIndependent = false;
    }
    
    if (bottomRightGrid.getHealth() != 2 || bottomRightGrid.getSymbol() != '#') {
        std::cout << "Bottom right wall was incorrectly damaged.\n";
        adjacencyIndependent = false;
    }
    
    std::cout << "Adjacent wall independence: " << (adjacencyIndependent ? "Pass" : "Fail") << "\n\n";
    
    // Test Case 3: Edge case - multiple damage applied to weak wall
    std::cout << "TEST CASE 3: OVER-DAMAGE HANDLING\n";
    
    WeakWall weakWall(Position(5, 5));
    std::cout << "Weak wall initial health: " << weakWall.getHealth() << "\n";
    
    // Apply first damage
    weakWall.takeDamage();
    bool firstDamageHandled = weakWall.isDestroyed();
    std::cout << "Weak wall after first damage - destroyed: " << (firstDamageHandled ? "Yes" : "No") << "\n";
    
    // Apply second damage (should have no additional effect)
    weakWall.takeDamage();
    bool overDamageHandled = weakWall.isDestroyed();
    std::cout << "Weak wall after second damage - destroyed: " << (overDamageHandled ? "Yes" : "No") << "\n";
    std::cout << "Weak wall over-damage properly handled: " << (overDamageHandled ? "Yes" : "No") << "\n\n";
    
    // Test Case 4: Wall visualization consistency
    std::cout << "TEST CASE 4: EMOJI VISUALIZATION CONSISTENCY\n";
    
    // Lambda for converting symbols to emoji
    auto symbolToEmoji = [](char symbol) {
        switch (symbol) {
            case '#': return "🟩";  // Regular wall
            case '=': return "🧱";  // Weak wall
            default: return "⬜";   // Default
        }
    };
    
    // Create fresh walls for testing
    Wall regularWall(Position(1, 1));
    WeakWall freshWeakWall(Position(2, 2));
    
    // Record initial visualizations
    std::string regularEmoji = symbolToEmoji(regularWall.getSymbol());
    std::string weakEmoji = symbolToEmoji(freshWeakWall.getSymbol());
    
    // Damage regular wall
    regularWall.takeDamage();
    std::string damagedRegularEmoji = symbolToEmoji(regularWall.getSymbol());
    
    // Check visualization consistency
    bool visualizationCorrect = 
        regularEmoji == "🟩" &&
        weakEmoji == "🧱" && 
        damagedRegularEmoji == "🧱";
    
    std::cout << "Regular wall emoji: " << regularEmoji << " (should be 🟩)\n";
    std::cout << "Weak wall emoji: " << weakEmoji << " (should be 🧱)\n";
    std::cout << "Damaged regular wall emoji: " << damagedRegularEmoji << " (should be 🧱)\n";
    std::cout << "Visualization consistency: " << (visualizationCorrect ? "Pass" : "Fail") << "\n\n";
    
    // Final summary
    std::cout << "======================================\n";
    std::cout << "TEST SUMMARY:\n";
    std::cout << "1. Boundary wall creation: " << (boundaryWallsValid && boundaryWeakWallsValid ? "PASS" : "FAIL") << "\n";
    std::cout << "2. Adjacent wall independence: " << (adjacencyIndependent ? "PASS" : "FAIL") << "\n";
    std::cout << "3. Over-damage handling: " << (overDamageHandled ? "PASS" : "FAIL") << "\n";
    std::cout << "4. Visualization consistency: " << (visualizationCorrect ? "PASS" : "FAIL") << "\n";
    
    bool allTestsPassed = boundaryWallsValid && boundaryWeakWallsValid && 
                         adjacencyIndependent && overDamageHandled && visualizationCorrect;
    
    if (allTestsPassed) {
        std::cout << "\n✅ All tests PASSED!\n";
    } else {
        std::cout << "\n❌ Some tests FAILED!\n";
    }
    std::cout << "======================================\n";
    
    return allTestsPassed ? 0 : 1;
}
