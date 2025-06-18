#ifndef SHELL_H
#define SHELL_H

#include "GameObject.h"
#include "Board.h"
#include "Directions.h"
#include "CellType.h"
#include "Tank.h"

class Shell : public GameObject {
private:
    int ownerPlayerIndex;  // Player who fired this shell (1 or 2)
    bool exploded = false;

public:
    Shell(int x, int y, Direction dir, int playerIndex)
        : GameObject(x, y, dir, CellType::SHELL), 
          ownerPlayerIndex(playerIndex) {}
    
    virtual ~Shell() = default;
    
    int getOwnerPlayerIndex() const { return ownerPlayerIndex; }
    bool hasExploded() const { return exploded; }

    // Move the shell in its current direction
    void update(Board& board, std::vector<std::unique_ptr<Tank>>& tanks, std::vector<Position>& explosions);
    
    // Check if this shell hits another object
    bool collidesWith(const GameObject& obj) const;
    
    // Create explosion effects
    void explode(int newX, int newY,std::vector<Position>& explosions);
    void explode(std::vector<Position>& explosions);

};

#endif // SHELL_H