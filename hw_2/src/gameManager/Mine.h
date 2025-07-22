#ifndef MINE_H
#define MINE_H

#include "GameObject.h"

class Mine final : public GameObject {
public:
    explicit Mine(const Position position): GameObject(position) {
        setObjectType(CellType::MINE);
        setDirection(Direction::NONE);
    }

    [[nodiscard]] char getSymbol() const override { return '@'; }
    [[nodiscard]] bool isMine() const override { return true; }
    [[nodiscard]] bool isDestroyable() const override { return true; }
    [[nodiscard]] bool isCollidable() const override { return true; }
    [[nodiscard]] bool isPassable() const override { return false; }
};

#endif //MINE_H
