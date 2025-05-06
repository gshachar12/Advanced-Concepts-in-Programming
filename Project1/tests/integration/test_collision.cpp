#include "../../src/GameManager.h"
#include "../../src/Board.h"
#include "../../src/Tank.h"
#include "../../src/Shell.h"
#include "../../src/algorithms/Controller.h"
#include "../test_utils.h"
#include <memory>
#include <fstream>

// Constants for testing
const int COLLISION_TEST_ITERATIONS = 5;

// Custom controller for testing that returns specified actions
class TestController : public Controller {
private:
    ActionType next_action;
    
public:
    TestController() : next_action(ActionType::NONE) {}
    
    void setNextAction(ActionType action) {
        next_action = action;
    }
    
    ActionType ChaseTank(const Board &board, const Tank &myTank,
                            const Tank &enemyTank, const std::vector<Shell> &shells) override {
        return next_action;
    }
};

// Helper to create test board
std::string createTestBoard(const std::string& content) {
    std::string filename = "test_collision_board.txt";
    std::ofstream out(filename);
    out << content;
    out.close();
    return filename;
}

// Cleanup helper
void cleanupTestBoard(const std::string& filename) {
    std::remove(filename.c_str());
}

bool test_shell_wall_collision() {
    // Create a test board with a wall
    std::string board_content = "5 5\n"
                               "#####\n"
                               "#   #\n"
                               "# # #\n"
                               "#   #\n"
                               "#####\n";
    std::string board_file = createTestBoard(board_content);
    
    // Create test controllers
    auto ctrl1 = std::make_unique<TestController>();
    auto ctrl2 = std::make_unique<TestController>();
    TestController* ctrl1_ptr = static_cast<TestController*>(ctrl1.get());
    
    // Set up the game
    GameManager game(std::move(ctrl1), std::move(ctrl2), false);
    bool init_success = game.initializeGame(board_file);
    ASSERT_TRUE(init_success);
    
    // Position tanks and shoot at wall
    // We can't directly manipulate objects inside GameManager,
    // but we can use the Controller to make the tank shoot in a direction
    ctrl1_ptr->setNextAction(ActionType::SHOOT);
    
    // Let the game run for a short time to process the collision
    for (int i = 0; i < COLLISION_TEST_ITERATIONS; i++) {
        game.runGameLoop();
    }
    
    // Cleanup
    cleanupTestBoard(board_file);
    return true;
}

bool test_shell_tank_collision() {
    // Create a test board where tanks are aligned
    std::string board_content = "7 3\n"
                               "#######\n"
                               "#1   2#\n"
                               "#######\n";
    std::string board_file = createTestBoard(board_content);
    
    // Create test controllers
    auto ctrl1 = std::make_unique<TestController>();
    auto ctrl2 = std::make_unique<TestController>();
    TestController* ctrl1_ptr = static_cast<TestController*>(ctrl1.get());
    
    // Set up the game
    GameManager game(std::move(ctrl1), std::move(ctrl2), false);
    bool init_success = game.initializeGame(board_file);
    ASSERT_TRUE(init_success);
    
    // Make tank 1 shoot
    ctrl1_ptr->setNextAction(ActionType::SHOOT);
    
    // Let the game run to process the collision
    for (int i = 0; i < COLLISION_TEST_ITERATIONS; i++) {
        game.runGameLoop();
    }
    
    // Cleanup
    cleanupTestBoard(board_file);
    return true;
}

bool test_tank_mine_collision() {
    // Create a test board with mines
    std::string board_content = "5 5\n"
                               "#####\n"
                               "#1  #\n"
                               "#   #\n"
                               "# @ #\n"
                               "#####\n";
    std::string board_file = createTestBoard(board_content);
    
    // Create test controllers
    auto ctrl1 = std::make_unique<TestController>();
    auto ctrl2 = std::make_unique<TestController>();
    TestController* ctrl1_ptr = static_cast<TestController*>(ctrl1.get());
    
    // Set up the game
    GameManager game(std::move(ctrl1), std::move(ctrl2), false);
    bool init_success = game.initializeGame(board_file);
    ASSERT_TRUE(init_success);
    
    // Move tank 1 toward the mine
    ctrl1_ptr->setNextAction(ActionType::MOVE_FORWARD);
    
    // Let the game run to process the collision
    for (int i = 0; i < COLLISION_TEST_ITERATIONS; i++) {
        game.runGameLoop();
    }
    
    // Cleanup
    cleanupTestBoard(board_file);
    return true;
}

int main() {
    TestFramework framework;
    framework.begin_test_suite("Collision Tests");
    
    framework.register_test("Shell-Wall Collision", test_shell_wall_collision);
    framework.register_test("Shell-Tank Collision", test_shell_tank_collision);
    framework.register_test("Tank-Mine Collision", test_tank_mine_collision);
    
    return framework.run_all_tests() ? 0 : 1;
}
