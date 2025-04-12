#include <iostream>
#include <vector>
#include "header_files/utilities.h"
using namespace std;



void displayMap(const std::vector<std::vector<char>>& map) {
    // Iterate through the 2D map and display the appropriate emoji for each character
    for (const auto& row : map) {
        for (const auto& cell : row) {
            switch (cell) {
                case '#': 
                    std::cout << "🟩 ";  // Wall
                    break;
                case '1':
                    std::cout << "🚗 ";  // Player 1's tank
                    break;
                case '2':
                    std::cout << "🚙 ";  // Player 2's tank
                    break;
                case '@':
                    std::cout << "💥 ";  // Mine
                    break;
                case ' ':
                    std::cout << "⬜ ";  // Empty space (white square)
                    break;
                default:
                    std::cout << "  ";  // Default to empty space
                    break;
            }
        }
        std::cout << std::endl;
    }
}
