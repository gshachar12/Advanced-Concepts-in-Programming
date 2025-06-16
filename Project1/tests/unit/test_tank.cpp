#include "../../src/Tank.h"
#include "../test_utils.h"

bool test_tank_initialization() {
    Tank tank(5, 10, "UR");
    
    ASSERT_EQUAL(5, tank.getX());
    ASSERT_EQUAL(10, tank.getY());
    ASSERT_EQUAL(Direction::UR, tank.getDirection());
    ASSERT_TRUE(tank.isAlive());
    ASSERT_TRUE(tank.canShoot());
    
    return true;
}

bool test_tank_movement() {
    Tank tank(5, 5, "R");
    
    // Test forward movement
    tank.moveForward();
    ASSERT_EQUAL(6, tank.getX());
    ASSERT_EQUAL(5, tank.getY());
    
    // Test backward movement
    tank.moveBackward();
    ASSERT_EQUAL(5, tank.getX());
    ASSERT_EQUAL(5, tank.getY());
    
    return true;
}

bool test_tank_rotation() {
    Tank tank(5, 5, "U");
    
    // Test rotation
    tank.rotateRight1_8();
    ASSERT_EQUAL(Direction::UR, tank.getDirection());
    
    tank.rotateRight1_8();
    ASSERT_EQUAL(Direction::R, tank.getDirection());
    
    tank.rotateLeft1_4();
    ASSERT_EQUAL(Direction::U, tank.getDirection());
    
    return true;
}

bool test_tank_backward_state_machine() {
    Tank tank(5, 5, "R");
    
    // Initial state
    ASSERT_EQUAL(BackwardState::NOT_REQUESTED, tank.getBackwardState());
    
    // Request backward movement
    tank.requestBackward();
    ASSERT_EQUAL(BackwardState::WAITING_1, tank.getBackwardState());
    
    // First update
    tank.update();
    ASSERT_EQUAL(BackwardState::WAITING_2, tank.getBackwardState());
    
    // Second update
    tank.update();
    ASSERT_EQUAL(BackwardState::MOVING_BACKWARD, tank.getBackwardState());
    
    // Third update should execute backward move and reset state
    int oldX = tank.getX();
    tank.update();
    ASSERT_EQUAL(BackwardState::NOT_REQUESTED, tank.getBackwardState());
    ASSERT_EQUAL(oldX - 1, tank.getX()); // Should have moved left (backward from right)
    
    return true;
}

bool test_tank_shooting() {
    Tank tank(5, 5, "U");
    
    // Initial state
    ASSERT_TRUE(tank.canShoot());
    
    // After shooting
    tank.shoot();
    ASSERT_FALSE(tank.canShoot()); // On cooldown
    
    // Wait for cooldown to expire
    for (int i = 0; i < 5; i++) {
        tank.update();
    }
    
    ASSERT_TRUE(tank.canShoot()); // Should be able to shoot again
    
    return true;
}

bool test_tank_destruction() {
    Tank tank(5, 5, "U");
    
    ASSERT_TRUE(tank.isAlive());
    
    tank.destroy();
    
    ASSERT_FALSE(tank.isAlive());
    
    return true;
}

int main() {
    TestFramework framework;
    framework.begin_test_suite("Tank Tests");
    
    framework.register_test("Tank Initialization", test_tank_initialization);
    framework.register_test("Tank Movement", test_tank_movement);
    framework.register_test("Tank Rotation", test_tank_rotation);
    framework.register_test("Tank Backward State Machine", test_tank_backward_state_machine);
    framework.register_test("Tank Shooting", test_tank_shooting);
    framework.register_test("Tank Destruction", test_tank_destruction);
    
    return framework.run_all_tests() ? 0 : 1;
}
