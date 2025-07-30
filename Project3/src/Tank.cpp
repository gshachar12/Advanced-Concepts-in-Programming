#include "Tank.h"
#include "Board.h"

Tank::Tank(const Position& pos, Direction dir, int player_id, int tank_id)
    : position_(pos), direction_(dir), player_id_(player_id), tank_id_(tank_id),
      alive_(true), shells_remaining_(MAX_SHELLS), shoot_cooldown_(0) {
}

Position Tank::getForwardPosition() const {
    return position_ + DirectionUtils::directionToOffset(direction_);
}

Position Tank::getBackwardPosition() const {
    Direction opposite = DirectionUtils::getOpposite(direction_);
    return position_ + DirectionUtils::directionToOffset(opposite);
}

bool Tank::canMoveForward(const Board& board) const {
    Position next_pos = getForwardPosition();
    return board.isPassable(next_pos);
}

bool Tank::canMoveBackward(const Board& board) const {
    Position next_pos = getBackwardPosition();
    return board.isPassable(next_pos);
}

void Tank::moveForward() {
    position_ = getForwardPosition();
}

void Tank::moveBackward() {
    position_ = getBackwardPosition();
}

void Tank::rotateLeft45() {
    direction_ = DirectionUtils::rotateCounterClockwise45(direction_);
}

void Tank::rotateRight45() {
    direction_ = DirectionUtils::rotateClockwise45(direction_);
}

void Tank::rotateLeft90() {
    direction_ = DirectionUtils::rotateCounterClockwise90(direction_);
}

void Tank::rotateRight90() {
    direction_ = DirectionUtils::rotateClockwise90(direction_);
}

bool Tank::canShoot() const {
    return alive_ && shells_remaining_ > 0 && shoot_cooldown_ == 0;
}

void Tank::shoot() {
    if (canShoot()) {
        shells_remaining_--;
        shoot_cooldown_ = SHOOT_COOLDOWN_TURNS;
    }
}

void Tank::takeDamage() {
    destroy();
}

void Tank::update() {
    if (shoot_cooldown_ > 0) {
        shoot_cooldown_--;
    }
}

CellType Tank::getCellType() const {
    if (!alive_) {
        return CellType::EXPLOSION;
    }
    
    if (player_id_ == 1) {
        return CellType::TANK1;
    } else {
        return CellType::TANK2;
    }
}
