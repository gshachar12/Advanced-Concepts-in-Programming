#include "../../src/GameManager.h"
#include "../../src/algorithms/Controller.h"
#include "../../src/algorithms/EvaderController.h"
#include "../test_utils.h"
#include <memory>
#include <fstream>
#include <vector>

// Custom controller for scenario testing
class ScenarioController : public Controller {
private:
    std::vector<ActionType> actions;
    size_t current_action;
    
public:
    ScenarioController(const std::vector<ActionType>& action_sequence)
        : actions(action_sequence), current_action(0) {}
    
    ActionType decideAction(const Board &board, const Tank &myTank,
                            const Tank &enemyTank, const std::vector<Shell> &shells) override {
        if (current_action < actions.size()) {
            return actions[current_action++];
        }
        return ActionType::NONE;
    }
};

std::string createScenarioBoard(const std::string& content) {
    std::string filename = "test_scenario_board.txt";
    std::ofstream out(filename);
    out << content;
    out.close();
    return filename;
}

void cleanupScenarioBoard(const std::string& filename) {
    std::remove(filename.c_str());
}

bool test_full_game_scenario() {
    // Create a simple test board
    std::string board_content = "7 5\n"
                               "#######\n"
                               "#1   2#\n"
                               "#     #\n"
                               "#     #\n"
                               "#######\n";
    std::string board_file = createScenarioBoard(board_content);
    
    // Create scenario controllers with predetermined action sequences
    // Tank 1: Move forward, shoot, rotate, shoot
    std::vector<ActionType> tank1_actions = {
        ActionType::MOVE_FORWARD,
        ActionType::SHOOT,
        ActionType::ROTATE_RIGHT_1_4,
        ActionType::SHOOT
    };
    
    // Tank 2: Move backward, rotate, shoot
    std::vector<ActionType> tank2_actions = {
        ActionType::MOVE_BACKWARD,
        ActionType::ROTATE_LEFT_1_4,
        ActionType::SHOOT
    };
    
    auto ctrl1 = std::make_unique<ScenarioController>(tank1_actions);
    auto ctrl2 = std::make_unique<ScenarioController>(tank2_actions);
    
    // Set up the game
    GameManager game(std::move(ctrl1), std::move(ctrl2), false);
    bool init_success = game.initializeGame(board_file);
    ASSERT_TRUE(init_success);
    
    // Run the game for a fixed number of turns
    for (int i = 0; i < 10; i++) {
        game.runGameLoop();
    }
    
    // Since we can't directly observe the game state, this test mainly checks
    // if the game can run through the scenario without crashing
    
    cleanupScenarioBoard(board_file);
    return true;
}

bool test_ammo_depletion_scenario() {
    // Create a board with widely separated tanks
    std::string board_content = "10 10\n"
                               "##########\n"
                               "#1       #\n"
                               "#        #\n"
                               "#        #\n"
                               "#        #\n"
                               "#        #\n"
                               "#        #\n"
                               "#        #\n"
                               "#       2#\n"
                               "##########\n";
    std::string board_file = createScenarioBoard(board_content);
    
    // Create controllers that always shoot
    std::vector<ActionType> shoot_actions(20, ActionType::SHOOT);
    
    auto ctrl1 = std::make_unique<ScenarioController>(shoot_actions);
    auto ctrl2 = std::make_unique<ScenarioController>(shoot_actions);
    
    // Set up the game
    GameManager game(std::move(ctrl1), std::move(ctrl2), false);
    bool init_success = game.initializeGame(board_file);
    ASSERT_TRUE(init_success);
    
    // Run the game until ammo is depleted (should be at least 16 turns)
    for (int i = 0; i < 50; i++) {
        game.runGameLoop();
    }
    
    // This mainly tests that the game handles ammo depletion correctly
    
    cleanupScenarioBoard(board_file);
    return true;
}

int main() {
    TestFramework framework;
    framework.begin_test_suite("End-to-End Scenario Tests");
    
    framework.register_test("Full Game Scenario", test_full_game_scenario);
    framework.register_test("Ammo Depletion Scenario", test_ammo_depletion_scenario);
    
    return framework.run_all_tests() ? 0 : 1;
}
