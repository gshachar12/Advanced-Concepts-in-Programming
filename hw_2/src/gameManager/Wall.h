#ifndef WALL_H
#define WALL_H

#include "GameObject.h"

static constexpr int MAX_HEALTH = 2;

class Wall : public GameObject {
    int health = MAX_HEALTH;

public:
    explicit Wall(const Position position): GameObject(position) {
        setObjectType(CellType::WALL);
        setDirection(Direction::NONE);
    }

    [[nodiscard]] char getSymbol() const override { return '#'; }

    void takeDamage(const int amount = 1) override {
        if (health > 0) health -= amount;
    }

    [[nodiscard]] int getHitCount() const { return MAX_HEALTH - health; }

    [[nodiscard]] virtual int getHealth() const { return health; }

    [[nodiscard]] bool isDestroyed() const override { return health == 0; }

    void destroy() override { health = 0; }

    [[nodiscard]] bool isWall() const override { return true; }
    
    [[nodiscard]] bool isDestroyable() const override { return true; }
    [[nodiscard]] bool isCollidable() const override { return true; }
    [[nodiscard]] bool isPassable() const override { return false; }
    [[nodiscard]] int getHitPoints() const override { return MAX_HEALTH; }
};

#endif //WALL_H
