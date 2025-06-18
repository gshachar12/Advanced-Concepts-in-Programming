#pragma once

#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <memory>
#include "Board.h"
#include "Tank.h"
#include "Shell.h"

class DisplayManager {
public:
    DisplayManager(int cellSize = 32);
    bool loadTextures();
void display(const Board &board, const std::vector<std::unique_ptr<Tank>> &tanks,
 const std::vector<std::unique_ptr<GameObject>> &shells, const std::string &gameResultMessage, bool gameOver);

private:
    int cellSize;
    sf::RenderWindow window;
    std::map<std::string, sf::Texture> textures;

    std::string getDirectionSuffix(Direction dir);
    void drawBoard(const Board& board);
    void drawShells(const std::vector<std::unique_ptr<GameObject>>& shells);
    void drawTanks(const std::vector<std::unique_ptr<Tank>>& tanks);
    void drawGameOverText(const std::string& msg);
    void waitForKeyPress();
};
