#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <map>
#include <memory>
#include <vector>

#include "GameObject.h"
#include "Mine.h"
#include "MySatelliteView.h"
#include "Shell.h"
#include "Tank.h"
#include "Wall.h"

enum ObjectType {
    TANK_1,
    TANK_2,
    MINE,
    WALL,
};

class Board {
    std::string desc;
    size_t max_steps;
    size_t num_shells;
    size_t width = 2;
    size_t height = 2;
    std::vector<std::vector<std::unique_ptr<GameObject> > > board;
    std::map<std::pair<int, int>, Position> tanks_pos;
    std::map<int, Position> shells_pos;
    std::map<int, Position> collisions_pos;
    std::map<int, Position> moving_pos;
    std::vector<std::unique_ptr<GameObject> > destroyed;

    GameObject *placeObjectReal(std::unique_ptr<GameObject> element, Position real_pos);

    bool isOccupiedReal(Position real_pos) const;

    void removeObjectReal(Position real_pos);

    GameObject *replaceObjectReal(Position from_real, Position to_real);

    Position wrapPositionReal(Position real_pos) const;

    GameObject *getObjectAtReal(Position real_pos) const;

    GameObject *moveObjectReal(Position from_real, Direction::DirectionType dir);

    void removeIndices(GameObject *game_object);

    void checkCollisions();

    void print_info() {
        std::cout << "Description: " << desc << ", max_steps: " << max_steps << ", num_shells: " << num_shells <<
                std::endl;
    }

    // block copy and move
    Board(const Board &) = delete;

    Board &operator=(const Board &) = delete;

    Board(Board &&) = delete;

    Board &operator=(Board &&) = delete;

public:
    Board();

    Board(std::string desc, size_t max_steps, size_t num_shells, size_t width, size_t height);

    [[nodiscard]] int getHeight() const { return height / 2; }

    [[nodiscard]] int getWidth() const { return width / 2; }

    [[nodiscard]] bool isOccupied(Position pos) const;

    template<typename T>
    T *placeObject(std::unique_ptr<T> element);

    void removeObject(Position pos);

    GameObject *replaceObject(Position from, Position to);

    GameObject *moveObject(Position from, Direction::DirectionType dir);

    std::vector<Tank *> getPlayerAliveTanks(int player_index) const;

    std::vector<Tank *> getPlayerTanks(int player_index) const;

    Tank *getPlayerTank(int player_index, int tank_index) const;

    Position wrapPosition(Position pos) const;

    GameObject *getObjectAt(Position pos) const;

    std::vector<Tank *> getAliveTanks() const;

    std::vector<Tank *> getTanks() const;

    void finishMove();

    std::map<int, Shell *> getShells() const;
    
    std::map<int, Position> getCollisions() const { return collisions_pos; }

    bool isWall(const Position pos) const { return isOccupied(pos) && getObjectAt(pos)->isWall(); }

    bool isTank(const Position pos) const { return isOccupied(pos) && getObjectAt(pos)->isTank(); }

    bool isShell(const Position pos) const { return isOccupied(pos) && getObjectAt(pos)->isShell(); }

    bool isCollision(const Position pos) const { return isOccupied(pos) && getObjectAt(pos)->isCollision(); }

    bool isMine(const Position pos) const { return isOccupied(pos) && getObjectAt(pos)->isMine(); }

    size_t getNumShells() const { return num_shells; }

    size_t getMaxSteps() const { return max_steps; }

    void fillSatelliteView(MySatelliteView &satellite_view) const;
    
    bool shoot(Position fromPos, Position toPos);

    ~Board() = default;
};

template<typename T>
T *Board::placeObject(std::unique_ptr<T> element) {
    static_assert(
        std::is_same_v<GameObject, Tank> ||
        std::is_same_v<GameObject, Wall> ||
        std::is_same_v<GameObject, Shell> ||
        std::is_same_v<GameObject, Mine> ||
        std::is_same_v<GameObject, GameObject>
    );
    if (element == nullptr) return nullptr;
    Position pos = element->getPosition();
    return dynamic_cast<T *>(placeObjectReal(std::move(element), pos * 2));
}

#endif //BOARD_H
