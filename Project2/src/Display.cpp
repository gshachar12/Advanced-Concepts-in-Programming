#include "Display.h"
#include <iostream>

DisplayManager::DisplayManager(int cellSize)
    : cellSize(cellSize),
      window(sf::VideoMode(1024, 768), "Tank Game Viewer") {}

bool DisplayManager::loadTextures() {
    const std::vector<std::string> cellTypes = {
        "wall", "weaken_wall", "mine", "boom", "empty"
    };
    for (const auto& name : cellTypes) {
        sf::Texture tex;
        if (!tex.loadFromFile("img/" + name + ".png")) {
            std::cerr << "Failed to load img/" << name << ".png" << std::endl;
            return false;
        }
        textures[name] = tex;
    }

    const std::vector<std::string> dirs = {
        "up", "ur", "right", "dr", "down", "dl", "left", "ul"
    };
    for (const auto& dir : dirs) {
        for (int i = 1; i <= 2; ++i) {
            std::string tankName = "tank" + std::to_string(i) + "_" + dir;
            std::string shellName = "r" + std::to_string(i) + "_" + dir;

            sf::Texture tex;
            if (!tex.loadFromFile("img/" + tankName + ".png")) {
                std::cerr << "Failed to load img/" << tankName << ".png" << std::endl;
                return false;
            }
            textures[tankName] = tex;
                        if (!tex.loadFromFile("img/" + shellName + ".png")) {
                std::cerr << "Failed to load img/" << shellName << ".png" << std::endl;
                return false;
            }
            textures[shellName] = tex;
        }
    }

    return true;
}

void DisplayManager::display(const Board &board, const std::vector<std::unique_ptr<Tank>> &tanks,
 const std::vector<std::unique_ptr<GameObject>> &shells, const std::string &gameResultMessage, bool gameOver){
    window.clear(sf::Color::Black);

    drawBoard(board);
    drawShells(shells);
    drawTanks(tanks);

    if (gameOver) {
        drawGameOverText(gameResultMessage);
    }

    window.display();
    waitForKeyPress();
}

std::string DisplayManager::getDirectionSuffix(Direction dir) {
    switch (dir) {
        case Direction::UP: return "up";
        case Direction::UP_RIGHT: return "ur";
        case Direction::RIGHT: return "right";
        case Direction::DOWN_RIGHT: return "dr";
        case Direction::DOWN: return "down";
        case Direction::DOWN_LEFT: return "dl";
        case Direction::LEFT: return "left";
        case Direction::UP_LEFT: return "ul";
        default: return "up";
    }
}

void DisplayManager::drawBoard(const Board& board) {
    for (int y = 0; y < board.getHeight(); ++y) {
        for (int x = 0; x < board.getWidth(); ++x) {
            sf::Sprite sprite;
            switch (board.getCellType(x, y)) {
                case CellType::WALL: sprite.setTexture(textures["wall"]); break;
                case CellType::WEAK_WALL: sprite.setTexture(textures["weak_wall"]); break;
                case CellType::MINE: sprite.setTexture(textures["mine"]); break;
                case CellType::BOOM: sprite.setTexture(textures["boom"]); break;
                case CellType::EMPTY:
                default: sprite.setTexture(textures["empty"]); break;
            }
            sprite.setPosition(x * cellSize, y * cellSize);
            window.draw(sprite);
        }
    }
}

void DisplayManager::drawShells(const std::vector<std::unique_ptr<GameObject>>& shells) {
    for (const auto& shell : shells) {
        if (shell->getObjectType() == CellType::SHELL) {
            sf::Sprite sprite;
            sprite.setTexture(textures["shell"]);
            sprite.setPosition(shell->getX() * cellSize, shell->getY() * cellSize);
            window.draw(sprite);
        }
    }
}

void DisplayManager::drawTanks(const std::vector<std::unique_ptr<Tank>>& tanks) {
    for (const auto& tank : tanks) {
        if (tank->isAlive()) {
            std::string suffix = getDirectionSuffix(tank->getDirection());
            std::string key = (tank->getObjectType() == CellType::TANK1)
                            ? "tank1_" + suffix
                            : "tank2_" + suffix;

            sf::Sprite sprite;
            sprite.setTexture(textures[key]);
            sprite.setPosition(tank->getX() * cellSize, tank->getY() * cellSize);
            window.draw(sprite);
        }
    }
}

void DisplayManager::drawGameOverText(const std::string& msg) {
    sf::Font font;
    if (!font.loadFromFile("img/font.ttf")) {
        std::cerr << "Missing font: img/font.ttf" << std::endl;
        return;
    }

    sf::Text text;
    text.setFont(font);
    text.setString("🏁 " + msg);
    text.setCharacterSize(36);
    text.setFillColor(sf::Color::White);
    text.setPosition(50, 20);

    window.draw(text);
}

void DisplayManager::waitForKeyPress() {
    while (true) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::KeyPressed || event.type == sf::Event::Closed)
                return;
        }
    }
}
