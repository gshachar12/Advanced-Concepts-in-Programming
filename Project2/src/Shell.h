#ifndef SHELL_H
#define SHELL_H

#include "GameObject.h"
#include "Board.h"
#include "Directions.h"
#include "CellType.h"

class Shell : public GameObject {
private:
    int ownerPlayerIndex;  // Player who fired this shell (1 or 2)

public:
    Shell(int x, int y, Direction dir, int playerIndex)
        : GameObject(x, y, dir, CellType::SHELL), 
          ownerPlayerIndex(playerIndex) {}
    
    virtual ~Shell() = default;
    
    int getOwnerPlayerIndex() const { return ownerPlayerIndex; }
    
    // Move the shell in its current direction
    void update(Board& board);
    
    // Check if this shell hits another object
    bool collidesWith(const GameObject& obj) const;
    
    // Create explosion effects
    void explode(Board& board);
};

#endif // SHELL_H