/*
*  Tanks Game
*  Made By: Shachar Gabbay And Dor Liberman
*  May 2025
*  
*
*/


#include <iostream>
#include <string>
#include <memory>
#include <fstream>
#include <filesystem>

#include "Board.h"
#include "Tank.h"
#include "Shell.h"
#include "GameManager.h"
#include "GameObject.h"

using namespace std;

auto board = std::make_unique<Board>();

// Function to remove the file extension (.txt) if present
std::string extractFileName(const std::string& filePath) {
    // Find the position of the last '/'
    size_t startPos = filePath.find_last_of("/\\") + 1;

    // Find the position of the first '.' after the '/'
    size_t endPos = filePath.find('.', startPos);

    // Extract the substring between the two positions
    return filePath.substr(startPos, endPos - startPos);
}
bool openFile(std::ifstream &fin, const std::string &filename)
{
    int w,h; 
    if (!fin.is_open()) {
        std::cerr << "Error: Could not open file '" << filename << "'\n";
        exit(1); 

    }

    // Read width, height
    if (!(fin >> w >> h )) {
        std::cerr << "Error: Failed to read board dimensions.\n";
        exit(1); 

    }

    board->setWidth(w);  // Set the width using the setter method
    board->setHeight(h);  // Set the height using the setter method

    if (board->getWidth() <= 0 || board->getHeight() <= 0) {
        std::cerr << "Error: Invalid board dimensions.\n";
        exit(1); 
    }


    return true; 
}

bool loadFromFile(const std::string &filename, Tank* tank1, Tank* tank2) {
    // Ignore the rest of the line
    std::ifstream fin(filename);
    openFile(fin, filename);
    fin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    // Resize grid and wallInfo
    board->grid.resize(board->getHeight(), std::vector<CellType>(board->getWidth(), CellType::EMPTY));
    board->wallInfo.resize(board->getHeight(), std::vector<WallDamage>(board->getWidth()));
    for (int row = 0; row < board->getHeight(); ++row) {
        std::string line;
        if (!std::getline(fin, line)) {
            break;
        }

        for (int col = 0; col < board->getWidth(); ++col) {
            char c = (col < (int)line.size()) ? line[col] : ' ';
            CellType type = Board::charToCellType(c);
            board->grid[row][col] = type;
            if (type == CellType::TANK1 && tank1) {
                tank1->setPosition(col, row);
            }
            else if (type == CellType::TANK2 && tank2) {
                tank2->setPosition(col, row);
            }
            // If it's a wall, set wallInfo
            if (type == CellType::WALL) {
                board->wallInfo[row][col].isWall = true;
                board->wallInfo[row][col].hitsTaken = 0;
            }
        }
    }

    fin.close();
    return true;
}



int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <board_file> [--visual]" << endl;
        return 1;
    }

    string filename = argv[1];

    bool visual_mode = false;
    if (argc >= 3) {
        string modeArg = argv[2];
        if (modeArg == "--visual") {
            visual_mode = true;
            cout << "Running the game in visual mode." << endl;
        } else {
            cout << "Running the game in text mode." << endl;
        }
    } else {
        cout << "Running the game in text mode." << endl;
    }


    // Load board
        // Create tanks with custom starting positions and directions
    Tank t1(0, 0, "L", CellType::TANK1, 1);  
    Tank t2(0, 0, "R", CellType::TANK2, 2);  


    if (!loadFromFile(filename, &t1, &t2)) {
        cerr << "Error: Failed to load board from " << filename << endl;
        return 1;
    }
    int width = board->getWidth();
    int height = board->getHeight();

    cout << "width: " << width << " height: " << height << endl;


    // Create minimal stub controllers
    auto ctrl1 = make_unique<Controller>();
    auto ctrl2 = make_unique<Controller>();
    std::string boardFileName = extractFileName(filename); // "exampleBoard"
    
    // Construct the output file name
    std::string outputFileName = "outputs/output_" + boardFileName + ".txt";

    // Create the GameManager using the new constructor
    GameManager gameManager(t1, t2, move(ctrl1), move(ctrl2), move(board), visual_mode);

    // Initialize game
    if (!gameManager.initializeGame()) {
        cerr << "Error during game initialization." << endl;
        return 1;
    }

    // Run the game loop
    gameManager.runGameLoop(outputFileName);

    return 0;
}
