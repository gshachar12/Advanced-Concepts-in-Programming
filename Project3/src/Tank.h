#ifndef TANK_H
#define TANK_H

#include "Position.h"
#include "Direction.h"
#include "CellType.h"

/**
 * Represents a tank in the game
 */
class Tank {
private:
    Position position_;
    Direction direction_;
    int player_id_;
    int tank_id_;
    bool alive_;
    int shells_remaining_;
    int shoot_cooldown_;
    
    // Constants
    static const int MAX_SHELLS = 16;
    static const int SHOOT_COOLDOWN_TURNS = 3;

public:
    /**
     * Constructor
     */
    Tank(const Position& pos, Direction dir, int player_id, int tank_id);
    
    /**
     * Get tank properties
     */
    const Position& getPosition() const { return position_; }
    Direction getDirection() const { return direction_; }
    int getPlayerId() const { return player_id_; }
    int getTankId() const { return tank_id_; }
    bool isAlive() const { return alive_; }
    int getShellsRemaining() const { return shells_remaining_; }
    int getShootCooldown() const { return shoot_cooldown_; }
    
    /**
     * Set tank properties
     */
    void setPosition(const Position& pos) { position_ = pos; }
    void setDirection(Direction dir) { direction_ = dir; }
    
    /**
     * Movement methods
     */
    Position getForwardPosition() const;
    Position getBackwardPosition() const;
    bool canMoveForward(const class Board& board) const;
    bool canMoveBackward(const class Board& board) const;
    void moveForward();
    void moveBackward();
    
    /**
     * Rotation methods
     */
    void rotateLeft45();
    void rotateRight45();
    void rotateLeft90();
    void rotateRight90();
    
    /**
     * Combat methods
     */
    bool canShoot() const;
    void shoot();
    void takeDamage();
    void destroy() { alive_ = false; }
    
    /**
     * Update tank state (called each turn)
     */
    void update();
    
    /**
     * Get cell type representation
     */
    CellType getCellType() const;
};

#endif // TANK_H
