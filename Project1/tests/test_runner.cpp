#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>

// Tests to run - add new test executables here
const std::vector<std::string> TEST_EXECUTABLES = {
    "./tests/unit/test_board",
    "./tests/unit/test_tank",
    "./tests/unit/test_shell",
    "./tests/integration/test_collision",
    "./tests/system/test_end_to_end"
};

int main(int argc, char* argv[]) {
    std::cout << "===== Running Tank Game Tests =====\n" << std::endl;

    int failed_tests = 0;

    for (const auto& test : TEST_EXECUTABLES) {
        std::cout << "\n----- Running " << test << " -----" << std::endl;
        int result = std::system(test.c_str());
        
        if (result != 0) {
            std::cout << "Test " << test << " FAILED!" << std::endl;
            failed_tests++;
        }
    }

    std::cout << "\n===== Test Summary =====" << std::endl;
    std::cout << "Total test suites: " << TEST_EXECUTABLES.size() << std::endl;
    std::cout << "Passed: " << (TEST_EXECUTABLES.size() - failed_tests) << std::endl;
    std::cout << "Failed: " << failed_tests << std::endl;

    return failed_tests ? 1 : 0;
}
