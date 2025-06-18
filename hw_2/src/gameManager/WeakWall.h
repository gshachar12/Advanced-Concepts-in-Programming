#ifndef WEAK_WALL_H
#define WEAK_WALL_H

#include "Wall.h"

class WeakWall : public Wall {
    // Override the health value for weak wall
    int health = 1; 
    
public:
    explicit WeakWall(Position pos) : Wall(pos) {
        setObjectType(CellType::WEAK_WALL);
        setDirection(Direction::NONE);
    }

    [[nodiscard]] char getSymbol() const override { return '='; }
    [[nodiscard]] char getDisplayChar() const override { return '='; } // Using a wall-like symbol
    [[nodiscard]] bool isDestroyable() const override { return true; }
    [[nodiscard]] bool isCollidable() const override { return true; }
    [[nodiscard]] bool isPassable() const override { return false; }
    [[nodiscard]] int getHitPoints() const override { return 1; }
    [[nodiscard]] int getHealth() const override { return health; }
    [[nodiscard]] bool isDestroyed() const override { return health <= 0; }
    
    void takeDamage(const int amount = 1) override {
        if (health > 0) health -= amount;
    }
    
    void destroy() override { health = 0; }
    
    [[nodiscard]] bool isWall() const override { return true; }
};

#endif // WEAK_WALL_H