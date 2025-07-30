#ifndef SHELL_H
#define SHELL_H

#include "Position.h"
#include "Direction.h"

/**
 * Represents a shell projectile in the game
 */
class Shell {
private:
    Position position_;
    Direction direction_;
    int owner_player_id_;
    int owner_tank_id_;
    bool active_;
    int distance_traveled_;
    
    // Constants
    static const int MAX_DISTANCE = 20;
    static const int MOVEMENT_SPEED = 2; // Moves 2 cells per turn

public:
    /**
     * Constructor
     */
    Shell(const Position& pos, Direction dir, int owner_player_id, int owner_tank_id);
    
    /**
     * Get shell properties
     */
    const Position& getPosition() const { return position_; }
    Direction getDirection() const { return direction_; }
    int getOwnerPlayerId() const { return owner_player_id_; }
    int getOwnerTankId() const { return owner_tank_id_; }
    bool isActive() const { return active_; }
    int getDistanceTraveled() const { return distance_traveled_; }
    
    /**
     * Shell movement
     */
    void advance();
    Position getNextPosition() const;
    
    /**
     * Shell state management
     */
    void deactivate() { active_ = false; }
    bool hasReachedMaxDistance() const;
    
    /**
     * Check if shell would hit target at position
     */
    bool wouldHitAt(const Position& target) const;
};

#endif // SHELL_H
