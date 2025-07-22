#include "../../src/Board.h"
#include "../test_utils.h"
#include <fstream>
#include <cstdio>

// Helper function to create temporary test board files
std::string create_temp_board_file(const std::string& content) {
    std::string filename = "temp_test_board.txt";
    std::ofstream out(filename);
    out << content;
    out.close();
    return filename;
}

// Delete temporary file after use
void cleanup_temp_file(const std::string& filename) {
    std::remove(filename.c_str());
}

bool test_board_loading_valid() {
    std::string content = "5 4\n"
                         "#####\n"
                         "#1 2#\n"
                         "# @ #\n"
                         "#####\n";
    
    std::string filename = create_temp_board_file(content);
    
    Board board;
    bool result = board.loadFromFile(filename);
    
    ASSERT_TRUE(result);
    ASSERT_EQUAL(5, board.getWidth());
    ASSERT_EQUAL(4, board.getHeight());
    
    // Check specific cells
    ASSERT_EQUAL(CellType::WALL, board.getCellType(0, 0));
    ASSERT_EQUAL(CellType::TANK1, board.getCellType(1, 1));
    ASSERT_EQUAL(CellType::TANK2, board.getCellType(3, 1));
    ASSERT_EQUAL(CellType::MINE, board.getCellType(2, 2));
    
    cleanup_temp_file(filename);
    return true;
}

bool test_board_loading_invalid() {
    std::string content = "abc def\n#####\n";
    std::string filename = create_temp_board_file(content);
    
    Board board;
    bool result = board.loadFromFile(filename);
    
    ASSERT_FALSE(result); // Should fail to load
    
    cleanup_temp_file(filename);
    return true;
}

bool test_board_cell_manipulation() {
    Board board;
    board.setCellType(1, 2, CellType::WALL);
    ASSERT_EQUAL(CellType::WALL, board.getCellType(1, 2));
    
    board.setCellType(1, 2, CellType::EMPTY);
    ASSERT_EQUAL(CellType::EMPTY, board.getCellType(1, 2));
    
    return true;
}

bool test_board_wall_damage() {
    std::string content = "3 3\n"
                         "###\n"
                         "# #\n"
                         "###\n";
                         
    std::string filename = create_temp_board_file(content);
    
    Board board;
    board.loadFromFile(filename);
    
    // Test wall weakening
    ASSERT_EQUAL(CellType::WALL, board.getCellType(0, 0));
    
    // First hit
    board.weakenWall(0, 0);
    ASSERT_EQUAL(CellType::WALL, board.getCellType(0, 0)); // Still a wall
    
    // Second hit
    board.weakenWall(0, 0);
    ASSERT_EQUAL(CellType::EMPTY, board.getCellType(0, 0)); // Should be destroyed
    
    cleanup_temp_file(filename);
    return true;
}

bool test_board_bounds_checking() {
    Board board;
    board.setCellType(0, 0, CellType::WALL);
    
    // Out-of-bounds access
    CellType type = board.getCellType(999, 999);
    ASSERT_EQUAL(CellType::UNKNOWN, type); // Should return UNKNOWN for out-of-bounds
    
    return true;
}

int main() {
    TestFramework framework;
    framework.begin_test_suite("Board Tests");
    
    framework.register_test("Board Loading - Valid", test_board_loading_valid);
    framework.register_test("Board Loading - Invalid", test_board_loading_invalid);
    framework.register_test("Board Cell Manipulation", test_board_cell_manipulation);
    framework.register_test("Board Wall Damage", test_board_wall_damage);
    framework.register_test("Board Bounds Checking", test_board_bounds_checking);
    
    return framework.run_all_tests() ? 0 : 1;
}
