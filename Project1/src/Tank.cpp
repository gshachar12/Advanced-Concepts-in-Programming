#include "Tank.h"
#include <iostream>
#include <algorithm>


void Tank::update() {
    if (shootCooldown > 0) {
        shootCooldown--;
    }
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
        shootCooldown = 4; 
    } else {
        std::cerr << "Cannot shoot: either on cooldown or out of shells." << std::endl;
    }
}

std::pair<int,int>  Tank::moveForward() {
    auto [dx, dy] = Directions::directionToOffset(getDirection());
    int new_pos_x = (getX() + dx + Global::width) % Global::width;
    int new_pos_y = (getY() + dy + Global::height) % Global::height;
    return {new_pos_x, new_pos_y};
}

std::pair<int,int> Tank::moveBackward() {
    auto [dx, dy] = Directions::directionToOffset(getDirection());
    int new_pos_x = (getX() - dx + Global::width) % Global::width;
    int new_pos_y = (getY() - dy + Global::height) % Global::height;
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
    std::cout << "Tank " << tankID<< " is at (" << getX() << ", " << getY() << ")." << std::endl;
}

void Tank::printStatus() const {
    std::cout << "Tank "<< tankID<< " at (" << getX() << ", " << getY() << "), direction="
              << Directions::directionToString(getDirection())
              << ", shells=" << shellCount
              << ", cooldown=" << shootCooldown
              << ", alive=" << (alive ? "true" : "false") << std::endl;
}

// Optional rotation helpers
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
    // Use size_t to avoid the integer-sign mismatch warning.
    for (size_t i = 0; i < Directions::getAllDirections().size(); i++) {
        if (Directions::getAllDirections()[i] == d)
            return static_cast<int>(i);
    }
    return -1;
}

void Tank::setDirectionByIndex(int idx) {
    int n = Directions::getAllDirections().size();
    idx = (idx % n + n) % n;
    setDirection(Directions::getAllDirections()[idx]);
}

 Tank:: ~Tank(){}