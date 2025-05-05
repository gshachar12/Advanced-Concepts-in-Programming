#include "../../src/Shell.h"
#include "../test_utils.h"

bool test_shell_initialization() {
    Shell shell(3, 4, Direction::DR);
    
    ASSERT_EQUAL(3, shell.getX());
    ASSERT_EQUAL(4, shell.getY());
    ASSERT_EQUAL(Direction::DR, shell.getDirection());
    ASSERT_TRUE(shell.isActive());
    
    return true;
}

bool test_shell_movement() {
    Shell shell(5, 5, Direction::R);
    
    // Test advancement (should move 2 cells)
    shell.advance();
    ASSERT_EQUAL(7, shell.getX()); // Moved 2 cells right
    ASSERT_EQUAL(5, shell.getY());
    ASSERT_EQUAL(2, shell.getDistanceTraveled());
    
    // Test a different direction
    Shell shell2(5, 5, Direction::D);
    shell2.advance();
    ASSERT_EQUAL(5, shell2.getX());
    ASSERT_EQUAL(7, shell2.getY()); // Moved 2 cells down
    
    // Test diagonal
    Shell shell3(5, 5, Direction::DR);
    shell3.advance();
    ASSERT_EQUAL(7, shell3.getX()); // Moved 2 cells diagonally
    ASSERT_EQUAL(7, shell3.getY());
    
    return true;
}

bool test_shell_update() {
    Shell shell(5, 5, Direction::U);
    
    // Update should call advance
    shell.update();
    ASSERT_EQUAL(5, shell.getX());
    ASSERT_EQUAL(3, shell.getY()); // Moved 2 cells up
    
    return true;
}

bool test_shell_deactivation() {
    Shell shell(5, 5, Direction::R);
    
    ASSERT_TRUE(shell.isActive());
    
    shell.deactivate();
    
    ASSERT_FALSE(shell.isActive());
    
    return true;
}

bool test_shell_max_range() {
    Shell shell(5, 5, Direction::R);
    shell.setMaxRange(4); // Set max range to 4
    
    // After moving 2 cells
    shell.advance();
    ASSERT_TRUE(shell.isActive());
    ASSERT_EQUAL(2, shell.getDistanceTraveled());
    
    // After moving another 2 cells (total 4)
    shell.advance();
    ASSERT_EQUAL(4, shell.getDistanceTraveled());
    ASSERT_FALSE(shell.isActive()); // Should deactivate after reaching max range
    
    return true;
}

bool test_shell_owner() {
    Shell shell(5, 5, Direction::R);
    
    // Default owner ID
    ASSERT_EQUAL(-1, shell.getOwnerID());
    
    // Set owner ID
    shell.setOwnerID(1);
    ASSERT_EQUAL(1, shell.getOwnerID());
    
    return true;
}

int main() {
    TestFramework framework;
    framework.begin_test_suite("Shell Tests");
    
    framework.register_test("Shell Initialization", test_shell_initialization);
    framework.register_test("Shell Movement", test_shell_movement);
    framework.register_test("Shell Update", test_shell_update);
    framework.register_test("Shell Deactivation", test_shell_deactivation);
    framework.register_test("Shell Max Range", test_shell_max_range);
    framework.register_test("Shell Owner", test_shell_owner);
    
    return framework.run_all_tests() ? 0 : 1;
}
