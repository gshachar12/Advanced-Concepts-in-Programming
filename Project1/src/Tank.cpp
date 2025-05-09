#include "Tank.h"
#include <iostream>
#include <algorithm>

#include "Board.h"

// Constants for tank properties
const int DEFAULT_SHELL_COUNT = 16;
const int SHOOT_COOLDOWN = 4;

void Tank::update() {
    if (shootCooldown > 0) {
        shootCooldown--;
    }
    
    // Update backward movement state
    switch (backwardState) {
        case BackwardState::WAITING_1:
            backwardState = BackwardState::WAITING_2;
            break;
        case BackwardState::WAITING_2:
            backwardState = BackwardState::MOVING_BACKWARD;
            break;
        case BackwardState::MOVING_BACKWARD:
            backwardState = BackwardState::NOT_REQUESTED;
            break;
        default:
            break;
    }
}

void Tank::shoot() {
    if (canShoot()) {
        shellCount = std::max(0, shellCount - 1);
        shootCooldown = SHOOT_COOLDOWN; 
    } else {
        std::cerr << "Cannot shoot: either on cooldown or out of shells." << std::endl;
    }
}

std::pair<int,int>  Tank::moveForward(Board board) {
    auto [dx, dy] = Directions::directionToOffset(getDirection());
    auto [new_pos_x, new_pos_y] = tryToMove( board, dx, dy); 
    return {new_pos_x, new_pos_y};
}

std::pair<int,int> Tank::moveBackward(Board board) {
    auto [dx, dy] = Directions::directionToOffset(getDirection());
    int new_pos_x = (getX() - dx + board.getWidth()) % board.getWidth();
    int new_pos_y = (getY() - dy + board.getHeight()) % board.getHeight();
    return {new_pos_x, new_pos_y};
}

void Tank::requestBackward() {
    if (backwardState == BackwardState::NOT_REQUESTED) {
        backwardState = BackwardState::WAITING_1;
    }
}

void Tank::cancelBackward() {
    if (backwardState == BackwardState::WAITING_1 ||
        backwardState == BackwardState::WAITING_2) {
        backwardState = BackwardState::NOT_REQUESTED;
    }
}

void Tank::printPosition() {
    std::cout << "Tank " << tankID << " is at (" << getX() << ", " << getY() << ")." << std::endl;
}

void Tank::printStatus()  {
    std::cout << "Tank "<< tankID << " at (" << getX() << ", " << getY() << "), direction="
              << Directions::directionToString(getDirection())
              << ", shells=" << shellCount
              << ", cooldown=" << shootCooldown
              << ", alive=" << (alive ? "true" : "false") << std::endl;
}

// Rotation helpers
void Tank::rotateLeft1_8() {
    int idx = findDirectionIndex(getDirection());
    if (idx >= 0) {
        setDirectionByIndex(idx - 1);
    }
}

void Tank::rotateRight1_8() {
    int idx = findDirectionIndex(getDirection());
    if (idx >= 0) {
        setDirectionByIndex(idx + 1);
    }
}

void Tank::rotateLeft1_4() {
    int idx = findDirectionIndex(getDirection());
    if (idx >= 0) {
        setDirectionByIndex(idx - 2);
    }
}

void Tank::rotateRight1_4() {
    int idx = findDirectionIndex(getDirection());
    if (idx >= 0) {
        setDirectionByIndex(idx + 2);
    }
}

int Tank::findDirectionIndex(const Direction &d) const {
    const auto& all_directions = Directions::getAllDirections();
    for (size_t i = 0; i < all_directions.size(); i++) {
        if (all_directions[i] == d)
            return static_cast<int>(i);
    }
    return -1;
}

void Tank::setDirectionByIndex(int idx) {
    const auto& all_directions = Directions::getAllDirections();
    int n = all_directions.size();
    idx = (idx % n + n) % n;  // Ensure positive modulo
    setDirection(all_directions[idx]);
}

Tank::~Tank() {}
