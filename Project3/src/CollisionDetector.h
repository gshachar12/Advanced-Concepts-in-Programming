#ifndef COLLISION_DETECTOR_H
#define COLLISION_DETECTOR_H

#include "GameState.h"
#include <vector>

/**
 * Handles collision detection and resolution for all game entities
 */
class CollisionDetector {
public:
    struct CollisionEvent {
        enum Type {
            SHELL_HITS_WALL,
            SHELL_HITS_TANK,
            SHELL_HITS_SHELL,
            TANK_HITS_MINE,
            SHELL_OUT_OF_BOUNDS
        };
        
        Type type;
        Shell* shell = nullptr;
        Tank* tank = nullptr;
        Position position;
        int damage = 1;
    };

private:
    GameState* game_state_;
    std::vector<CollisionEvent> collision_events_;

public:
    explicit CollisionDetector(GameState* game_state);
    
    /**
     * Check for all collisions and generate collision events
     */
    void detectCollisions();
    
    /**
     * Process all collision events and modify game state accordingly
     */
    void resolveCollisions();
    
    /**
     * Check if a specific position would cause a collision for a tank
     */
    bool wouldTankCollide(const Tank* tank, const Position& new_pos) const;
    
    /**
     * Check if a shell at position would hit something
     */
    bool wouldShellHit(const Shell* shell, const Position& pos) const;
    
private:
    void detectShellCollisions();
    void detectTankCollisions();
    void processCollisionEvent(const CollisionEvent& event);
};

#endif // COLLISION_DETECTOR_H
