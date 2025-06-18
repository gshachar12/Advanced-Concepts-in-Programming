#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Direction.h"

// Define CellType enum
enum class CellType {
    EMPTY,
    WALL,
    WEAK_WALL,
    MINE,
    TANK,
    SHELL
};

static int static_id = 0;

class GameObject {
    GameObject(const GameObject &) = delete;

    GameObject &operator=(const GameObject &) = delete;

    GameObject(GameObject &&) = delete;

    GameObject &operator=(GameObject &&) = delete;

protected:
    int id;
    Position position;
    Direction::DirectionType direction = Direction::UP;
    bool destroyed = false;

public:
    virtual ~GameObject() = default;

    explicit GameObject(const Position position,
                        const Direction::DirectionType direction): id(static_id++), position(position),
                                                                   direction(direction) {
    }

    explicit GameObject(const Position position): id(static_id++), position(position) {
    }

    [[nodiscard]] virtual char getSymbol() const { return ' '; }

    virtual int getId() const { return id; }

    [[nodiscard]] virtual Position getPosition() const { return position; }

    virtual void setPosition(const Position pos) { position = pos; }

    [[nodiscard]] virtual Direction::DirectionType getDirection() const { return direction; }

    virtual void setDirection(const Direction::DirectionType &dir) { direction = dir; }

    virtual void destroy() { destroyed = true; }

    [[nodiscard]] virtual bool isDestroyed() const { return destroyed; }

    virtual bool isWall() const { return false; }

    virtual bool isShell() const { return false; }

    virtual bool isMine() const { return false; }

    virtual bool isCollision() const { return false; }

    virtual bool isTank() const { return false; }
    
    // Add missing virtual methods
    virtual void setObjectType(CellType type) { (void)type; /* Default implementation does nothing */ }
    [[nodiscard]] virtual CellType getObjectType() const { return CellType::EMPTY; }
    [[nodiscard]] virtual CellType getType() const { return CellType::EMPTY; }
    [[nodiscard]] virtual char getDisplayChar() const { return getSymbol(); }
    [[nodiscard]] virtual bool isDestroyable() const { return false; }
    [[nodiscard]] virtual bool isCollidable() const { return false; }
    [[nodiscard]] virtual bool isPassable() const { return true; }
    [[nodiscard]] virtual int getHitPoints() const { return 0; }
    virtual void takeDamage(const int amount = 1) { (void)amount; /* Default implementation does nothing */ }

    friend std::ostream &lessThenOperator(std::ostream &os, const GameObject &element);
};

#endif //GAME_OBJECT_H
