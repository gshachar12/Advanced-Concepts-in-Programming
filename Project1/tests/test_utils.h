#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <iostream>
#include <string>
#include <vector>
#include <functional>

// Simple test framework
class TestFramework {
private:
    struct TestCase {
        std::string name;
        std::function<bool()> test_func;
        bool run_result;
    };
    
    std::vector<TestCase> tests;
    std::string current_suite;
    int passed = 0;
    int failed = 0;

public:
    void begin_test_suite(const std::string& suite_name) {
        current_suite = suite_name;
        std::cout << "\n=== Running Test Suite: " << suite_name << " ===\n";
    }
    
    void register_test(const std::string& test_name, std::function<bool()> test_func) {
        tests.push_back({test_name, test_func, false});
    }
    
    bool run_all_tests() {
        passed = 0;
        failed = 0;
        
        for (auto& test : tests) {
            std::cout << "Running: " << test.name << "... ";
            try {
                test.run_result = test.test_func();
                if (test.run_result) {
                    std::cout << "PASSED" << std::endl;
                    passed++;
                } else {
                    std::cout << "FAILED" << std::endl;
                    failed++;
                }
            } catch (const std::exception& e) {
                std::cout << "EXCEPTION: " << e.what() << std::endl;
                test.run_result = false;
                failed++;
            }
        }
        
        std::cout << "\n=== Test Results: " << current_suite << " ===\n";
        std::cout << "Passed: " << passed << ", Failed: " << failed << std::endl;
        
        return failed == 0;
    }
};

// Helper assertion macros
#define ASSERT_TRUE(condition) \
    if (!(condition)) { \
        std::cout << "Assertion failed: " << #condition << " is not true"; \
        std::cout << " (at " << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
        return false; \
    }

#define ASSERT_FALSE(condition) \
    if (condition) { \
        std::cout << "Assertion failed: " << #condition << " is not false"; \
        std::cout << " (at " << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
        return false; \
    }

#define ASSERT_EQUAL(expected, actual) \
    if ((expected) != (actual)) { \
        std::cout << "Assertion failed: " << #expected << " != " << #actual; \
        std::cout << " (" << (expected) << " != " << (actual) << ")"; \
        std::cout << " (at " << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
        return false; \
    }

#endif // TEST_UTILS_H
