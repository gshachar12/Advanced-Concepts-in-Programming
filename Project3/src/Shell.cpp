#include "Shell.h"

Shell::Shell(const Position& pos, Direction dir, int owner_player_id, int owner_tank_id)
    : position_(pos), direction_(dir), owner_player_id_(owner_player_id), 
      owner_tank_id_(owner_tank_id), active_(true), distance_traveled_(0) {
}

void Shell::advance() {
    if (!active_) {
        return;
    }
    
    Position offset = DirectionUtils::directionToOffset(direction_);
    
    // Move MOVEMENT_SPEED cells per turn
    for (int i = 0; i < MOVEMENT_SPEED && active_; ++i) {
        position_ = position_ + offset;
        distance_traveled_++;
        
        // Check if reached maximum distance
        if (distance_traveled_ >= MAX_DISTANCE) {
            deactivate();
            break;
        }
    }
}

Position Shell::getNextPosition() const {
    Position offset = DirectionUtils::directionToOffset(direction_);
    return position_ + offset;
}

bool Shell::hasReachedMaxDistance() const {
    return distance_traveled_ >= MAX_DISTANCE;
}

bool Shell::wouldHitAt(const Position& target) const {
    return active_ && position_ == target;
}
